/* lab2.c
 * Nitya Harikumar    <<-- you must change to your name and id
 * C12035452
 * ECE 2220, Fall 2022
 * MP2 
 *
 * Purpose:  The purpose is to use bitwise operators to encode, decode, and 
 * check ASCII characters.
 *
 * Assumptions:
 *  #1:  The menu driven input was provided and must be used exactly
 *       as written.  A user can enter commands:
 *            enc CUt
 *            dec 0E8A549C
 *            quit
 *       Encoding takes three printable ASCII letters
 *       Decoding takes up to eight HEX digits. If exactly eight digits are
 *            entered, the first digit must be 0 or 1.
 *            Leading zeros can be dropped.
 *
 *  #2:  The string and character type libraries cannot be used except as
 *       already provided.  These libraries are for checking inputs in main
 *       and in printing after decoding is complete.  They cannot be used
 *       for anyother purpose.
 *
 *  #3:  No arrays can be used (excpet as already provided for collecting
 *       keyboard input).  You must use bitwise operators for all encoding
 *       and decoding.  If you want to use an array as a lookup table you
 *       must first propose your design and get it approved.  Designs that
 *       use tables to avoid bitwise operators will not be approved.  There
 *       are many good and simple designs that do not require tables.
 *
 *  #4   No changes to the code in main.  Your code must be placed in
 *       functions.  Additional functions are encouraged.
 *
 * Bugs:
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 100

// function prototypes
void encode(unsigned char first_letter, unsigned char second_letter,
        unsigned char third_letter);
void decode(unsigned int codeword);

int main()
{
    char line[MAXLINE];
    char command[MAXLINE];
    char inputcs[MAXLINE];
    int  items;
    int i, invalid;
    unsigned int codeword;

    printf("\nMP2: encoding and decoding (29, 24) Hamming code.\n");
    printf("Commands:\n\tenc 3-letters\n\tdec 8-hex-digits\n\tquit\n");

    // each call to fgets, collects one line of input and stores in line
    while (fgets(line, MAXLINE, stdin) != NULL) {
        items = sscanf(line, "%s%s", command, inputcs);
        if (items == 1 && strcmp(command, "quit") == 0) {
            break;
        } else if (items == 2 && strcmp(command, "enc") == 0) {
            // encoding
            if (strlen(inputcs) != 3 || !isprint(inputcs[0]) ||
                    !isprint(inputcs[1]) || !isprint(inputcs[2])) {
                printf("Invalid input to encoder: %s\n", inputcs);
            } else {
                encode(inputcs[0], inputcs[1], inputcs[2]);
            }
        } else if (items == 2 && strcmp(command, "dec") == 0) {
            // decoding: convert hex digits to integer
            items = sscanf(inputcs, "%x", &codeword);
            if (items != 1 || strlen(inputcs) > 8) {
                printf("Invalid input to decoder: %s\n", inputcs);
            } else {
                // verify all digits are hex characters because
                // scanf does not reject invalid letters
                for (i=0, invalid=0; i < strlen(inputcs) && !invalid; i++) {
                    if (!isxdigit(inputcs[i]))
                        invalid = 1;
                }
                // if 8 digits, leading digit must be 1 or 0
                if (invalid) {
                    printf("Invalid decoder digits: %s\n", inputcs);
                } else if (strlen(inputcs) == 8 && inputcs[0] != '1'
                        && inputcs[0] != '0') {
                    printf("Invalid decoder leading digit: %s\n", inputcs);
                } else {
                    decode(codeword);
                }
            }
        } else {
            printf("# :%s", line);
        }
    }
    printf("Goodbye\n");
    return 0;
}

/* encode: calculates parity bits and prints codeword
 *
 * input: three ASCII characters
 * assumptions: input is valid
 *
 * Example: if input letters are is 'C', 'U', and 't'
 * the final print must be:
 *  ---01110 10001010 01010100 10011100
 *  Codeword: 0x0E8A549C
 */
