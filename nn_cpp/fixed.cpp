/**
 * Fixed Point Arithmetic Class - fixed.cpp
 *
 * For simulated 16-bit hardware
 *
 * @author: Andrew Elliott
 * email: ae3@pdx.edu
 *
 *
 * Revision Date:
 *
 * 0.1a 5/17/2018        // ae3 - First Release
 * 0.2b 5/19/2018        // Re-write with "double hosting" and C++11 Stuff
 *
 *
 * NOTE: Future version build with template<Type> to change internal maximum bit sizes.
 */


#include "fixed.h"



/********** Accessor Functions **********/



/**
 * Get Sign Bit
 * @return u16 sign bit
 */
u8 fixed::getSign() {
    return num_sign & 0x1;
}



/**
 * Get Integer Value
 * @return u16 integer value
 */
u16 fixed::getInt() {

    return num_int;
}



/**
 * Get fracional Component
 * @return u16 fracional value
 */
u16 fixed::getFrac() {

    return num_frac;
}



/**
 * Returns double value of number
 * @return Result as Double
 */
double fixed::getDouble(){
    return num_double;
}



/**
 * Set Int Comp.
 * @param input
 */
void fixed::setInt(u16 input){
    u8 ovfl;
    num_int = doDisposal(input, size_int, ovfl);    // Make input fit.
    toDouble();

    // Discard Overflow - Saturation or Truncate will handle this.

}



/**
 * Update fractional component
 * @param input
 */
void fixed::setFrac(u16 input){

    u8 ovfl;
    num_frac = doDisposal(input, maxSize(fracSize()), ovfl);    // Make input fit.
    toDouble();

    if(ovfl){
        auto temp = num_double + 1;
        fromDouble(temp);
    }
}



/**
 * Convert a double into a FP structure:
 * NOT A 'SIMULATION' FUNCTION - REQUIRED FOR PARSING TYPES
 * @param input
 */
void fixed::fromDouble(double input){

    double temp_d = 0, tmp_dsplit = 0, _fraction = input;
    int _int;
    u16 maxFrac, temp_i = 0;

    u8 ovfl = 0;        // Rounding Overflow
    u8 zero_cnt = 0;    // # of leading zeros

    // Get Sign:
    if(_fraction < 0){
        _fraction = _fraction * -1;

        if(_signed){
            num_sign = 1;
        }
    }

    // Split up double:
    _fraction = modf(_fraction , &temp_d);
    _int = (u16) temp_d;

    // Check if Zero:
    if(_fraction > 0) {

        // Count Leading Zeros:
        temp_d = _fraction * 10;
        for(;;){

            // Check if value in ones place:
            if((u16) temp_d > 0){
                break;
            }

            // Catch Double-Round Error
            if((float) temp_d == 1){
                break;
            }

            temp_d = temp_d * 10;
            zero_cnt++;
        }

        // Calculate maximum fractional value:
        num_zeros = zero_cnt;
        maxFrac = maxSize( fracSize() );

        // Convert Fractional Comp:
        _fraction = _fraction * pow(10, zero_cnt);

        for (;;) {

            // Split and check:
            tmp_dsplit = modf(_fraction , &temp_d);

            // Break when fraction == 0
            if( tmp_dsplit == 0 || temp_d >= maxFrac ){
                break;
            }

            // Catch double cast error (== 0.1 * 10):
            if((float) tmp_dsplit == 1 && temp_d == 0){
                num_frac = 1;
                break;
            }

            _fraction = _fraction * 10;
            num_frac = (u16) temp_d;
        }

        // Round Result (if required)
        if(mode & LSB_ROUND && _fraction >= 5){
            num_frac = 1;
            if(num_zeros){
                num_zeros --;
            }
        }


    }
    else{
        num_frac = 0;   // Result was zer
        maxFrac = maxSize( fracSize() );
    }

    // Save Fractional Comp.
    num_frac = doDisposal(num_frac, maxFrac, ovfl);

    // Get Integer Comp (with overflow from decimal conversion)
    num_int = doDisposal( (u32) _int + ovfl, max_int, ovfl);

    // Update Internal Double
    toDouble();
}



/**
 * 16-bit Adder
 * Result is added to the called operator
 * @param fp_input 
 */
void fixed::add16(fixed& fp_input){

    // Add Fractional Components
    auto temp = this->getDouble();
    auto tmp_in = fp_input.getDouble();

    temp = temp + tmp_in;   // Perform on reduced doubles
    this->fromDouble(temp); // Store Result
};



/**
 * Subtract 16-bit (from this value)
 * @param fp_input Class Object
 */
