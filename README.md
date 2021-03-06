= GSM CODEC 03.38 Instrucvtions =
The decoder must transform GSM 03.38 code to ASCII, and ASCII to GSM 03.38.

1- Translate every character which has a valid codification in ASCII and GSM 03.38, some characters are impossible 
   to translate between both encodings. When a character cannot be translated it shall be substituted by a white space (“ “).

2- You can modify the base code as you wish, add extra tests, etc. Do not hesitate to make changes to the code base if you find any error or possible improvement.

3- A Makefile with basic options must be provided.

4- Code must be optimized to use little Flash and little RAM, as it would if you were coding FW.

5- To submit your solution for evaluation, you must create a private repository in your own GitHub account. Name the new repository “LastnamesFirstname-Geotab-FwChallenge”, 
   upload your code in a single commit in branch “master”, and add users @DVLG and @csanchezdll as collaborators to that repository.

= Known bugs =
-'@' character problem: 
(0x00 in GSM encoding, 0x40 in ASCII) cant be the first character in the decoding string. This is a C limitation since hex tring with 0x0 is interpreted as null value.

One way to solve this will be using uint8 arrays instead strings.       

For this reason "testCoDec" function is splited into: 

bool testCoDec(const char *encodedText)

and 

bool testCoDec((uint8_t *)encodedText, originalLen)

so function can be called directly with uint8_t array full with 0x0 characters without problem. 

SOLVED


= Debug options =
Comment/uncomment preprocessor directive "#define DEBUG" (main.cpp line 17) to see debug traces.

= TODO =
-Adjunst variable sizes

-Check RAM optimization