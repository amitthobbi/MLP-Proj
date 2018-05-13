//============================================================================
// Name        : lab3_1.cpp
// Author      : Amit,Andy
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


/*
 * This file implements a neural network with the specififed layers.
 * This is used to train and test the neural network
 * it returns the accuracy of the algorithm.
 * Created By Amit Thobbi,Vivan Bhalla
 */


#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Definitions for limiting number of dataset samples,number of features,output classes
#define MAX_TRAIN_SAMPLES 120
#define MAX_TEST_SAMPLES  30
#define NUM_FEATURES      4
#define NUM_CLASSES       3

// Definitions for the number of of nodes in layers in the network
#define INPUT_LAYERS  NUM_FEATURES
#define HIDDEN_LAYERS 8
#define OUTPUT_LAYERS 3

// Definitions for number of epochs
#define EPOCHS 20

// Variable for training
#define OP_TARGET 0.95
#define ETA       0.2
#define MOMENTUM  0.9

//Train and Test Array
int train_arr[MAX_TRAIN_SAMPLES][NUM_FEATURES];
int test_arr[MAX_TEST_SAMPLES][NUM_FEATURES];


/******************* VARIABLE DECLERATIONS *******************/

double weights_ih [INPUT_LAYERS+1][HIDDEN_LAYERS]={0.1}; // Weight matrix from inputs to hidden layers
double weights_ho [HIDDEN_LAYERS][OUTPUT_LAYERS]={0.1}; // Weight matrix from hidden to output layers
double old_weights_ih_delta [INPUT_LAYERS+1][HIDDEN_LAYERS]={0.0}; // Weight matrix to store the old weights changed
double old_weights_ho_delta [HIDDEN_LAYERS+1][OUTPUT_LAYERS]={0.0}; // Weight matrix to store the old hidden to output weights change
double bias_old_weights_ho [OUTPUT_LAYERS] = {0.0}; // Matrix to store the old delta values for the bias
double bias_old_weights_ih [HIDDEN_LAYERS] = {0.0};
double hidden_activations [HIDDEN_LAYERS]; // MAtrix to store the hidden activations calculated
double output_activations [OUTPUT_LAYERS]; // Matrix to store the output activations
double target [OUTPUT_LAYERS] = {0.1}; // Target matrix
double error_output[OUTPUT_LAYERS]={0.0}; // matrix to store the output layers errors
double error_hidden[HIDDEN_LAYERS]={0.0}; // Matrix to store the hidden layer errors
double correct_predictions = 0; // Variable to store the number of correct predictions for accuracy
double hidden_activations_test [HIDDEN_LAYERS] ={0.0}; // Variable to store the hidden activations for the test data
double output_activations_test [OUTPUT_LAYERS] ={0.0};

/****************** FUNCTION DECLARATIONS **********************/

int readcsv();
double sigmoid(double x); // Function to calculate the sigmoid of a variable x
double der_sigmoid(double y); // Function to calculate the derivative of sigmoid
double gen_rand(double min, double max); // Function to generate random number sin range
void train(int input[],int target); // Function to train the network
void initialize(); // Function to initialize the network
void test(int input_test[], int target); // Test the data
void calc_accuracy(); // Function to calculate the accuracy

double get_weights();

/****************** MAIN PROGRAM ***********/

int main(){

	setvbuf(stdout, NULL, _IONBF, 0);

	//load test and train arrays
	readcsv();

	//initialize initial weights
    initialize();

    int i,k =0;
	correct_predictions = 0;

    //Train for some epochs
    for(k=0;k<EPOCHS;k++){
        printf("\n epoch: %d",k);

        for(i=0;i<MAX_TRAIN_SAMPLES;i++)
            train(train_arr[i],train_arr[i][0]);

    }

    //Test the results
    for(i=0; i<MAX_TEST_SAMPLES;i++)
        test(test_arr[i],test_arr[i][0]);

 printf("\nCorrect Predictions are %f",correct_predictions);

 get_weights();


return 0;
}

/************** FUNCTION DEFINITIONS ****************/


int readcsv()
{

       char buffer[1000];
       char *record,*line;

       int i=0,j=0;
       FILE *fstream = fopen("iris_train2.csv","r");
       if(fstream == NULL)
       {
          printf("\n file opening failed ");
          return -1 ;
       }

       while((line=fgets(buffer,sizeof(buffer),fstream))!=NULL)
       {
         record = strtok(line,",");
         while(record != NULL)
         {
         train_arr[i][j++] = strtol(record,NULL,10) ;
         record = strtok(NULL,",");
         }
         ++i ;
         j=0;
         if(i > MAX_TRAIN_SAMPLES) break;
       }



       i=0,j=0;
       FILE *fstream1 = fopen("iris_test2.csv","r");
       if(fstream1 == NULL)
       {
         printf("\n file opening failed ");
         return -1 ;
       }

       while((line=fgets(buffer,sizeof(buffer),fstream1))!=NULL)
       {
          record = strtok(line,",");
          while(record != NULL)
          {
            test_arr[i][j++] = strtol(record,NULL,10) ;
            record = strtok(NULL,",");
           }
             ++i;
             j=0;
             if(i>MAX_TEST_SAMPLES) break;
        }

return 0;

}

