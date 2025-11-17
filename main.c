#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For isspace


///////////////////////////////  declare variables //////////////////////




typedef struct {
    float feature_values[100]; // Values of the selected features
    float label_value;                    // Value of the selected label
} DataRow;








int feature_n = 1;                       // Counter for the number of selected features


// variables for data storage and indices
int row_num = 1;      // Total number of rows
DataRow *all_data = NULL;  // Pointer to hold the array of DataRow (Actual data)

int selected_label_index = -1;                 // 0-based index of the selected label


/////////////////////////////// functions ////////////////////////////////


float test (float a) {
    
   // printf("activated a function\n");
    

    return 0;
}







int main() {


printf("main program start\n");

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

float output_num = 1 ;

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


int z_lastlayer_index = -1 ;

float *output ;
output = malloc(output_num*sizeof(float)) ;

float learning_rate = 0.01 ;

for (int adjust_time_count = 0 ; adjust_time_count < adjust_times ; adjust_time_count ++) {

    printf("loop number : %d\n",adjust_time_count);

    float weight_adjust_record[1][1] = {{1}}; //fix dimention//////////////////////////////////////////////
    float bias_adjust_record[1][1] = {{0}}; //fix dimention////////////////////////////////////////////////

    printf("there is %d rows of data\n\n",row_num);

    for (int row = 0 ; row < row_num ; row ++ ) {

        printf("in loop %d, run trought row : %d\n",adjust_time_count,row);

        int z_index = 0; //for tracking index easily

        for (int layer_num = 0 ; layer_num < layers ; layer_num ++ ) {

            for (int node_num = 0 ; node_num < dimention[layer_num] ; node_num++){
                sum = 0;
                if (layer_num == 0) {
                    for (int previous_node = 0 ; previous_node < feature_n ; previous_node++ ) {

                        
                        sum += all_data[row].feature_values[previous_node] * weights[layer_num][node_num*2+previous_node] ; //segmentation false ///////////////////
                        
                        dzdw_array[z_index++] = all_data[row].feature_values[previous_node] ;
                        
                    }
                    
                }else{

                    for (int previous_node = 0 ; previous_node < dimention[layer_num-1]; previous_node ++ ) {
                    
                        
                        sum += activationfunction_outputs[previous_node] * weights[layer_num][node_num*2+previous_node] ;//////////////////////////////////////////////////////
                        dzdw_array[z_index++] = activationfunction_outputs[z_lastlayer_index+previous_node] ;////////////////////////////////////////////////////////////////
                    }
                }
            
                sum += bias[layer_num][node_num] ; /////////////////////////////////////////////////////////////////////
                sum = (*functions_pointer[layer_num])(sum) ;
                activationfunction_outputs[node_num] = sum ; ////////////////////////////////////////////////////////////

                printf("output in this node : %f\n",sum);
                
            }

            printf("finished one layer\n");

        }

        printf("finished forward propagation\n");

        for (int outputnode_num = 0 ; outputnode_num < output_num ; outputnode_num ++) { ///////////////////////////////////////////////
            sum = 0 ;
            for (int lasthiddennode_num = 0 ; lasthiddennode_num < dimention[sizeof(dimention)/sizeof(int)-1]; lasthiddennode_num++){
                sum += activationfunction_outputs[lasthiddennode_num] * weights[layers][outputnode_num*2+lasthiddennode_num]; //////////////////////////////////////////////////
                dzdw_array[z_index++] = activationfunction_outputs[lasthiddennode_num]; ////////////////////////////////////////////////////////////////////////////
            }

            sum += bias[layers][outputnode_num] ; /////////////////////////////////////////////////////////////
            sum = (*functions_pointer[outputnode_num])(sum);///////////////////////////////////////////////////
            activationfunction_outputs[outputnode_num] = sum ;///////////////////////////////////////
        }

        printf("finish answering\n");

        *output = activationfunction_outputs[layers]; ///////////////////////////////////////////////////

        printf("output saved\n");

///--------------------- form here, activationfunction_output will keep dl/dz value ----------------------

        losses[adjust_time_count] = (*lfunction_pointer)(*output); ////////////////////////////////////////

        float dzdw,dzdz,dzdb,dldlast_z,dvaluedz,dlast_zdvalue,dldz ;

        for (int layer_num = layers-1 ; layer_num >= 0 ; layer_num --) { ///////////////////////////////////////
            for (int closer_node = dimention[layer_num-1]-1 ; closer_node >= 0 ; closer_node--) { /////////////////////////////////////////

                if (closer_node == dimention[layer_num-1]-1) {
                    for (int outputnode_num = output_num ; outputnode_num >= 0 ; outputnode_num--) {

                    }

                }else{

                    for (int furter_node = dimention[layer_num]-1 ; furter_node >= 0 ; furter_node --) {

                        dldlast_z = activationfunction_outputs[furter_node] ; ///////////////////////////////////////////////////////////

                        //dlast_zdvalue = weights[layer_num][furter_node*2+closer_node] ; //segmentation false
                        dlast_zdvalue = 1 ;

                        //dvaluedz = (*dfunction_pointer[layer_num])(activationfunction_outputs[z_index]);/////////Segmentation fault//////////////////////////////////
                        dvaluedz = (*dfunction_pointer[layer_num])(0);

                        //dzdw = dzdw_array[z_index--] ; /////segmentation false////////////////////////////////////////////////////////////////
                        dzdw = 1 ;
                        
                        dldz = dldlast_z * dlast_zdvalue * dvaluedz ;

                        //activationfunction_outputs[closer_node] = dldz ; //segmentation false ///////////////////////
                        activationfunction_outputs[0] = dldz ; 

                        //weight_adjust_record[layer_num][furter_node*2+closer_node] += (dldz * dzdw)/row_num ; //segmentation false
                        weight_adjust_record[0][0] += (dldz * dzdw)/row_num ;
                        //bias_adjust_record[layer_num][closer_node] += (dldz)/row_num ; //segmentation false
                        bias_adjust_record[0][0] += (dldz)/row_num ;
                    }
                }
                
            }
        }

    }

    for (int layer_num = 0 ; layer_num < layers ; layer_num++) { //////////////////////////////////
        for (int next_node = 0 ; next_node < dimention[layer_num+1] ; next_node++) {
            for (int previous_node = 0 ; previous_node < dimention[layer_num] ; previous_node++) {////////////////////////////////////////////
                weights[layer_num][previous_node*2+next_node] -= weight_adjust_record[layer_num][previous_node*1+next_node] * learning_rate ;
            }

            bias[layer_num][next_node] -= bias_adjust_record[layer_num][next_node] * learning_rate ;

        }
            
    }

}

printf("\nprinting model \n\n");

for (int layer_num = 0 ; layer_num < layers ; layer_num++) { 
    //printf("model in layer : %d",layer_num);
    for (int next_node = 0 ; next_node < dimention[layer_num] ; next_node++) {
        //printf("node : %d",next_node);
        if (layer_num == 0) {
            for (int previous_node = 0 ; previous_node < feature_n ; previous_node++) {
                printf("weight : %f\n",weights[layer_num][previous_node*2+next_node]);
            }
        }else {
            for (int previous_node = 0 ; previous_node < dimention[layer_num-1] ; previous_node++) {
                printf("weight : %f\n",weights[layer_num][previous_node*2+next_node]);
            }
        }
        

        printf("bias : %f\n",bias[layer_num][next_node]);

    }
            
}


printf("end of program....");
///////////// output ////////////////////

return 0;



}


