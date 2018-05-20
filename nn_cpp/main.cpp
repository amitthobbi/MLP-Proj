/*
 *  Fixed-point MLP
 *
 * Authors:
 * Andrew Elliott, Amit
 *
 *
 *
 *  References:
 *
 *  csv.h reader: https://github.com/ben-strasser/fast-cpp-csv-parser by Ben Strasser
 *  https://stackoverflow.com/questions/18726102/what-difference-between-rand-and-random-functions
 */

//#include "mlp.h"
#include "csv.h"        /* CSV Parsint lib */
#include "fixed.h" /* FixedPoint Object Class */

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <fstream>
#include <random>
#include <iomanip>

using std::cout;
using std::cin;
using std::endl;



/****** MLP CONFIGURATION **********/

// Input File Names
#define SIGMOID_LUT "sigmoid_lut_0_8_16bit.csv"       // Single Column File
#define IRIS_DATA   "iris_data_nonames.csv"           // 5 Column file (sepal_l, sepal_w, petal_l, petal_w, label)


#define LUT_SIZE 256        // Size of LUT
#define LUT_MAX 8           // Maximum X input

#define DATASET_SIZE 150    // Total dataset size
#define TEST_SIZE   50      // Size of test batch
#define TRAIN_SIZE  100     // Size of train batch


#define INPUT_FEATURES 4    // Number of features *THIS IS HARDCODED IN MAIN*
#define OUTPUT_CLASSES 3    // Total number of output nodes / classes
#define HIDDEN_LAYER_CNT       2        // 2 Hidden Layers
#define HIDDEN_LAYER_SIZE      16       // 1:4 Inputs to Hidden Neurons


#define LEARN_RATE 0.01     // Learning Rate
#define MOMENTUM 0.01       // Momentum


/*
 * Labels:
 * 0 = Iris Setosa
 * 1 = Iris Versicolour
 * 2 = Iris Virginica
*/

/***** FUNCTION PROTOTYPES ****/
void sigmoid(fixed& x, fixed& y, u16 (&lut)[LUT_SIZE]);         // Sigmoid Lookup table with interpolation


// Test Functions (for fixed lib)
void sigmoidLutTest( u16 (&logistic_lut)[LUT_SIZE]);            // Test the lib and sigmoid lut
void showFixedPointTest();                                      // Fixed Point Lib Test



/****** MAIN LOOP *******/

int main() {


    // Check Settings
    assert( (TEST_SIZE + TRAIN_SIZE) == DATASET_SIZE );

    // MLP Data Sets:
    u16 logistic_lut[LUT_SIZE];                         // Logistic LUT

    //fixed train_data[TRAIN_SIZE][INPUT_FEATURES];  // Train Data
    //fixed test_data[TEST_SIZE][INPUT_FEATURES];    // Test Data

    //u16 train_labels[TRAIN_SIZE];                       // Label Data
    //u16 test_labels[TRAIN_SIZE];                        // Label Data

    // Temp Vars:
    u32 index_t = 0, test_i = 0, train_i = 0;           // List-building index
    float sepal_l, sepal_w, petal_l, petal_w;           // Temps for CSV extraction
    u16 label;                                          // Temp for CSV extraction


    // Setup Randomness:
    std::random_device r;                                           // Device
    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};     // Non-deterministic Seeds
    std::mt19937 eng{seed};                                         // Mersenne Twister 19937 generator
    std::uniform_int_distribution<> dist(1,10);                     // Constrain to range


    // Load Sigmoid Function:
    io::LineReader lut_in(SIGMOID_LUT);
    while(char*line = lut_in.next_line()){
        logistic_lut[index_t] = (u16) std::stoi(line, nullptr, 16);
        index_t++;

        // Check Bounds
        if(index_t >= LUT_SIZE){
            break;
        }
    }

    // Perform LUT Test:
    sigmoidLutTest(logistic_lut);







    /*
    // Load Iris Data Set:
    // Shuffle and split up data using random distribution:
    io::CSVReader<5> iris_in(IRIS_DATA);
    iris_in.read_header(io::ignore_extra_column, "sepal_l", "sepal_w", "petal_l", "petal_w", "label");
    while(iris_in.read_row(sepal_l, sepal_w, petal_l, petal_w, label)) {

        // DEBUG: Print formatted data set:
        cout << std::fixed << std::setprecision(2) << "S_L: " << sepal_l << "\tS_W: " << sepal_w << "\tP_L: " << petal_l << "\tP_W: " << "\tLabel: " << label << endl;

        // Add to test set:
        if(  (dist(eng) % 2 && test_i < TEST_SIZE) || train_i >= TRAIN_SIZE){
            test_data[test_i][0] = sepal_l;
            test_data[test_i][1] = sepal_w;
            test_data[test_i][2] = petal_l;
            test_data[test_i][3] = petal_w;
            test_labels[test_i] = label;
            test_i ++;
        }
        // Add to training set:
        else{
            train_data[train_i][0] = sepal_l;
            train_data[train_i][1] = sepal_w;
            train_data[train_i][2] = petal_l;
            train_data[train_i][3] = petal_w;
            train_labels[train_i] = label;
            train_i ++;
        }
    }


    // Training Loop
    for(int train_index = 0; train_index < TRAIN_SIZE; train_index++){

        // Perform forward propagation:

        // Perform Back Propagation

    }


    // Testing Loop
    for(int test_index = 0; test_index < TEST_SIZE; test_index++){

        // Perform forward propagation & Record Error
        // Display error for each round + collect
    }


    // Print Results
        */

    // Show Test:
    //showFixedPointTest();


    return 0;
}