double sigmoid(double x){

    double e = 1/exp(x);
    // Return the calculated sigmoid value
    return 1.0 / (1.0 + e);
}

double der_sigmoid(double y)
{
    return y * (1.0 - y);
}

void initialize(){

	int i=0;
    int k =0;

    // Initialize the input to hidden layer weight matrix with random numbers
    for( i = 0; i < INPUT_LAYERS+1;i++){
        for( k = 0; k < HIDDEN_LAYERS; k++){
            // Store the matrix with random numbers in range -0.05 to 0.05
            weights_ih[i][k] = gen_rand(-0.05,0.05);
        }
    }

    // Initialize the hidden to output layer weights matrix in range -0.05 to 0.05
        for( i = 0; i < HIDDEN_LAYERS;i++){
            for( k = 0; k < OUTPUT_LAYERS; k++){
                // Initialize the target matrix once to 0 and also initalize the error matrices
                error_output[k] = 0;
                error_hidden[i] = 0;

                // Store the matrix with random numbers in range -0.05 to 0.05
                weights_ho[i][k] = gen_rand(-0.05,0.05);
            }
        }

}

double gen_rand(double min, double max){

    double scale = rand() / (double) RAND_MAX;
    return min + scale *(max - min);
}

void train(int input[],int target_in){

	// The index is used to keep track of the weights matrix
	int i,k = 0;

    /*********************FEEDFORWARDING *******************************/
    // Feedforward the network to get the hidden activations

	// Iterate through the hidden layers
	for(k=0; k < HIDDEN_LAYERS;k++){
        // Get the bias for the specific layer
        double bias_ih =0;
        bias_ih = weights_ih[0][k];
        for(i=0; i < INPUT_LAYERS;i++){
            //printf("Weights_ih[%d][%d] is %f input is %d\n",i+1,k,weights_ih[i+1][k],input[i+1]);
            bias_ih += (input[i+1]/1.00)*weights_ih[i+1][k];
            }
        //printf("Bias output is %f\n",bias_ih);
        // Calculate the sigmoid of the bias and save it in the matrix
        hidden_activations[k] = sigmoid(bias_ih);
       //printf("HIDDEN ACTIVATION %d is %f\n",k,hidden_activations[k]);
    }

    // Feedforward the network to find the activations for output layers
    for(k=0; k < OUTPUT_LAYERS;k++){
        // Get the bias for the specific layer
        // printf("Bias is %f \n",weights_ho[0][k]);
        double bias_ho = 0;
        bias_ho = weights_ho[0][k];

        for(i=0; i < HIDDEN_LAYERS;i++){ // Iterate through the activations of hidden layers
            // printf("\nWeights_ho[%d][%d] is %f",i+1,k,weights_ho[i+1][k]);
            bias_ho += hidden_activations[i]*weights_ho[i+1][k];
            }

        // Calculate the sigmoid of the bias and save it in the matrix
        output_activations[k] = sigmoid(bias_ho);
        //printf("Output activation %d is %f \n",k,output_activations[k]);
        }

    /*********************************************************************/

    /**************** TARGET SETTING ************************************/

    // Set the target value to OP_TARGET for the specific target input. Rest all remain to 0

    target[target_in] = OP_TARGET;

    /*********************************************************************/

    /*************************** BACK PROPOGATION ************************/

    // Calculate the error terms for the output
    for(i=0; i < OUTPUT_LAYERS; i++){
        error_output[i] = der_sigmoid(output_activations[i]) * (target[i] - output_activations[i]);
        //printf("Error term for output % is %f\n",i,error_output[i]);
    }


    // Calculate the error for hidden units
    for(i =0; i < HIDDEN_LAYERS;i++){
        double temp =0;
        // Calculate the summation over weights and output error

        for(k=0; k < OUTPUT_LAYERS; k++){
            temp += error_output[k] * weights_ho[i+1][k];
            //printf("Error term calculation for hidden layers %d. Error output %f is. Weights_ho[%d][%d] is %f\n",i,error_output[k],i,k,weights_ho[i][k]);
        }

        //printf("temp is %f\n ",temp);
        // Calcualte the error term
        error_hidden[i] = der_sigmoid(hidden_activations[i]) * temp;
        //printf("Error term for hidden %d is %f\n",i,error_hidden[i]);
    }

    /*********************** UPDATE WEIGHTS ******************************/

    double delta_ho = 0; // Variable to store the weight change
    double bias_ho_delta =0;

    // Update the bias weights for the hidden to output
    for(i=0;i< OUTPUT_LAYERS; i++){
        bias_ho_delta = ETA * error_output[i] + MOMENTUM * bias_old_weights_ho[i];
        weights_ho[0][i] += bias_ho_delta;
        bias_old_weights_ho[i] = bias_ho_delta;
    }

    // Update weights for the hidden to output layer matrix
    for(i=0; i <HIDDEN_LAYERS;i++){
        for (k=0; k < OUTPUT_LAYERS; k++){
            delta_ho = (ETA * error_output[k] * hidden_activations[i]) + (MOMENTUM * old_weights_ho_delta[i+1][k]); // Calculate the value of delta
            weights_ho[i+1][k] += delta_ho; // Update the weights
            old_weights_ho_delta[i+1][k] = delta_ho; // Store the value of current delta to old delta
            //printf("\n new weight for hidden layer to op layer  weights_ho[%d] [%d]  is %f is ",(i+1),k,weights_ho[i+1][k]);

        }
    }

   // Update bias weights from input to hidden layers
   double bias_ih_delta = 0;
   for(i=0; i<HIDDEN_LAYERS;i++){
       bias_ih_delta = ETA * error_hidden[i] + MOMENTUM * bias_old_weights_ih[i];
       weights_ih[0][i] += bias_ih_delta;
       bias_old_weights_ih[i] = bias_ih_delta;
    }

    double delta_ih =0;
    // Update weights for the input to hidden layer matrix
    for (i=0; i < INPUT_LAYERS; i++){
        for(k=0; k < HIDDEN_LAYERS; k ++){
            delta_ih = (ETA * error_hidden[k] * (input[i+1]/1.00)) + (MOMENTUM * old_weights_ih_delta[i+1][k]);
            weights_ih[i+1][k] += delta_ih;
            old_weights_ih_delta[i+1][k] = delta_ih;
            //printf("\n new weight for ip layer to hidden layer  weights_ih[%d] [%d]  is %f is ",(i+1),k,weights_ho[i+1][k]);
        }
    }

    /*************************************************************************/

    // Reset the target layer
    for(i=0;i<OUTPUT_LAYERS;i++){
    target[i] = 1-OP_TARGET;
    }

}


