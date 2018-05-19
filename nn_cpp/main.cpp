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
#include "fixedPoint.h" /* FixedPoint Object Class */
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <fstream>
#include <time.h>       /* time */
#include <random>
#include <iomanip>



/****** MLP CONFIGURATION **********/

// Input File Names
#define SIGMOID_LUT "sigmoid_lut_0_1.csv"       // Single Column File
#define IRIS_DATA   "iris_data_nonames.csv"     // 5 Column file (sepal_l, sepal_w, petal_l, petal_w, label)


#define LUT_SIZE 256        // Size of LUT

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



/****** MAIN LOOP *******/

int main() {

    // Check Settings
    static_assert( (TEST_SIZE + TRAIN_SIZE) == DATASET_SIZE );

    // MLP Data Sets:
    u16 logistic_lut[LUT_SIZE];                         // Logistic LUT

    fixedPoint train_data[TRAIN_SIZE][INPUT_FEATURES];  // Train Data
    fixedPoint test_data[TEST_SIZE][INPUT_FEATURES];    // Test Data

    u16 train_labels[TRAIN_SIZE];                       // Label Data
    u16 test_labels[TRAIN_SIZE];                        // Label Data

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


    // Load Iris Data Set:
    // Shuffle and split up data using random distribution:
    io::CSVReader<5> iris_in(IRIS_DATA);
    iris_in.read_header(io::ignore_extra_column, "sepal_l", "sepal_w", "petal_l", "petal_w", "label");
    while(iris_in.read_row(sepal_l, sepal_w, petal_l, petal_w, label)) {

        // DEBUG: Print formatted data set:
        cout << fixed << setprecision(2) << "S_L: " << sepal_l << "\tS_W: " << sepal_w << "\tP_L: " << petal_l << "\tP_W: " << "\tLabel: " << label << endl;

        // Add to test set:
        if(  (dist(eng) % 2 && test_i < TEST_SIZE) || train_i >= TRAIN_SIZE){
            test_data[test_i][0]. = sepal_l;
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

    // Debug


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




    return 0;
}