void fixed::sub16(fixed& fp_input){

    // Get Doubles
    auto temp = this->getDouble();
    auto tmp_in = fp_input.getDouble();

    temp = temp - tmp_in;   // Perform on reduced doubles
    this->fromDouble(temp); // Store Result
};



/**
 * 32 Multiplication (Accumulator)
 * @param fp_input 
 */
void fixed::mul32(fixed& fp_input){

    // Get Doubles
    auto temp = this->getDouble();
    auto tmp_in = fp_input.getDouble();

    temp = temp * tmp_in;   // Perform on reduced doubles
    this->fromDouble(temp); // Store Result
}



/**
 * 32-bit Division
 * @param fp_input 
 */
void fixed::div32(fixed& fp_input){

    // Get Doubles
    auto temp = this->getDouble();
    auto tmp_in = fp_input.getDouble();

    temp = temp / tmp_in;   // Perform on reduced doubles
    this->fromDouble(temp); // Store Result
}



/********** Operator Overloads **********/


/**
 * iostreme overload
 * @param os
 * @param fp_input
 * @return
 */
ostream& operator<<(ostream& os, fixed &fp_input){
    os << fp_input.getDouble();
    return os;
}



/**
 * Addition Operator Overload
 * @param fp_input
 */
void fixed::operator +=(fixed& fp_input){
    return add16(fp_input);
}



/**
 * Subtraction Operator Overload
 * @param fp_input
 */
void fixed::operator -=(fixed& fp_input){
    return sub16(fp_input);
}



/**
 * Increment (++) operator Overload
 */
void fixed::operator ++(int){

    auto temp = this->num_double;
    temp ++;
    fromDouble(temp);
}



/**
 * Decrement Operator
 */
void fixed::operator--(int){

    auto temp = this->num_double;
    temp --;
    fromDouble(temp);
}



/**
 * Multiply Operator
 * @param fp_input
 */
void fixed::operator*=(fixed &fp_input){
    return mul32(fp_input);
}



/**
 * Division Operator
 * @param fp_input
 */
void fixed::operator/=(fixed &fp_input){
    return div32(fp_input);
}



/**
 * Assignment operator (=) Overload
 * @param fp_input
 */
fixed& fixed::operator =(fixed& fp_input){

    // Check for self before copy:
    if(&fp_input != this){
        this->fromDouble(fp_input.getDouble());
        return *this;
    }

    return *this;
}



/********** Private Functions - Bit Disposal **********/



/**
 * Bit Disposal Routine for UNSIGNED INTEGERS ONLY
 * @param input Input Value
 * @param size Size of result
 * @param mode Mode Bits
 * @return
 */
u16 fixed::doDisposal(u32 input, u16 size, u8 &ovfl){

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
        temp = (u16) input;
    }

    // Do bit disposal
    if(mode & LSB_ROUND){
        return lsbRounding(temp, size, ovfl);
    }

    if(mode & LSB_JAMMING){
        return lsbJamming(temp, size);
    }

    return lsbTruncate(temp, size);

}



/**
 * Truncate LSB
 * @param input
 * @return result
 */
u16 fixed::lsbTruncate(u16 input, u16 size){

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
u16 fixed::lsbJamming(u16 input, u16 size){

    return lsbTruncate(input, size) | 0x0001;
}



/**
 * Round an Integer Component
 * @param input
 * @return
 */
u16 fixed::lsbRounding(u16 input, u16 size, u8 &ovfl){

    u32 temp = lsbTruncate(input, size);

    if(temp % 2){
        temp = input + 1;
        if(temp > size){
            ovfl = 1;
        }
    }

    return lsbTruncate(input, size);
}



/**
 * Count Zero Buffer and Adjust bits:
 */
u8 fixed::bitCount(u16 input) {

    auto tmp = input;
    u8 cnt = 0;
    if(num_zeros > 0){
        for(;;){
            if(tmp == 0){
                break;
            }

            tmp = tmp >> 1;
            cnt++;
        }
    }

    return cnt;
}




/**
 * Get the Maximum Value of a bit width
 * @param num_bits
 * @return
 */
u16 fixed::maxSize(u8 num_bits){
    return (u16) pow(2, num_bits) - 1;
}


/**
 * Perform (internal) double conversion from values.
 * @return
 */
void fixed::toDouble(){

    // Convert fracional component to double:
    double frac = num_frac;
    while( (int)frac != 0 ){
        frac /= 10;
    }

    // Add-in zeros
    for(int i = 0; i < num_zeros; i++){
        frac /= 10;
    }

    // Compute Sign:
    num_double = (double) (num_sign) ? -1*(frac+num_int) : frac+num_int;
}



//#endif