/**
 * Fixed Point Arithmetic Class - fixedPoint.cpp
 *
 * For simulated 16-bit hardware
 *
 * @author: Andrew Elliott
 * email: ae3@pdx.edu
 *
 *
 * Revision Date:
 *
 * 0.1 5/18/2018        // ae3 - First Release
 *
 *
 * NOTE: Future version build with template<Type> to change internal maximum bit sizes.
 */


#include "fixedPoint.h"


using namespace std;


/**********  Construct & Destruct **********/


/**
 * Default Constructor
 */
fixedPoint::fixedPoint() {

    mode = DEF_MODE ;        // Bit disposal option
    size_int = DEF_INT_SZE;             // Size (in bits) of integer component
    size_frac = DEF_FRC_SZE;            // Size (in bits) of fracional component

    num_sign = 1;           // Sign bit (two's complement)
    num_int = 0;           // Integer Component
    num_frac = 0;          // fracional Component


    max_int = pow(2, size_int) - 1;            // Maximum Value of int
    max_frac = pow(2, size_frac) - 1;          // Maximum Value of fracion
}

//#ifdef DISABLED

/**
 * Constructor
 * @param sign  Sign Bit
 * @param integer_size Size of integer component in bits
 * @param size_frac  Size of fracional component in bits
 * @param disposal_flags - Choose options for disposal
 */
fixedPoint::fixedPoint(u8 sign, u8 integer_size, u8 fraction_size,  u8 disposal_flags) {

    assert(integer_size + fraction_size <= BIT_WIDTH);

    mode = disposal_flags ;     // Bit disposal option
    size_int = integer_size;            // Size (in bits) of integer component
    size_frac = fraction_size;            // Size (in bits) of fracional component

    num_sign = sign;                    // Sign bit (two's complement)
    num_int = 0;                        // Integer Component
    num_frac = 0;                       // fracional Component

    max_int = (u16) pow(2, size_int) - 1;                // Maximum Value of int
    max_frac = (u16) pow(2, size_frac) - 1;             // Maximum Value of fracion

}



/********** Accessor Functions **********/



/**
 * Get Sign Bit
 * @return u16 sign bit
 */
u8 fixedPoint::getSign() {
    return num_sign & 0x1;
}



/**
 * Get Integer Value
 * @return u16 integer value
 */
u16 fixedPoint::getInt() {

    return num_int;
}



/**
 * Get fracional Component
 * @return u16 fracional value
 */
u16 fixedPoint::getFrac() {

    return num_frac;
}



/**
 * Set Sign Bit
 * @param sign 
 */
void fixedPoint::setSign(u8 sign){
    num_sign = sign & 0x1;
}



/**
 * Set Int Value
 * @param input 
 */
void fixedPoint::setInt(u16 input){
    num_int = input;
}



/**
 * Set fracional Component
 * @param input 
 */
void fixedPoint::setFrac(u16 input){
    num_frac = input;
}



/**
 * Convert a Fixed-point type to floating point
 * @tparam T Class Type
 * @param fp_in Class Type Input (Fixed-point)
 * @return Result as Double
 */
double fixedPoint::asDouble(){

    // Convert fracional component to double:
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

    auto temp_double = input;
    auto temp_int = (int) input;
    u8 ovfl = 0;

    // Get Sign:
    if(temp_int < 0){
        num_sign = 1;
        temp_int = temp_int * -1;  // Make positive
        temp_double = temp_double * -1;
    }
    else{
        num_sign = 0;
    }
    
    // Get fractional Comp.
    temp_double = temp_double - temp_int;  // Subtract int. comp

    if(temp_double > 0) {
        for (;;) {

            if (temp_double > max_frac) {
                break;
            }

            num_frac = (u16) temp_double;
            temp_double = temp_double * 10;
        }
    }
    else{
        num_frac = 0;
    }
    num_frac = doDisposal(num_frac, max_frac, ovfl);

    // Get Integer Comp.
    num_int = doDisposal( (u32) temp_int + ovfl, max_int, ovfl);

}



