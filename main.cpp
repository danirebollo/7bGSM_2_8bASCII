//GSM CODEC 03.38
// The decoder must transform GSM 03.38 code to ASCII, and ASCII to GSM 03.38.
/*  1- Translate every character which has a valid codification in ASCII and GSM 03.38, some characters are impossible 
        to translate between both encodings. When a character cannot be translated it shall be substituted by a white space (“ “).
    2- You can modify the base code as you wish, add extra tests, etc. Do not hesitate to make changes to the code base if you find any error or possible improvement.
    3- A Makefile with basic options must be provided.
    4- Code must be optimized to use little Flash and little RAM, as it would if you were coding FW.
    5- To submit your solution for evaluation, you must create a private repository in your own GitHub account. Name the new repository “LastnamesFirstname-Geotab-FwChallenge”, 
        upload your code in a single commit in branch “master”, and add users @DVLG and @csanchezdll as collaborators to that repository.
        */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BUFFERSIZE 256

//const char *ALPHABET = "@£$¥èéùìòÇ\nØø\rÅåΔ ΦΓΛΩΠΨΣΘΞ ÆæßÉ !\"#¤%&'()*+,-./0123456789:;<=>?"
//                       "¡ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÑÜ§¿abcdefghijklmnopqrstuvwxyzäöñüà";

bool isValidTextArray(uint8_t *text)
{
    if (text == nullptr)
    {
        printf("textChecks:: CAUTION. nullptr\r\n");
        return false;
    }
    //TODO change BUFFERSIZE for dynamic allocation size pointer
    if (strlen((const char *)text) >= BUFFERSIZE || strlen((const char *)text) <= 0)
    {
        printf("textChecks:: CAUTION. buffer overflow attempt\r\n");
        return false;
    }
    return true;
}

void encodeOneByte(uint8_t input, uint8_t *encoded)
{
    //TODO reverse decoder.

    //First get special characters
    if ((input >= 0 && input <= 31) || (input == 36) || (input >= 58 && input <= 64) || (input >= 91 && input <= 96) || (input >= 123 && input <= 127))
    {
        *encoded = ' ';
        //TODO transform special characters

        //printf("encode:: character '%d'-'%c'\r\n", input, *encoded);
        return;
    }

    //if it is not special then is ASCII

    *encoded = input;
    //printf("encode:: character '%d'-'%c'\r\n", input, *encoded);
}

void decodeOneByte(uint8_t input, char *decoded)
{
    //First get special characters
    if ((input >= 0 && input <= 31) || (input == 36) || (input >= 58 && input <= 64) || (input >= 91 && input <= 96) || (input >= 123 && input <= 127))
    {
        *decoded = ' ';
        //TODO transform special characters

        //printf("decode:: character '%d'-'%c'\r\n", input, *decoded);
        return;
    }

    //if it is not special then is ASCII

    *decoded = input;
    //printf("decode:: character '%d'-'%c'\r\n", input, *decoded);
}

/* Decodes a 7 bit GSM encoding string to 8 bit ASCII null terminated string.
 *
 * input: ASCII 7 bit GSM string
 * length7bits: Length in bytes of the input string
 * decoded: Output with the decoded 8 bit ASCII string
 *
 * returns: nothing
 */
void decode(uint8_t *input, uint32_t length7bits, char *decoded, uint32_t decodedlen)
{
    if ((length7bits == 1) && (input[0] > 127))
    {
        printf("decode:: CAUTION. input error ('%d'>127)\r\n", input[0]);
        return;
    }
    else
    {
        uint16_t decodedTempcounter = 0;
        uint16_t sevencounter = 0;
        int i = 0;
        for (decodedTempcounter = 0; decodedTempcounter < decodedlen;)
        {
            uint16_t v2 = 0;
            if (i < length7bits - 1)
                v2 = (input[i + 1] << 8);
            uint16_t myValue = (v2 + input[i]);

            if (sevencounter == 0)
            {
                uint8_t myValueSub1 = myValue & 0x7F;
                decodeOneByte(myValueSub1, &decoded[decodedTempcounter]);
                decodedTempcounter++;
            }
            uint8_t myValueSub2 = (myValue >> 7 - sevencounter) & 0x7F;
            decodeOneByte(myValueSub2, &decoded[decodedTempcounter]);
            decodedTempcounter++;

            sevencounter++;
            if (sevencounter > 6)
                sevencounter = 0;
        }
    }

    //TODO
    printf("decode:: input = '%s'\r\n", input);
    printf("decode:: length7bits = '%d'\r\n", length7bits);
    printf("decode:: decoded = '%s'\r\n", decoded);
}

/* Encodes an ASCII string to 7 bit GSM encoding.
 *
 * input: ASCII null terminated string
 * encodedLen: Output with the encoded length
 * encoded: Output with the encoded 7 bit GSM string
 *
 * returns: nothing
 */
