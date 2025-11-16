#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For isspace


///////////////////////////////  declare variables //////////////////////


#define MAX_COLS 100           // Maximum number of columns expected
#define MAX_COL_NAME_LEN 300   // Maximum length of a column name
#define SELECTION_SIZE 100     // Maximum number of features that can be selected
#define MAX_ROW_LENGTH 250000  // Maximum expected length of a single row
#define FILE_NAME_LEN 100      // Maximum expected length file name


typedef struct {
    float feature_values[SELECTION_SIZE]; // Values of the selected features
    float label_value;                    // Value of the selected label
} DataRow;


char file_name[FILE_NAME_LEN];// Global variable for file name


// Global variables to store column names
char all_column_names[MAX_COLS][MAX_COL_NAME_LEN];
int num_columns = 0;


char selected_features[SELECTION_SIZE][MAX_COL_NAME_LEN]; //kepp name of features to select as x     max_row_lenght ?
char selected_label[MAX_COL_NAME_LEN];                    //keep naem of label to sclect as y
int feature_n = 0;                       // Counter for the number of selected features


// variables for data storage and indices
int row_num = 0;      // Total number of rows
DataRow *all_data = NULL;  // Pointer to hold the array of DataRow (Actual data)
int selected_feature_indices[SELECTION_SIZE]; // 0-based index of selected features
int selected_label_index = -1;                 // 0-based index of the selected label


/////////////////////////////// functions ////////////////////////////////


float test (float a) {
    for (int i = 0 ; i < 10 ; i ++) {
        printf("test");
    }

    return 0;
}







int main() {


int adjust_times = 1; // have to get user input first -----> do it later

/////////////// read file, show to user and make user select (feture,row,label) ////////// output --> features,label,features_num ////////

float losses[adjust_times] ;

int layers = 1 ; //get from user input

float (*functions_pointer[layers])(float) ;
float (*dfunction_pointer[layers])(float) ;
float (*lfunction_pointer[layers])(float) ;

functions_pointer[0] = &test; //loop trought user input
dfunction_pointer[0] = &test;
lfunction_pointer[0] = &test;

// printf("%d",functions_pointer[0]());


///////////// train test split ///////////





////////////// main program /////////////




int dimention[1] = {1}; // set of nodes inputed from user   [3,4,1]
float features_train[1][1] = {{1}}; //get from csv's data   
float weights[1][1] = {{1}}; //from random
float bias[1][1] = {{1}}; //from random

float sum ; //declare here is fine
int z_size = 0; //for set z_value-array size 
int number_of_node = 0 ; // for set 

for (int layer_num = 0 ; layer_num < layers ; layer_num ++ ) {

    
    if (layer_num == 0) {
        z_size += feature_n*dimention[layer_num] ;
    }else if (layer_num+1 == layers) {
        z_size += selected_label_index * dimention[layer_num] ;
    }else {
        z_size += dimention[layer_num-1] * dimention[layer_num] ;
    }

    number_of_node += dimention[layer_num] ;

}

number_of_node += output_num ;


float dzdw_array[z_size] ;
float activationfunction_outputs[number_of_node];

float weight_adjust_record[1][1][1] = {{{0}}}; //fix dimention
float bias_adjust_record[1][1][1] = {{{0}}}; //fix dimention

int z_lastlayer_index = -1 ;

for (int adjust_time_count = 0 ; adjust_time_count < adjust_times ; adjust_time_count ++) {

    for (int row = 0 ; row < row_num ; row ++ ) {

        int z_index = 0; //for tracking index easily

        for (int layer_num = 0 ; layer_num < layers ; layer_num ++ ) {

            for (int node_num = 0 ; node_num < dimention[layer_num] ; node_num++){
                sum = 0;
                if (layer_num == 0) {
                    for (int previous_node = 0 ; previous_node < feature_n ; previous_node++ ) {

                        
                        sum += all_data[row].feature_values[previous_node] * weights[layer_num][node_num*2+previous_node] ;
                        dzdw_array[z_index++] = all_data[row].feature_values[previous_node] ;
                        
                    }
                    
                }else{

                    for (int previous_node = 0 ; previous_node < dimention[layer_num-1]; previous_node ++ ) {
                    
                        
                        sum += activationfunction_outputs[previous_node] * weights[layer_num][node_num*2+previous_node] ;
                        dzdw_array[z_index++] = activationfunction_outputs[z_lastlayer_index+previous_node] ;
                    }
                }
            
                sum += bias[layer_num][node_num] ;
                sum = (*functions_pointer[layer_num])(sum) ;
                activationfunction_outputs[node_num] = sum ;
                
            }

        }

        for (int outputnode_num = 0 ; outputnode_num < output_num ; outputnode_num ++) {
            sum = 0 ;
            for (int lasthiddennode_num = 0 ; lasthiddennode_num < dimention[sizeof(dimention)/sizeof(int)-1]; lasthiddennode_num++){
                sum += activationfunction_outputs[lasthiddennode_num] * weights[lastlayer][node];
                dzdw_array[last] = activationfunction_outputs[lasthiddennode_num]
            }

            sum += bias[lastlayer][outputnode_num] ;
            sum = (*functions_pointer[outputnode_num])(sum);
            activationfunction_outputs[outputnode_num] = sum ;
        }

        output = activationfunction_outputs[lastlayer];

        losses[adjust_time_count] = (*lfunction_pointer)(output);

        float dzdw,dzdz,dldlast_z,dvaluedz ;

        for (int layer_num = layers-1 ; layer_num >= 0 ; layer_num --) {
            for (int furter_node = dimention[layer_num]-1 ; furter_node >= 0 ; furter_node --) {
                for (int closer_node = dimention[layer_num-1]-1 ; closer_node >= 0 ; closer_node--) {

                    dldlast_z = 

                    dvaluedz = (*dfunction_pointer[layer_num])(activationfunction_outputs[z_index]);

                    dzdw = dzdw_array[z_index--] ;
                    
                    dldz = dldlast_z * d

                    keep[layer_num][furter_node][closer_node] = dzdz * keep[w_layer+1][furter_node][closer_node] ;
                }
            }
        }


    }

    weights[][][] += weight_adjust_record[][][];
    bias[][][] += bias_adjust_record[][][];

}






///////////// output ////////////////////

return 0;



}