/**
 * 16-bit Adder
 * Result is added to the called operator
 * @param fp_input 
 */
void fixedPoint::add16(fixedPoint& fp_input){

    // Vars:
    u32 temp_sum = 0;
    u8 overflow = 0;

    // Check For Subtraction
    if( (fp_input.getSign() && !num_sign) || (!fp_input.getSign() && num_sign) ){
        return sub16(fp_input);             // Subtraction
    }
    
    // Add Fractional Components
    auto temp = this->asDouble();
    auto tmp_in = fp_input.asDouble();

    cout << "This: " << temp << "\tThat: " << tmp_in << endl;

    temp = temp + tmp_in;
    cout << "result: " << temp << endl;



    this->fromDouble(temp);
};

/**
 * Addition Operator Overload
 * @param fp_input
 */
void fixedPoint::operator +(fixedPoint& fp_input){
    return add16(fp_input);
}

/**
 * Increment (++) operator Overload
 * @param fp_input
 */
void fixedPoint::operator ++(){
    u8 overflow;
    num_int = doDisposal(num_int + 1, max_int, overflow);
}



/**
 * Subtract 16-bit (from this value)
 * @param fp_input Class Object
 */
void fixedPoint::sub16(fixedPoint& fp_input){

};



/**
 * 32 Multiplication (Accumulator)
 * @param fp_input 
 */
void fixedPoint::mul32(fixedPoint& fp_input){

}



/**
 * 32-bit Division
 * @param fp_input 
 */
void fixedPoint::div32(fixedPoint& fp_input){

}




/********** Accessory Functions **********/




/**
    * Format and print a fixed-point number
    * @tparam T
    * @param in_a
    */
void fixedPoint::print(){

    int temp = num_int;
    if(num_sign){
        temp *= -1;
    }
    cout << temp << "." << num_frac << endl;
}



/********** Private Functions - Bit Disposal **********/



/**
 * Bit Disposal Routine for UNSIGNED INTEGERS ONLY
 * @param input Input Value
 * @param size Size of result
 * @param mode Mode Bits
 * @return
 */
u16 fixedPoint::doDisposal(u32 input, u16 size, u8 &ovfl){

    u16 temp;

    // Check Saturation:
    if(input > size){
        // Saturate
        if(mode & BIT_SATURATE){
            temp = MAX_INT16;
        }
        else{
            for(;;){
                // Shift right for bit reduction (16-bit)
                input = input >> 1U;
                if(input <= MAX_INT16){
                    temp = (u16) input;
                    break;
                }
            }
        }
    }
    else{
        // Truncate Upper 16 bits
        return (u16) input;
    }

    // Do bit disposal
    if(mode & LSB_ROUND){
        return lsbRounding(temp, size, ovfl);
    }
    else if(mode & LSB_JAMMING){
        return lsbJamming(temp, size);
    }
    else{
        return lsbTruncate(temp, size);
    }
}



/**
 * Truncate LSB
 * @param input
 * @return result
 */
u16 fixedPoint::lsbTruncate(u16 input, u16 size){

    u16 result = input;

    while(result > size){
        result = result >> 1U;
    }

    return result;
}



/**
 * Truncate and Jam LSB
 * @param input
 * @return
 */
u16 fixedPoint::lsbJamming(u16 input, u16 size){

    return lsbTruncate(input, size) | 0x0001;
}



/**
 * Round an Integer Component
 * @param input
 * @return
 */
u16 fixedPoint::lsbRounding(u16 input, u16 size, u8 &ovfl){

    u32 temp = lsbTruncate(input, size);

    if(temp % 2){
        temp = input + 1;
        if(temp > size){
            ovfl = 1;
        }

        //temp = temp >> 1;
    }

    return lsbTruncate(input, size);
}

//#endif