/**
 * Sigmoid Lut:
 * Input is S3.12
 * Output is U.16
 * @param input as double (math performed in full precision)
 * @param y - result passed by ref. (could not get assignment operator to work)
 * @return NONE
 */
void sigmoid(fixed& x, fixed& y, u16 (&lut)[LUT_SIZE]) {
    //assume this is global
    //int logistic_lut[index_t];

    // Convert input x to index_t
    // 8/256 ~= 0.03137 <-> 8/0.03137 ~= 255
    // x=8 will return 255, 0 will return 0

    double input_x = x.getDouble();
    double input_x_pos = input_x;
    double step_size = (double) LUT_MAX / LUT_SIZE;
    u8 index;

    // Linear Interpolation
    double y2, y1;
    double x1, x2;
    double temp = 0;

    // Check Sign
    if( input_x < 0 ){
        input_x_pos = input_x * -1;
    }

    // Calculate Array Index:
    index = (u8) (input_x_pos / step_size);   // Truncate into index

    // Check bounds of X
    if( 0 == index ){
        y2 = lut[index+1];
        y1 = lut[index];

        x2 = step_size;
        x1 = 0;

    }
    else if ( (LUT_SIZE -1) == index){
        y2 = lut[index];
        y1 = lut[index-1];

        x2 = step_size * (index);
        x1 = step_size * (index - 1);
    }
    else{
        y2 = lut[index+1];
        y1 = lut[index-1];

        x2 = step_size * (index + 1);
        x1 = step_size * (index - 1);
    }

    if( input_x < 0 ){
        temp = (input_x_pos - x1) * ((y2-y1)/(x2-x1)) + y1;  // Y = mx + b, where m = slope of the line near x
        decimate(temp) // Convert to double
        temp = 1 - temp;

    }
    else{
        temp = (input_x - x1) * ((y2-y1)/(x2-x1)) + y1;  // Y = mx + b, where m = slope of the line near x
        decimate(temp) // Convert to double:
    }


    // Debugging:
    //cout << "\nX_input: " << input_x << "\tstep size: " << step_size  << endl;
    //cout << "Y1: " << y1 << "\tY2: " << y2 << "\tX1: " << x1 << "\tX2: " << x2 << endl;
    //cout << "Sigmoid Result: " << temp << endl;


    // Check Bounds:
    if (input_x >= 8) {
        y.fromDouble(1.0); // Not reachable with S3.12
        return;
    }
    else if (input_x <= -8){
        y.fromDouble(0.0);
        return;
    }

    y.fromDouble(temp);

}



/**
 * Show the fixed point testing
 */
void showFixedPointTest(){

    // Create a few 'fixed' vars:
    fixed A( 0, 4, LSB_TRUCATE | BIT_SATURATE);
    fixed B( 1, 8, LSB_TRUCATE);
    fixed C( 0, 10, LSB_TRUCATE);
    fixed D( 1, 3, LSB_ROUND | BIT_SATURATE);   // Example Output type for a LUT

    // Convert from static double:
    A.fromDouble(13.01);
    B.fromDouble(-23.01);
    C.fromDouble(32.0009);

    //cout << "C: " << C << endl;

    // Print:
    cout << "Inputs\tA: " << A << "\t"  << "B: " << B << "\t" << "C: " << C << endl;
    A += B;
    cout << "Addition: A += B" << endl;
    cout << "A = " << A << "\tB = " << B << endl;

    // Reset A and add to B
    A.fromDouble(13.001);
    B +=A;
    cout << "A: " << A << endl;
    cout << "Addition: B += A" << endl;
    cout << "A = " << A << "\tB = " << B << endl;

    // Mul Test
    C*=B;
    cout << "Mul. C *= B, C = " << C << endl;

    // Div Test
    C/=B;
    cout << "Div. C /= B, C = " << C << endl;

    // Decrement
    C--;
    cout << "Decrement. C-- = " << C << endl;

    // Increment
    C++;
    cout << "Increment. C++ = " << C << endl;

}



/**
 * Test Algorithm for Sigmoid LUT
 * and fixed library.
 */
void sigmoidLutTest( u16 (&logistic_lut)[LUT_SIZE]){

    // Sigmoid - Test with Fixed Point Values:
    fixed Y( 0, 0, LSB_ROUND);      // Lut Results
    fixed X( 1, 3, BIT_SATURATE);   // Example Output type for a LUT

    double step_size = (double) LUT_MAX/LUT_SIZE;
    double test_index = -7;

    // -7 to 0
    for(;;){
        X.fromDouble(test_index);
        sigmoid(X, Y, logistic_lut);
        cout << std::fixed << std::setprecision(6) << "Index: " << test_index << "\tX = " << X <<  "\tf(x) = " << Y << "\n" << endl;
        test_index += step_size;
        if(test_index >= 0) {
            break;
        }
    }

    // 0 to 7
    test_index = 0;
    for(;;){
        X.fromDouble(test_index);
        sigmoid(X, Y, logistic_lut);
        cout << std::fixed << std::setprecision(6) << "Index: " << test_index << "\tX = " << X <<  "\tf(x) = " << Y << "\n" << endl;
        test_index += step_size;
        if(test_index >= 7) {
            break;
        }
    }
}