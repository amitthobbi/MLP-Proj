/**
 * Fixed Point Arithmetic Class (header file) - fixedPoint.h
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

//#ifndef CPP_DEV_FIXEDPOINT_H
//#define CPP_DEV_FIXEDPOINT_H


// Constraints:
#define BIT_WIDTH 16U
#define MAX_INT16 0xFFFFu
#define MIN_INT16 0x0000u

// Defaults:
#define DEF_INT_SZE 0     // Fixed point s.15
#define DEF_FRC_SZE 15
#define DEF_SIGN    1
#define DEF_MODE    LSB_TRUCATE | BIT_SATURATE;


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



/**
 * Fixed Point Class
 * @tparam T
 */

class fixedPoint {

public:


    // Functions:
    fixedPoint();   // Default Constructor
    fixedPoint(u8 sign, u8 integer_size, u8 fraction_size, u8 disposal_flags);   // Constructor

    // Getters
    u8 getSign();           // Get the Sign
    u16 getInt();           // Get the Int
    u16 getFrac();          // Get the fraction

    // Setters
    void setSign(u8 sign);      // Set Sign Bit
    void setInt(u16 input);     // Set Integer Component
    void setFrac(u16 input);    // Set fraction Component

    // Accessory
    void print();                           // Print Contents
    double asDouble();                      // Return as double
    void fromDouble(double input);          // Convert from double

    // 16-bit Adder
    void add16(fixedPoint &fp_input);           // 16-bit Sum
    void operator+(fixedPoint &fp_input);       // 16-bit Sum (operator)
    void operator++();                          // 16-bit Incremenet (operator)

    // 16-bit Subtract
    void sub16(fixedPoint &fp_input);

    // 32-bit Multiply-accumulator
    void mul32(fixedPoint &fp_input);

    // 32-bit divider
    void div32(fixedPoint &fp_input);

private:

    // Data Members
    u8 mode;                // Bit disposal option
    u8 size_int;            // Size (in bits) of integer component
    u8 size_frac;           // Size (in bits) of fracional component
    u16 max_int;            // Maximum Value of int
    u16 max_frac;           // Maximum Value of fracion

    // Object Values:
    u8 num_sign;            // Sign bit (two's complement)
    u16 num_int;            // Integer Component
    u16 num_frac;           // fracional Component

    // Private Functions
    u16 doDisposal(u32 input, u16 size, u8 &ovfl);            // Perform Disposal Functions
    u16 lsbTruncate(u16 input, u16 size);           // Truncate
    u16 lsbJamming(u16 input, u16 size);            // Jam
    u16 lsbRounding(u16 input, u16 size, u8 &ovfl);        // Rounding

};


//#endif //CPP_DEV_FIXEDPOINT_H