void encode(unsigned char first_letter, unsigned char second_letter,
        unsigned char third_letter)
{
    unsigned int codeword = 0;
    printf("%9s%9c%9c%9c\n", "Encoding:", third_letter, second_letter, first_letter);
    printf(" 0x    00%9x%9x%9x\n ", third_letter, second_letter, first_letter);

    // ADD your code here
    //
    // hexnum is the same as the information word
    unsigned int hexNum = 0; 
    
    //place ASCII character values into respective bits of hexNum
    hexNum = (hexNum  & 0xFF00FFFF) | (first_letter << 0);
    hexNum = (hexNum & 0xFFFF00FF) | (second_letter << 8);
    hexNum = (hexNum & 0xFF00FFFF) | (third_letter << 16);
    
    //placing information word into codeword but considering parity bit spacing
    codeword = (hexNum & 0x1) << 2; //bit 1
    codeword = (codeword & 0xF) | ((hexNum & 0xE)<<3); //bits 2-4
    codeword = (codeword & 0xFF) | ((hexNum & 0x7F0) << 4); //bits 5-11
    codeword = (codeword & 0xFFFF) | ((hexNum & 0xFFF800) << 5); //bits 12-24    
   
    // you must print the info_word in binary format
    for (int i =31; i >=0; i--){
	if (i<=31 && i > 23){
		printf ("-");
		continue;
	}
	else if (i == 23 || i==15 || i== 7)
		printf (" ");

	printf("%d", hexNum >> i & 0x1);
    }
    printf("\n");
    
    //calculating parity bits
    int p1=0,p2=0,p4=0,p8=0,p16=0,sum=0, parity =0;
    
    //outside loop for each parity bit 
    for (int k = 1; k <= 16;k*=2){
	sum = 0; 
	//loop for jumps
    	for (int j = k-1; j<=28; j+=2*k){
		//loop for getting info bit
		for (int i = 0; i < k ; i++){
			if ( j!= k-1 || i != 0){
				int temp = (codeword >> (j+i)) & 0x1;
				sum += temp; 
			}
		}	
	}
	if (sum%2 == 1){
		switch (k){
			case 1:
				p1 = 1;
				parity |= 0x1;
				break;
			case 2:
				p2 = 1;
				parity |= 0x2;
				break;
			case 4:
				p4 = 1;
				parity |= 0x8;
				break;
			case 8:
				p8 = 1;
				parity |= 0x80;
				break;
			case 16:
				p16 =1;
				parity |= 0x8000;
				break;
		}
	}
    }

    //putting parity bits into codeword
    codeword = codeword | parity;

   // print the information word in binary form with spaces
    printf("P1 : %d\n", p1);
    printf("P2 : %d\n", p2);
    printf("P4 : %d\n", p4);
    printf("P8 : %d\n", p8);
    printf("P16: %d\n ", p16);

    // print the codeword bits in binary form with spaces
    for (int i =31; i >=0; i--){
	if (i<=31 && i > 28){
		printf ("-");
		continue;
	}
	else if (i == 23 || i==15 || i== 7)
		printf (" ");
    	printf("%d", codeword >> i & 0x1);
    }
    printf("\n");
	

    // print the codeword in hex format
    printf(" Codeword: 0x%.8X\n", codeword);
    printf("\n");
}

/* decode: checks parity bits and prints information characters
 *
 * input: A 29-bit codeword
 * assumptions: the codeword has either no or only one error.
 *
 *              the information characters may not be printable
 *
 * FYI: when a codeword has more than one error the decoding algorithm
 * may generate incorrect information bits.  In a practical system
 * inforamtion is grouped into multiple codewords and a final CRC
 * code verifies if all codewords are correct.  We will not
 * implement all of the details of the system in this project.
 *
 * Example: if the codeword is 0x0E8A549C
 * the final print must be:
 *  No error
 *  -------- 01110100 01010101 01000011
 *  Information Word: 0x745543 (CUt)
 *
 * Example with one error in codeword bit 21: 0x0E9A549C
 * Notice the 8 in the previous example has been changed to a 9
 * the final print must be:
 *  Corrected bit: 21
 *  -------- 01110100 01010101 01000011
 *  Information Word: 0x745543 (CUt)
 */
