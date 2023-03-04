#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//------------------------------------------First Exercise------------------------------------------
//Function 1: Function that converts a float to a binary string (return type an array of characters)
char* float_to_binary(float value) {  
    //Here we first check if the value is positive or negative, this will determine the value of the sign bit 
    char* sign;
    if(value < 0) {sign = "1";}
    else if (value >= 0) {sign = "0";}

    //Here we split the float into the integer section and the decimal section. 
    //So if it is 203.5, then the integer will be 203 and the decimal would be 0.5
    if(value < 0) {value *= -1;}
    int integerSect = (int)value;
    float decimalSect = value - integerSect;

    //Here we allocate 64 bits for the Mantissa also known as 'M'.
    //Hence you could do 32 bits for Integer and Decimal and 2 bits for the '.' and '\0'
    char* M = (char*)malloc(66*sizeof(char));

    //Here we convert the integer section of the float to pure binary. 
    //Hence, if it's % 2 == 0, then we append '0', else we append '1'
    int i=0;
    while(integerSect > 0) {
        M[i] = (integerSect % 2) + '0';
        integerSect /= 2;
        i++;
    }

    //When appending, it places the binary in the reverse order, hence we simply reverse the string this manner:
    int len = strlen(M);
    int j;
    for (j = 0; j < len / 2; j++) {
        char temp = M[j];
        M[j] = M[len - j - 1];
        M[len - j - 1] = temp;
    }

    //Here we hadd the "decimal point" so then we can work on converting the decimal section to pure binary.
    //I also added a decimal_indicator index which would be helpful for calculating the value of E (exponent).
    M[i] = '.';
    int decimal_indicator = i;
    i++;

    //Here we convert the decimal section of the float to pure binary.
    //Hence, if it's *2 >= 1, then we append '1', else we append '0'
    while(decimalSect > 0 && i < 64) {
        decimalSect *= 2;
        if(decimalSect >= 1) {
            M[i] = '1';
            decimalSect -= 1;
        }
        else {
            M[i] = '0';
        }
        i++;
    }

    //This is just a check if the value of the float is 0, then we append 0 into the Mantissa.
    //This is because 0 doesn't work with any of the above conditions for calculating the Mantissa. 
    if (i == 0) { 
        M[i] = '0';
        i++;
    }
    M[i] = '\0';  //Adding the terminating character into the string.

    //Here we calculate the value of E, which is the Exponent value needed to move the decimal place up.
    int E = 0;
    //We start at the beginning of the binary representation. If it is '1', then we calculate the value of E based on the decimal indicator.
    for (i = 0; i < decimal_indicator; i++) {
        if (M[i] == '1') {
            E = decimal_indicator - (i+1);
            break;
        }
    }
    
    //Since we have values of M and E, we can now represent the float in its IEEE754 Binary.
    //Hence 'frac' would be the encoding of M and 'exp' would be the encoding of E.
    char* frac = (char*)malloc(24*sizeof(char));  //Allocating 24 bits for frac (23bits + '\0')
    char str_temp[strlen(M)-decimal_indicator];   //This is a temp string which contains all the bits after the decimal place

    //Here I copy the bits from the "imaginary decimal point" till the decimal point into frac
    strncpy(frac, &M[decimal_indicator-E], decimal_indicator-1);        
    //Here I copy all the bits to the right of the decimal place into the temp string variable
    strncpy(str_temp, &M[decimal_indicator+1], strlen(M)-decimal_indicator);
    //Then I concatenate the temp string into frac
    strcat(frac, str_temp);

    //Here I add the remining extra '0' bits to the end of frac.
    int extra_fracbits = 23-strlen(frac);
    for(j=0; j < extra_fracbits; j++) {
        strcat(frac, "0");
    }
    frac[23] = '\0';
    
    //Then I calculate the value of exp. Since it is 32 bits, the bias is 127 hence I add that with E to get exp
    int exp = E+127;
    
    //I have to represent exp as pure binary hence the same steps are used to store the binary representation of exp into exp_binary 
    char exp_binary[9];
    i=0;
    while(exp > 0) {
        exp_binary[i] = (exp % 2) + '0';
        exp /= 2;
        if(exp == 0) {exp_binary[++i] = '0';}
        i++;
    }

    len = 8;
    for (j = 0; j < len / 2; j++) {
        char temp = exp_binary[j];
        exp_binary[j] = exp_binary[len - j - 1];
        exp_binary[len - j - 1] = temp;
    }
    exp_binary[8] = '\0';

    //Here I allocate 33 bits for a string float_binary which will be the final float binary representation.
    //I allocate 33 bits for the 32 bit representation and the '\0' character.
    //Then I simply concatenate all the different components: sign, exp, frac and return the value.
    char* float_binary = (char*)malloc(33*sizeof(char));
    strcat(float_binary, sign); 
    strcat(float_binary, exp_binary);
    strcat(float_binary, frac);
    float_binary[32] = '\0';

    return float_binary;
}

