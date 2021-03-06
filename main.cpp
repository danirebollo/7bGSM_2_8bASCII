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

//#define DEBUG
//const char *ALPHABET = "@£$¥èéùìòÇ\nØø\rÅåΔ ΦΓΛΩΠΨΣΘΞ ÆæßÉ !\"#¤%&'()*+,-./0123456789:;<=>?"
//                       "¡ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÑÜ§¿abcdefghijklmnopqrstuvwxyzäöñüà";

//uint8_t CODECMAP[20][20]={
//    {36,    64, },
//    {2,     0,  }};

void encodeOneByte(uint8_t input, uint8_t *encoded)
{
    //TODO reverse decoder.

    //First get special characters
    if (input == 36)
    {
        *encoded = 2;
        return;
    }
    else if (input == 64)
    {
        *encoded = 0;
        return;
    }
    else if (((input >= 0) && (input <= 9)) || (input == 11) || (input == 12) || ((input >= 14) && (input <= 31)) || ((input >= 91) && (input <= 95)) || ((input >= 123) && (input <= 127)))
    {
        //return SPACE character if unknown
        *encoded = 32;
        return;
    }

    *encoded = input;
}

uint8_t decodeOneByte(uint8_t input)
{
    if (input == 0)
    {
        return 64;
    }
    else if (input == 2)
    {
        return 36;
    }
    else if (((input >= 0) && (input <= 9)) || (input == 11) || (input == 12) || ((input >= 14) && (input <= 31)) || ((input >= 91) && (input <= 95)) || ((input >= 123) && (input <= 127)))
    {
        //return SPACE character if unknown
        return 32;
    }

    return input;
}

/* Decodes a 7 bit GSM encoding string to 8 bit ASCII null terminated string.
 *
 * input: ASCII 7 bit GSM string
 * length7bits: Length in bytes of the input string
 * decoded: Output with the decoded 8 bit ASCII string
 *
 * returns: nothing
 */
void decode(uint8_t *input, uint32_t length7bits, uint8_t *decoded, uint32_t decodedlen)
{
#ifdef DEBUG
    printf("decode:: length7bits: %d, decodedlen:%d\r\n", length7bits, decodedlen);
#endif
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
                decoded[decodedTempcounter] = decodeOneByte(myValueSub1);
#ifdef DEBUG
                printf("decode== decodedTempcounter:'%d', i:%d, sevencounter: %d, decodedTempcounter: %d, input: %d, myValue:%d, decoded: %x, decoded: %x\r\n", decodedTempcounter, i, sevencounter, decodedTempcounter, input[i], myValue, myValueSub1, decoded[decodedTempcounter]);
#endif
                decodedTempcounter++;
            }
            uint16_t myValueSub2 = (myValue >> 7 - sevencounter) & 0x7F;
            decoded[decodedTempcounter] = decodeOneByte(myValueSub2);

#ifdef DEBUG
            printf("decode:: decodedTempcounter:'%d', i:%d, sevencounter: %d, decodedTempcounter: %d, input: %d, myValue:%d, decoded: %x, decoded: %x\r\n", decodedTempcounter, i, sevencounter, decodedTempcounter, input[i], myValue, myValueSub2, decoded[decodedTempcounter]);
#endif
            decodedTempcounter++;

            sevencounter++;
            if (sevencounter > 6)
            {
                sevencounter = 0;
            }
            i++;
        }
    }
    decoded[decodedlen] = '\0';

//TODO
#ifdef DEBUG
    printf("decode:: input = '%s': ", input);
    for (int i = 0; i < length7bits; i++)
    {
        printf("'%x',", input[i]);
    }
    printf("\r\n");
    printf("decode:: output = '%s': ", (uint8_t *)decoded);
    for (int i = 0; i < decodedlen; i++)
    {
        printf("'%x',", decoded[i]);
    }
    printf("\r\n");
    printf("decode:: length7bits = '%d'\r\n", length7bits);
    printf("decode:: decoded = '%s'\r\n", decoded);
#endif
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
#ifdef DEBUG
    printf("encode inputsize: '%d', outputsize: '%d' \r\n", inputsize, encodedLen);
#endif

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
#ifdef DEBUG
        printf("encode XX %d , %d\r\n", i, sevencounter);
#endif

        if (sevencounter == 0)
        {
            sevencounter++;
            continue;
        }
#ifdef DEBUG
        printf("encodedarray[i - 1]-[x]: %x - %x \r\n", encodedarray[i - 1], encodedarray[i]);
#endif

        uint8_t ea1 = ((encodedarray[i - 1] >> (sevencounter - 1)) & 0x7F);
        uint8_t ea2 = 0;
        if (sevencounter == 7)
            ea2 = 13;
        if (i != inputsize)
            ea2 = encodedarray[i];
        ea2 = (ea2 << (8 - sevencounter));

        encoded[i - c1] = ea1 + ea2; //It is already a 8b var, no need to force to &  0xFF
#ifdef DEBUG
        printf("encode IN (%d) i:%d , 7cnt: %d: %x-%x: %x\r\n", i - c1, i, sevencounter, ea1, ea2, encoded[i - c1]);
