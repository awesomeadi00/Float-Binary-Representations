#include <stdio.h>
#include "functions.c"
//Tested Float values:
// 203.5
// 29.671875
// -13.90625
// -322.625
// 1024.068237
// -1.625

int main() {
    float test = -13.90625;

    printf("%s \n", "------------------------------------------------------------------------------------------");
    printf("Original value: %f\n", test);
    
    //Exercise 1: Float Binary String Manipulation:
    printf("%s \n", "------------------------------------EXERCISE 1--------------------------------------------");
    //Prints the binary representation of the original value 'test'
    printf("Original value in Binary Representation as a string: ");
    printf("%s \n", float_to_binary(test));

    //Switch the significand and exponent in string representation
    char* switch_pos = float_to_binary(test);
    printf("Switched significand and exponent positions: ");
    printf("%s \n", switch_significand_exponent_string(switch_pos));

    //Expands the switched string to 64 bits
    char* expand_binary = switch_significand_exponent_string(switch_pos);
    printf("Expanded 64 Float Binary: ");
    printf("%s \n", expand_switched_significand_exponent(expand_binary));

    //Exercise 2: Float Binary Bit Manipulation:
    printf("\n");
    printf("%s \n", "------------------------------------EXERCISE 2--------------------------------------------"); 
    printf("Original value in Binary Representation through bits: ");
    //For the helper function, to pass an unsigned int: print_binary(x)
    //To pass a float variable: print_binary(*(unsigned int*)&x);
    print_binary(*(unsigned int*)&test);

    //Retreiving all the integer values for each of the component's binary representation. To be used with 'print_binary'
    unsigned int significand = get_significand(test);
    unsigned int exponent = get_exponent(test);
    unsigned int sign = get_sign(test);
    unsigned int switched_binary = switch_significand_exponent(test);

    //This is to print out the new float value of the switched significand and exponent function:
    float float_of_switched_binary = *(float*)&switched_binary;

    //Extracted Significand in binary format:
    printf("Extracted Significand in Binary: ");
    print_binary(significand);

    //Prints the extracted significand as an integer:
    printf("Extracted Significand as Integer: ");
    printf("%u \n", get_significand(test));

    //Extracted Exponent in binary format:
    printf("Extracted Exponent in Binary: ");
    print_binary(exponent);

    //Extracted Sign-but in binary format:
    printf("Extracted Sign-bit in Binary: ");
    print_binary(sign);

    //Prints the binary of the switched significand and exponent format:
    printf("Switched Significand and Exponent in Binary: ");
    print_binary(switched_binary);

    //Prints the new float variable of the switched significand and exponent format:
    printf("Switched Binary as a Float: ");
    printf("%f \n", float_of_switched_binary);
    printf("%s \n", "------------------------------------------------------------------------------------------");
    return 0;
}