//Function 2: Function that switches the significand and exponent positions in string representation of float
char* switch_significand_exponent_string(char* value) {
    //I extract the sign bit through the value's first character.
    char sign = value[0];

    //I extract the exponent from value[1] for 8 characters as the exp is 8 bits
    char exp[9];
    strncpy(exp, value+1,8);
    exp[8] = '\0';

    //I extract the significand from value[9] for 23 characters as the significand is 23 bits
    char frac[24];
    strncpy(frac,value+9,23);
    frac[23] = '\0';

    //I allocate 33 bits for the switched floated binary and once more concatenate all the different components. 
    //I first add the sign bit, then the significand and then the exponent as we are switching the positions.
    char* switched_float_binary = (char*)malloc(33*sizeof(char));
    switched_float_binary[0] = sign;
    strncpy(switched_float_binary+1, frac, 23);
    switched_float_binary[24] = '\0';
    strcat(switched_float_binary, exp);

    return switched_float_binary;
}

//Function 3: Expand the significand to 52 bits and the exponent to 11 bits
char* expand_switched_significand_exponent(char* switched_value) {
    //I allocate 65 bits for the expanded binary: 64 bits + '\0'
    char* expanded_binary = (char*)malloc(65*sizeof(char));

    //The sign bit is still the first character.
    char sign = switched_value[0];

    //I extract the frac which is the the first 23 characters after switched_value[1].
    char frac_32[24];
    strncpy(frac_32, switched_value+1, 23);
    frac_32[23] = '\0';

    //I extract the exp which is the last 9 characters of the switched_value.
    char exp_32[9];
    strncpy(exp_32, switched_value+24, 8);
    exp_32[8] = '\0';

    //I then convert the string of the exponent to an integer and calulcate the exponent_binary encode for 64 bits. 
    //This is done by finding the previous value of E by subtracting the previous bias '127' and adding the new bias '1023'
    int E_32 = strtol(exp_32, NULL, 2);
    int E_64 = 1023 + (E_32 - 127);

    //I then represent the exponent encoding as pure binary for 64 bits using the same method shown previously.
    char exp_64[12];
    int i=0;
    while(E_64 > 0) {
        exp_64[i] = (E_64 % 2) + '0';
        E_64 /= 2;
        i++;
    }

    int len = 11;
    int j;
    for (j = 0; j < len / 2; j++) {
        char temp = exp_64[j];
        exp_64[j] = exp_64[len - j - 1];
        exp_64[len - j - 1] = temp;
    }
    
    //Then I concatenate everything (sign, significand, exponent) together once more into the expanded_binary variable. 
    expanded_binary[0] = sign;
    strcat(expanded_binary, frac_32);

    //Here I add 29 more '0' characters as we are expanding the significand to 52 bits, and 52-23 = 29.
    for(int i=0; i<29; i++) {
        strcat(expanded_binary, "0");
    }

    strcat(expanded_binary, exp_64);

    return expanded_binary;
}

//------------------------------------------Second Exercise------------------------------------------
//Function 1: Print the value of a variable in binary
void print_binary(unsigned int  value) {
    //This essentially shifts each bit of the binary representation to the first bit and compares it with 1 as that is just ...0001
    //The AND operator would check if that particular bit is a '1' or a '0', hence overall printing the value's binary
    for (int i = 31; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

//Function 2: Function that extracts the significand from float using bit operations
unsigned int get_significand(float value) {
    //This casts the value into its unsigned integer and compares it with 0x7fffff with the AND operator. 
    //This is because 0x7fffff is 1111111... for 23 bits. This will make all exponent and sign bit values '0' except the signficand
    unsigned int float_bits = *(unsigned int*)&value;
    unsigned int significand = float_bits & 0x7fffff;
    return significand;
}

//Function 3: Function that extracts the exponent from float using bit operations
unsigned int get_exponent(float value) {
    //This casts the value into its unsigned integer.
    unsigned int float_bits = *(unsigned int*)&value;

    //Then I shift the float 23 bits to the right, menaing only the exponent bits is on the far right side
    //Then I compare it with 0xff with the AND operator. This is because 0xff is 11111111 for 8 bits. 
    //This will make everything else except the exponent '0'.
    unsigned int exponent = (float_bits >> 23) & 0xff;

    //Then I shift the exponent back 23 spaces because that is the space allocated for the exponent. 
    exponent <<= 23;
    return exponent; 
}

//Function 4: Function that extracts the sign from float using bit operations
unsigned int get_sign(float value) {
    //This casts the value into its unsigned integer.
    unsigned int float_bits = *(unsigned int*)&value;
    //I shift the float 31 bits to the right so the only thing to the right is the sign bit.
    //I compare it with 1 through the AND operator as if it is negative it will be 1, else if it's positive, it will be 0
    unsigned int sign = (float_bits >> 31) & 1;
    sign <<= 31;
    return sign;    
}

//Function 5: Function that switches the significand and exponent positions in float using bit operations using previous functions
unsigned int switch_significand_exponent(float value) {
    //Here I use the previous functions to retreive all the binary formats for each of the components.
    unsigned int sign = get_sign(value);
    unsigned int exponent = get_exponent(value);
    unsigned int significand = get_significand(value);

    //I shift the significand 8 spaces to the left and the exponent 23 spaces to the right as we are switching their positions
    significand <<= 8;
    exponent >>= 23;

    //Then I use the OR operator to 'concatenate' essentially all of the binary formats together. 
    unsigned int switched_binary = sign | significand | exponent;
    return switched_binary;
}




