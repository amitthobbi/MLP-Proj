//
// Created by shmandy on 5/18/18.
//

#include "fixedPoint.h"

using namespace std;


/**
 * Default Constructor
 */
fixedPoint::fixedPoint(void) {

    bit_disposal = BIT_TRUCATE ;        // Bit disposal option
    size_int = DEF_INT_SZE;             // Size (in bits) of integer component
    size_fract = DEF_FRC_SZE;            // Size (in bits) of fractional component

    num_sign = 1;           // Sign bit (two's complement)
    num_int = 0;           // Integer Component
    num_frac = 0;          // Fractional Component


    max_int = pow(2, size_int) - 1;            // Maximum Value of int
    max_fract = pow(2, size_fract) - 1;          // Maximum Value of fraction
}



/**
 * Constructor
 * @param sign  Sign Bit
 * @param integer_size Size of integer component in bits
 * @param size_fract  Size of fractional component in bits
 * @param disposal_flags - Choose options for disposal
 */
fixedPoint::fixedPoint(u8 sign, u16 integer_size, u16 size_fract, u8 disposal_flags) {

    bit_disposal = disposal_flags ;     // Bit disposal option
    size_int = integer_size;            // Size (in bits) of integer component
    size_fract = size_fract;            // Size (in bits) of fractional component

    num_sign = sign;                    // Sign bit (two's complement)
    num_int = 0;                        // Integer Component
    num_frac = 0;                       // Fractional Component

    max_int = pow(2, size_int) - 1;                 // Maximum Value of int
    max_fract = pow(2, size_fract) - 1;             // Maximum Value of fraction

}



/**
 * Destructor (NOT USED - NO DYNAMIC MEMORY)
 */
fixedPoint::~fixedPoint() {

    /* Clean up any dynamic memory here */

}

/**
    * Format and print a fixed-point number
    * @tparam T
    * @param in_a
    */
void fixedPoint::print(void){

    int temp = num_int;
    if(num_sign){
        temp *= -1;
    }
    std::cout << temp << "." << num_frac << std::endl;
}



/**
 * Get Sign Bit
 * @return u16 sign bit
 */
u16 fixedPoint::getSign(void) {
    return num_sign & 0x1;
}



/**
 * Get Integer Value
 * @return u16 integer value
 */
u16 fixedPoint::getInt(void) {

    return num_int;
}



/**
 * Get Fractional Component
 * @return u16 fractional value
 */
u16 fixedPoint::getFract(void) {

    return num_frac;
}



/**
 * Convert a Fixed-point type to floating point
 * @tparam T Class Type
 * @param fp_in Class Type Input (Fixed-point)
 * @return Result as Double
 */
double fixedPoint::asDouble(){

    // Convert fractional component to double:
    double frac = num_frac;
    while( (int)frac != 0 ){
        frac /= 10;
    }

    // Compute Sign:
    return (num_sign) ? -1*(frac+num_int) : frac+num_int;
}



/**
 * Convert a double into a FP structure:
 * NOT A 'SIMULATION' FUNCTION - REQUIRED FOR PARSING TYPES
 * @param input
 */
void fixedPoint::fromDouble(double input){

    double temp;

    // Get Sign:
    if(input < 0){
        num_sign = 1;
        temp = input * -1;  // Make positive
    }

    // Get Integer Comp.
    num_int = doDisposal((u32) temp, size_int, bit_disposal);   
    
    // Get Fractional Comp.
    temp = temp - num_int;  // Subtract int. comp

    for(;;){

        if(temp > max_fract){
            break;
        }

        num_frac = (u16) temp;
        temp = temp * 10;
    }
}

// 16-bit Adder
void fixedPoint::add16(const fixedPoint& fp_input){

    // Check sign bits
    //if(fp_input)

};



// 16-bit Subtract
template <class U>
void fixedPoint::sub16(const U& fp_input){

};



// 32-bit Multiply-accumulator
template <class U>
void fixedPoint::mul32(const U& fp_input){

}



// 32-bit divider
template <class U>
void fixedPoint::div32(const U& fp_input){

}


/********** Private Functions **********/



/**
 * Bit Disposal Routine
 * @param input Input Value
 * @param size Size of result
 * @param mode Mode Bits
 * @return
 */
u16 doDisposal(u32 input, u8 size, u8 mode){
    
    if(mode & LSB_ROUND){
        return lsbRounding(input, size);
    }
    else if(mode & LSB_JAMMING){
        return lsbJamming(input, size);
    }
    else{
        return lsbTruncate(input, size);
    }
}

/**
 * Truncate LSB
 * @param input
 * @return result
 */
u16 fixedPoint::lsbTruncate(u16 input, u8 size){

    u16 result = input;

    while(result > size){
        result = result >> 1;
    }
}


/**
 * Truncate and Jam LSB
 * @param input
 * @return
 */
u16 fixedPoint::lsbJamming(u16 input, u8 size){

    return lsbTruncate(input, size) | 1;
}



/**
 *
 * @param input
 * @return
 */
u16 fixedPoint::lsbRounding(u16 input, u8 size){

    u16 temp = lsbTruncate(input, size);

    if(temp & 0x1){
        return input & 0xFFFE;
    }

    return temp;

}


/**
 * Check bounds of incoming data and provide the saturated value
 * @param input
 * @return
 */
u16 fixedPoint::resSaturate(int input, u8 size){

    if( 0 <= input ){
        
        if( MAX_INT16 < input){
            return input;
        }
        
        return MAX_INT16;
        
    }

    return input;
}