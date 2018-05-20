/**
 * Fixed Point Arithmetic Class (header file) - fixed.h
 *
 * For simulated 16-bit hardware
 *
 * Author: Andrew Elliott
 * email: ae3@pdx.edu
 *
 * Revision Date:
 *
 * 0.1 5/18/2018
 *
 */

#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using std::ostream;

//#ifndef CPP_DEV_fixed_H
//#define CPP_DEV_fixed_H


// Constraints:
#define BIT_WIDTH 16U
#define MAX_INT16 0xFFFFu
#define MIN_INT16 0x0000u


// Defaults:
#define DEF_INT_SZE 3U       // Int size = 2 bits (0-3)
#define DEF_SIGN    1U       // Unsigned Value
#define DEF_MODE    LSB_TRUCATE | BIT_SATURATE


// Type Shortcuts:
#define u32 unsigned int
#define u16 unsigned short int
#define u8 unsigned char

// Parameters:
#define LSB_TRUCATE     0x0U     // Truncate LSB
#define LSB_JAMMING     0x1U     // LSB Always 1
#define LSB_ROUND       0x2U     // Round to Even
#define BIT_SATURATE    0x4U     // Saturate Value on OVF/UNF

// Macros
#define fracSize() (bit_width - (size_int + num_zeros + _signed))


/**
 * Fixed Point Class
 * @tparam T
 */

class fixed {

    public:

        // Constructor:
        fixed(u8 _signed, u8 integer_size, u8 truncation_mode) {

            bit_width = 16;                     // 16-bit Constructor Call (move to template)

            // Validate
            assert(integer_size + _signed <= bit_width);

            mode = truncation_mode ;            // Bit disposal option
            size_int = integer_size;            // Size (in bits) of integer component
            this->_signed = _signed & 0x1U;     // Size (in bits) of fractional component

            num_sign = 0;                       // Sign bit (two's complement)
            num_int = 0;                        // Integer Component
            num_frac = 0;                       // fractionalComponentn
            num_double = 0;                     // Double Value
            num_zeros = 0;

            max_int = maxSize(integer_size);    // Get Max Value
            max_frac = maxSize(bit_width - (integer_size + _signed));    // Get Max Value
        }   // Constructor

        // Defaults delegate
        fixed() : fixed(DEF_SIGN, DEF_INT_SZE, DEF_MODE) {}

        // Getters & Setters
        u8 getSign();                           // Get the Sign
        u16 getInt();                           // Get the Int
        u16 getFrac();                          // Get the fraction
        double getDouble();                     // Return as double

        void setInt(u16 input);                 // Update Int comp.
        void setFrac(u16 input);                // Update Frac comp.

        // Conversion
        void fromDouble(double input);          // Convert from double

        // Math Operations
        void add16(fixed &fp_input);           // 16-bit Sum
        void sub16(fixed &fp_input);           // 16-bit Subtract
        void mul32(fixed &fp_input);           // 32-bit Multiply
        void div32(fixed &fp_input);           // 32-bit division

        // Overloaded Operators
        void operator+=(fixed &fp_input);       // Sum (operator)
        void operator++(int);                      // Increment (operator)
        void operator-=(fixed &fp_input);       // Subract
        void operator--(int);                      // Decrement
        void operator*=(fixed &fp_input);       // Multiply
        void operator/=(fixed &fp_input);       // Divide

        fixed& operator=(fixed& fp_input);      // Assignment Operator (copy into 'this' config)

        friend ostream& operator<<(ostream& os, fixed &fp_input);       // STD::COUT operator

    private:

        // Simulated Hardware Constraints
        u8 mode;                // Precision reduction algorithm selection
        u8 bit_width;           // Width of simulated fixed-point value (in bits)
        u8 size_int;            // Size (in bits) of integer component
        u8 _signed;             // Signed or Unsigned Representation

        // Limits
        u16 max_int;            // Maximum (uint) value for int component
        u16 max_frac;

        // Numerical Values:
        u8 num_sign;            // Sign bit (two's complement)
        u8 num_zeros;           // Leading zeros in fractional component
        u16 num_int;            // Integer Component
        u16 num_frac;           // fractional Component
        double num_double;      // Double Value (rounded / truncated to spec)

        // Private Functions
        u16 doDisposal(u32 input, u16 size, u8 &ovfl);              // Perform Disposal Functions
        u16 lsbTruncate(u16 input, u16 size);                       // Truncate
        u16 lsbJamming(u16 input, u16 size);                        // Jam
        u16 lsbRounding(u16 input, u16 size, u8 &ovfl);             // Rounding

        u8 bitCount(u16 zeros);
        u16 maxSize(u8 num_bits);
        void toDouble();

};


//#endif //CPP_DEV_fixed_H