#endif

        sevencounter++;
    }
}

/* Tests an encoded string, it first decodes the message and prints the ASCII output and then
* reencodes it back to compare with the original encoded version, if the encoded string does
 * not match the original one it will print an error message.
 *
 * encodedText: String with the 7 bit encoded text
 *
 * returns: success
 */

bool testCoDec(const uint8_t *encodedText, uint8_t encsize)
{
#ifdef DEBUG
    printf("--- ----> STARTING TEST <---- ---\r\n");
#endif
    //decode
    uint8_t originalLen = encsize;
    //Needed to fix @ character in first place -alone: "\x0"-. 
    //Calling this function with uint8_t instead char it is not needed.
    //bug described in README.
    if (originalLen == 0 && encodedText != nullptr && encodedText[0] == 0) 
        originalLen = 1;

    uint8_t charactersNum = ((originalLen * 8) / 7);
#ifdef DEBUG
    printf("Text over test: = '%s'. original size: %d, encoded size: %d \r\n", encodedText, originalLen, charactersNum);
#endif
    char decoded[charactersNum + 1] = {0};
    decode((uint8_t *)encodedText, originalLen, (uint8_t *)decoded, charactersNum);
#ifdef DEBUG
    printf("Decoded = '%s' \r\n", decoded);
#endif
    //end decode

    //Encode
    int inputsize = strlen(decoded);
    int encodedLen = inputsize - (inputsize / 8);
    uint8_t *encoded;
    encoded = new uint8_t[encodedLen + 1];
    encoded[encodedLen] = '\0';
    encode((char *)decoded, inputsize, encoded, encodedLen + 1);

#ifdef DEBUG
    printf("Encoded = '%s': ", encoded);
    for (int i = 0; i < encodedLen; i++)
    {
        printf("'%x',", encoded[i]);
    }
    printf("\r\n");
#endif
    //end encode

    bool success = false;
    if (memcmp(encodedText, encoded, originalLen))
    {
#ifdef DEBUG
        printf("@@@ @@@ ----> FAIL <---- @@@ @@@\r\nError encode result does not match original encoded text\r\n");
#endif
    }
    else
    {
#ifdef DEBUG
        printf("### ----> PASS <---- ###\r\n");
#endif
        success = true;
    }
    free(encoded);
    return success;
}
bool testCoDec(const char *encodedText)
{
    return testCoDec((uint8_t *)encodedText, strlen(encodedText));
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
    uint8_t testarray1[] = {0x00, 0x00, 0x00};
    uint8_t testarray2[] = {0x31, 0xD9, 0x8C, 0x56, 0xB3, 0xDD, 0x70};

    if (testCoDec("\x31\xD9\x8C\x56\xB3\xDD\x70"))
        if (testCoDec("\x48"))
            if (testCoDec(" "))
                if (testCoDec(testarray1, sizeof(testarray1) / sizeof(uint8_t)))
                    if (testCoDec(testarray2, sizeof(testarray2) / sizeof(uint8_t)))
                        if (testCoDec("\x00"))
                            if (testCoDec("\xc8\x20"))
                                if (testCoDec("\xC8\x32\x9B\xFD\x6E\x28\xEE\x6F\x39\x9B\x0C"))
                                    if (testCoDec("\x54\x74\x19\x04\x97\xA7\xC7\x65\x50\x7A\x0E\x8A\xC1\x04"))
                                        if (testCoDec("\x31\xD9\x8C\x56\xB3\xDD\x70"))
                                            if (testCoDec("\x54\x74\x7A\x0E\x3A\x52\xA7\x20\x72\xD9\x9C\x76\x97\xE7\x20\x3A\xBA\x0C\x62\x87\xDD\xE7\x7A\xF8\x5C\x6E\xCD\xE1\xE5\x71\xDA\x9C\x1E\x83\xE4\xE5\x78\x3D\x2D\x2F\xB7\xCB\x6E\xFA\x1C\x64\x7E\xCB\x41\xC7\x69\x13\x74\x4F\xD3\xD1\x69\x37\x88\x8E\x2E\x83\xC8\xE9\x73\x9A\x1E\x66\x83\xC6\x65\x36\xBB\xCE\x0E\xCB\xE9\x65\x76\x79\xFC\x6E\xB7\xEB\xEE\xF4\x38\x4C\x4F\xBF\xDD\x73\xD0\x3C\x3F\xA7\x97\xDB\x20\x14\x14\x1D\x9E\x97\x41\xB2\x17\x14\x1D\x9E\x97\x41\xB2\x55\xCA\x05"))
                                                if (testCoDec("\xD3\xB2\x9B\x0C\x0A\xBB\x41\xE5\x76\x38\xCD\x06\xD1\xDF\xA0\xB4\xDB\xFC\x06\xA4\xDD\xF4\xB2\x9B\x9C\x0E\xBB\xC6\xEF\x36"))
                                                {
                                                    printf("----------------------------------------------\r\n[[ CODEC WORKS!! ]]");
                                                    return 0;
                                                }
    printf("----------------------------------------------\r\n$$ BAD TEST :( $$");
}