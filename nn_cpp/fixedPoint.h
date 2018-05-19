/**
 * Fixed-point Data Simulation Class
 *
 * All math functions are performed internally via passing a class pointer into
 * a function.
 *
 *
 *  Currently set for 16-bit data types
 *
 *  Future Work: Convert u16 to template for extensibility
 */

#ifndef CPP_DEV_FIXEDPOINT_H
#define CPP_DEV_FIXEDPOINT_H


// Constraints:
#define MAX_INT16 0xFFFF
#define MIN_INT16 0x0000

// Defaults:
#define DEF_INT_SZE 0     // Fixed point s.15
#define DEF_FRC_SZE 15
#define DEF_SIGN    1


// Type Shortcuts:
#define u32 unsigned int
#define u16 unsigned short int
#define u8 unsigned char

// Parameters:
#define LSB_TRUCATE     0x0     // Truncate LSB
#define LSB_JAMMING     0x1     // LSB Always 1
#define LSB_ROUND       0x2     // Round to Even
#define BIT_SATURATE    0x4     // Saturate Value on OVF/UNF

// Macros



/**
 * Fixed Point Class
 * @tparam T
 */

class fixedPoint {

    public:



        // Functions:
        fixedPoint(void);   // Default Constructor
        fixedPoint(u16 integer_size, u16 size_fract, u8 disposal_flag);   // Constructor
        ~fixedPoint(void);          // Destructor

        // Accessory
        void print(void);           // Print Contents
        double asDouble(void);      // Return as double
        void fromDouble(void);      // Convert from double

        // Accessor
        u8 getSign(void);       // Get the Sign
        u16 getInt(void);       // Get the Int
        u16 getFract(void);     // Get the fraction

        // 16-bit Adder
        void add16(const fixedPoint& fp_input);

        // 16-bit Subtract
        void sub16(const fixedPoint& fp_input);

        // 32-bit Multiply-accumulator
        void mul32(const fixedPoint& fp_input);

        // 32-bit divider
        void div32(const fixedPoint& fp_input);

    private:

        // Data Members
        u8 bit_disposal;        // Bit disposal option
        u8 size_int;            // Size (in bits) of integer component
        u8 fract_size;           // Size (in bits) of fractional component
        u16 max_int;            // Maximum Value of int
        u16 max_fract;          // Maximum Value of fraction

        // Object Values:
        u8 num_sign;            // Sign bit (two's complement)
        u16 num_int;            // Integer Component
        u16 num_frac;           // Fractional Component

        // Private Functions
        u16 lsbTruncate(u16 input, u16 size);           // Truncate
        u16 lsbJamming(u16 input, u16 size);            // Jam
        u16 lsbRounding(u16 input, u16 size);           // Round
        u16 resSaturate(u32 input, u16 size);           // Saturate Result
};


#endif //CPP_DEV_FIXEDPOINT_H