void decode(unsigned int codeword)
{
    // you must determine these values:
    unsigned int info_word = 0;
    unsigned char first_letter = 0x42;
    unsigned char second_letter = 0x61;
    unsigned char third_letter = 0x64;
    int bit_error_location = 0;

    printf("Decoding: 0x%.8X\n", codeword);

    //Fixes  a bit error if one exists  
    int e1=0, e2=0, e4=0, e8=0, e16=0, sum =0, parity = 0;
    //outer loop for each parity bit
    for (int k = 1; k <= 16;k*=2){
	sum = 0; 
	// loop for number of jumps
	for (int j = k-1; j<=28; j+=2*k){
		// loop for error checking each bit
		for (int i = 0; i < k ; i++){
			if ( j== k-1 && i == 0){
				parity = codeword >> (j+i) & 0x1;
			}
			else{
				int temp = (codeword >> (j+i)) & 0x1;
				sum += temp;
			}	
		}	
	}
	if (parity != sum%2){
		switch (k){
			case 1:
				e1 = 1;
				bit_error_location |= 0x1;
				break;
			case 2:
				e2 = 1;
				bit_error_location |= 0x2;
				break;
			case 4:
				e4 = 1;
				bit_error_location |= 0x4;
				break;
			case 8:
				e8 = 1;
				bit_error_location|= 0x8;
				break;
			case 16:
				e16 =1;
				bit_error_location |= 0x10;
				break;
		}
	}
    }
   
    // print the error location bits, one bit per line.
    printf("E1 : %d\n", e1);
    printf("E2 : %d\n", e2);
    printf("E4 : %d\n", e4);
    printf("E8 : %d\n", e8);
    printf("E16: %d\n", e16);

    //print bit error location 
    if (bit_error_location == 0)
        printf(" No error\n ");
    else if (bit_error_location > 0 && bit_error_location <= 29) {
        printf(" Corrected bit: %d\n ", bit_error_location);
	codeword = codeword^(1<< (bit_error_location-1));
    } else
        printf(" Decoding failure: %d\n ", bit_error_location);

    //set info word from code word (remove parity bits)
    info_word = (codeword & 0x4) >>  2; //bit 1
    info_word = (info_word & 0x1) | ((codeword & 0x70)>>3); //bits 2-4
    info_word = (info_word & 0xF) | ((codeword & 0x7F00)>> 4); //bits 5-11
    info_word = (info_word & 0x7FF) | ((codeword & 0x1FFF0000) >> 5); //bits 12-24    
    //print info word in binary based on specific formatting
    for (int i =31; i >=0; i--){
	if (i<=31 && i > 23){
		printf ("-");
		continue;
	}
	else if (i == 23 || i==15 || i== 7)
		printf (" ");

	printf("%d", info_word >> i & 0x1);
    }
   
    printf ("\n");

    // print the information word in hex and set ASCII characters from info word
    third_letter = (info_word  & 0x00FF0000) >> 16;
    second_letter = (info_word & 0x0000FF00) >> 8;
    first_letter = (info_word  & 0x000000FF) >> 0;
    
    printf(" Information Word: 0x%X%X%X", third_letter, second_letter, first_letter);
    // only print information word as letters if 7-bit printable ASCII
    // otherwise print a space for non-printable information bits
    if ((first_letter & 0x80) == 0 && isprint(first_letter))
        printf(" (%c", first_letter);
    else
        printf(" ( ");
    if ((second_letter & 0x80) == 0 && isprint(second_letter))
        printf("%c", second_letter);
    else
        printf(" ");
    if ((third_letter & 0x80) == 0 && isprint(third_letter))
        printf("%c)\n", third_letter);
    else
        printf(" )\n");
    printf("\n");
}