void test(int input_test[], int target){

    int i, k =0;
    //printf("\n input test is %s ",*input_test);
    printf("\n\n target input is %d ",target);

     // Feedforward the network to get the hidden activations
    for(k=0; k < HIDDEN_LAYERS;k++){
        // Get the bias for the specific layer
        double bias_ih_test = 0;
        bias_ih_test = weights_ih[0][k];

        for(i=0; i < INPUT_LAYERS;i++){
            bias_ih_test += (input_test[i+1]/1.00)*weights_ih[i+1][k];
            }
        printf("\n bias ih %d is %f ",k,bias_ih_test);

        // Calculate the sigmoid of the bias and save it in the matrix
        hidden_activations_test[k] = sigmoid(bias_ih_test);
        printf("\n hidden activations is %f ",hidden_activations_test[k]);
    }

    // Feedforward the network to find the activations for output layers
    for(k=0; k < OUTPUT_LAYERS;k++){
            // Get the bias for the specific layer
            double bias_ho_test = weights_ho[0][k];
                for(i=0; i < HIDDEN_LAYERS;i++){
                bias_ho_test += hidden_activations_test[i]*weights_ho[i+1][k];
                }

            printf("\n bias ho %d is %f ",k,bias_ho_test);

            // Calculate the sigmoid of the bias and save it in the matrix
            output_activations_test[k] = sigmoid(bias_ho_test);
            printf("\n output activations is %f ",output_activations[k]);

        }


    // Find the position of the index with maximum activation

    double maximum = 0;
    maximum = output_activations_test[0]; //Initial Value

    int l = 0;
    int location = 0; // Variable to store the location of the index with max value

    for (l = 1; l < OUTPUT_LAYERS; l++)
        {
            if (output_activations_test[l] > maximum)
            {
                maximum  = output_activations_test[l];
                location = l;
            }
        }
    printf("\n predicted output is %d",location);
    // Check if the output is equal to target
    if(location == target){
        correct_predictions +=1;
    }

}

double get_weights()
{
    int i,k=0;

    printf("\n The weights ih is \n");

    for(i = 0; i < INPUT_LAYERS+1;i++){
        for( k = 0; k < HIDDEN_LAYERS; k++){
        	printf("%f ", weights_ih[i][k]);
        }
        printf("\n");
    }

    printf("\n The weights ho is \n");
    for(i = 0; i < HIDDEN_LAYERS;i++){
        for(k = 0; k < OUTPUT_LAYERS; k++){
            printf("%f ", weights_ho[i][k]);
        }
        printf("\n");
    }

}