void encode(char *input, uint32_t inputsize, uint8_t *encoded, uint32_t encodedLen)
{
    printf("encode inputsize: '%d', outputsize: '%d' \r\n", inputsize, encodedLen);

    //TODO
    uint8_t sevencounter = 0;
    uint8_t encodedarray[inputsize];
    int encodedcounter = 0;
    int c1 = 1;

    for (int i = 0; i <= inputsize; i++)
    {
        if (sevencounter > 7)
        {
            sevencounter = 0;
            c1++;
        }
        encodeOneByte(input[i], &encodedarray[i]);
        printf("encode XX %d , %d\r\n", i, sevencounter);

        if (sevencounter == 0)
        {
            sevencounter++;
            continue;
        }

        printf("encodedarray[i - 1]-[x]: %x - %x \r\n", encodedarray[i - 1], encodedarray[i]);

        uint8_t ea1 = ((encodedarray[i - 1] >> (sevencounter - 1)) & 0x7F);
        uint8_t ea2 = 0;
        if (sevencounter == 7)
            ea2 = 13;
        if (i != inputsize)
            ea2 = encodedarray[i];
        ea2 = (ea2 << (8 - sevencounter));

        encoded[i - c1] = ea1 + ea2; //It is already a 8b var, no need to force to &  0xFF
        printf("encode IN (%d) i:%d , 7cnt: %d: %x-%x: %x\r\n", i - c1, i, sevencounter, ea1, ea2, encoded[i - c1]);

        sevencounter++;
    }
}

/* Tests an encoded string, it first decodes the message and prints the ASCII output and then
* reencodes it back to compare with the original encoded version, if the encoded string does
 * not match the original one it will print an error message.
 *
 * encodedText: String with the 7 bit encoded text
 *
 * returns: nothing
 */
void testCoDec(uint8_t *encodedText)
{
    if (!isValidTextArray(encodedText))
        return;

    char *decoded;
    uint8_t *encoded;
    uint8_t outputLen;
    uint32_t originalLen = strlen((const char *)encodedText);

    uint8_t charactersNum = (originalLen * 8) / 7;

    decoded = new char[charactersNum + 1];
    decoded[charactersNum] = '\0';

    decode((uint8_t *)encodedText, originalLen, decoded);
    printf("Decoded = '%s' \r\n", decoded);

    int inputsize = strlen((const char *)decoded);
    int encodedLen = (inputsize * 7) / 8;
    encoded = new uint8_t[encodedLen + 1];
    encoded[encodedLen] = '\0';
    encode((char *)decoded, inputsize, encoded, encodedLen);


    if (memcmp(encodedText, encoded, originalLen))
        printf("Error encode result does not match original encoded text\r\n");
}

void testCoDec(const char *encodedText)
{
    testCoDec((uint8_t *)encodedText);
}

/* Test program for the 7BIT CodDec
 *
 * argc: Argument count
 * argv: Argument list
 *
 * returns: program exit status
 */
int main(int argc, char **argv)
{
    testCoDec("\xC8\x32\x9B\xFD\x6E\x28\xEE\x6F\x39\x9B\x0C");
    testCoDec("\x54\x74\x19\x04\x97\xA7\xC7\x65\x50\x7A\x0E\x8A\xC1\x04");
    testCoDec("\x31\xD9\x8C\x56\xB3\xDD\x70");
    testCoDec("\x54\x74\x7A\x0E\x3A\x52\xA7\x20\x72\xD9\x9C\x76\x97\xE7\x20\x3A\xBA\x0C\x62\x87\xDD\xE7\x7A\xF8\x5C\x6E\xCD\xE1\xE5\x71\xDA\x9C\x1E\x83\xE4\xE5\x78\x3D\x2D\x2F\xB7\xCB\x6E\xFA\x1C\x64\x7E\xCB\x41\xC7\x69\x13\x74\x4F\xD3\xD1\x69\x37\x88\x8E\x2E\x83\xC8\xE9\x73\x9A\x1E\x66\x83\xC6\x65\x36\xBB\xCE\x0E\xCB\xE9\x65\x76\x79\xFC\x6E\xB7\xEB\xEE\xF4\x38\x4C\x4F\xBF\xDD\x73\xD0\x3C\x3F\xA7\x97\xDB\x20\x14\x14\x1D\x9E\x97\x41\xB2\x17\x14\x1D\x9E\x97\x41\xB2\x55\xCA\x05");
    testCoDec("\xD3\xB2\x9B\x0C\x0A\xBB\x41\xE5\x76\x38\xCD\x06\xD1\xDF\xA0\xB4\xDB\xFC\x06\xA4\xDD\xF4\xB2\x9B\x9C\x0E\xBB\xC6\xEF\x36");
}