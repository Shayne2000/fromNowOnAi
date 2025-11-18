#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For isspace


///////////////////////////////  declare variables //////////////////////




typedef struct {
    float feature_values[100]; // Values of the selected features
    float label_value;                    // Value of the selected label
} DataRow;








int feature_n = 3;                       // Counter for the number of selected features


// variables for data storage and indices
int row_num = 5;      // Total number of rows
DataRow *all_data = NULL;  // Pointer to hold the array of DataRow (Actual data)

int selected_label_index = -1;                 // 0-based index of the selected label


/////////////////////////////// functions ////////////////////////////////


float test (float a) {
    
   // printf("activated a function\n");

    return a;
}
#define SELECTION_SIZE 100
#define MAX_COL_NAME_LEN 300

char selected_features[SELECTION_SIZE][MAX_COL_NAME_LEN];
char selected_label[MAX_COL_NAME_LEN];


int main() {

    strcpy(selected_features[0], "Age");          // เปลี่ยนได้เลย 
    strcpy(selected_features[1], "Salary"); 
    strcpy(selected_features[2], "Experience");  
    strcpy(selected_label, "PerfScore"); 


    all_data = malloc(row_num * sizeof(DataRow));
    if (!all_data) {
        printf("Memory alloc failed!\n");
        return 1;
    }


    float feature_samples[5][3] = {  // เปลี่ยนขนาดได้เลือกตามต้องการเลย แต่ต้องสอดคร้องกับ [row_num][feature_num]
        {25.0, 32000.0, 1.0},
        {30.0, 45000.0, 3.0},
        {22.0, 28000.0, 0.5},
        {40.0, 60000.0, 10.0},
        {35.0, 52000.0, 7.0}
    };

    float label_samples[5] = {3.5, 4.2, 3.0, 4.8, 4.5};

    for (int r = 0; r < row_num; r++) {
        for (int f = 0; f < feature_n; f++) {
            all_data[r].feature_values[f] = feature_samples[r][f];
        }
        all_data[r].label_value = label_samples[r];
    }


printf("main program start\n");

int adjust_times = 1; // have to get user input first -----> do it later

/////////////// read file, show to user and make user select (feture,row,label) ////////// output --> features,label,features_num ////////

float losses[adjust_times] ;

int layers = 2 ; //get from user input

float (*functions_pointer[layers])(float) ;
float (*dfunction_pointer[layers])(float) ;
float (*lfunction_pointer)(float) ;
float (*dlfunction_pointer)(float) ;
float (*outputfunctions_pointer)(float) ;
float (*doutputfunction_pointer)(float) ;


functions_pointer[0] = &test; //loop trought user input
functions_pointer[1] = &test;
dfunction_pointer[0] = &test;
dfunction_pointer[1] = &test;
lfunction_pointer = &test;
dlfunction_pointer = &test;
outputfunctions_pointer = &test;
doutputfunction_pointer = &test;

// printf("%d",functions_pointer[0]());

(*dfunction_pointer)(0) ;
///////////// train test split ///////////


float value ; // just keep things around


////////////// main program /////////////




const int dimention[2] = {1,1}; // set of nodes inputed from user   [3,4,1]
//float features_train[1][1] = {{1}}; //get from csv's data   
float weights[3][3] = {{1,1,1},{1},{1}}; //from random
float bias[3][1] = {{1},{1},{1}}; //from random

float sum ; //declare here is fine
int w_size = 0; //for set z_value-array size 
int number_of_node = 0 ; // for set size of array according to nodes

float output_num = 1 ;

for (int layer_num = 0 ; layer_num < layers ; layer_num ++ ) {
    
    if (layer_num == 0) {
        w_size += feature_n*dimention[layer_num] ;
    }else if (layer_num+1 == layers) {
        w_size += selected_label_index * dimention[layer_num] ;
    }else {
        w_size += dimention[layer_num-1] * dimention[layer_num] ;
    }

    number_of_node += dimention[layer_num] ;

}

number_of_node += output_num ;


float dzdw_array[w_size] ;
float z_keep [number_of_node] ;


int z_lastlayer_index = -1 ;

float *output ;
output = malloc(output_num*sizeof(float)) ;

float learning_rate = 0.01 ;

for (int adjust_time_count = 0 ; adjust_time_count < adjust_times ; adjust_time_count ++) {

    printf("loop number : %d\n",adjust_time_count);

    float weight_adjust_record[1][1] = {{1}}; //fix dimention//////////////////////////////////////////////
    float bias_adjust_record[1][1] = {{0}}; //fix dimention////////////////////////////////////////////////

    //printf("there is %d rows of data\n",row_num);
    printf("\n");

    for (int row = 0 ; row < row_num ; row ++ ) {

        printf("\n             run trought row : %d\n",row);

        int w_index = 0; //for tracking index easily
        int node_index = 0; // using with activationfunction_output
        int previouslayernode_sum = 0 ;//for tracking previous node during interaction between layers

        for (int layer_num = 0 ; layer_num < layers ; layer_num ++ ) {

            for (int node_num = 0 ; node_num < dimention[layer_num] ; node_num++){
                sum = 0;
                if (layer_num == 0) {
                    for (int previous_node = 0 ; previous_node < feature_n ; previous_node++ ) {

                        
                        sum += all_data[row].feature_values[previous_node] * weights[layer_num][node_num*2+previous_node] ;
                        printf("(%f * %f) + ",all_data[row].feature_values[previous_node],weights[layer_num][node_num*2+previous_node]);

                        dzdw_array[w_index++] = all_data[row].feature_values[previous_node] ;
                        
                    }
                    
                }else{

                    for (int previous_node = 0 ; previous_node < dimention[layer_num-1]; previous_node ++ ) {
                        
                        value = (*functions_pointer[previous_node])(z_keep[previouslayernode_sum + previous_node]) ; //last z

                        printf("(%f * %f) + ",value,weights[layer_num][node_num*2+previous_node]);
                        sum += value * weights[layer_num][node_num*2+previous_node] ;
                        dzdw_array[w_index++] = value ;
                    
                        //printf("using value of node %d",previouslayernode_sum+previous_node);
                    }
                }

                previouslayernode_sum += dimention[layer_num-1] ;
            
                sum += bias[layer_num][node_num] ; 
                printf("%f\n",bias[layer_num][node_num]);
                z_keep[node_index] = sum ;
                sum = (*functions_pointer[layer_num])(sum) ; 
                

                printf("output in this node : %f at index %d\n",sum,node_index);

                node_index++;
                
            }

            //printf("finished one layer\n");

        }

        printf("\n-----finished forward propagation--------\n");

        for (int outputnode_num = 0 ; outputnode_num < output_num ; outputnode_num ++) { 
            sum = 0 ;
            for (int lasthiddennode_num = 0 ; lasthiddennode_num < dimention[layers-1]; lasthiddennode_num++){

                value = (*functions_pointer[layers-1])(z_keep[node_index-dimention[layers-1]+outputnode_num]); //value of last hidden

                sum += value * weights[layers][outputnode_num*2+lasthiddennode_num]; 
                dzdw_array[w_index++] = value; ////////////////////////////////////////////////////////////////////////////
            }

            sum += bias[layers][outputnode_num] ; 
            z_keep[node_index+outputnode_num] = sum ;
            sum = (*outputfunctions_pointer)(sum);
            *(output+outputnode_num) = sum ;

            printf("output node%d's answer : %f\n",outputnode_num,sum);
        }

        printf("-------finish answering----------\n");


///--------------------- form here, z_keep will keep dl/dz value ----------------------

        losses[adjust_time_count] = (*lfunction_pointer)(*output); ////////////////////////////////////////

        float dzdw,dzdz,dzdb,dldlast_z,dvaluedz,dlast_zdvalue,dldz ;
        printf("\n---------start backpropagation-------------\n");

        node_index = number_of_node-1;// using with z_keep

        float dlossdoutput,doutputdz ;

        printf("finding dloss/doutputnode loop\n");
        for (int outputnode_num = output_num-1 ; outputnode_num >= 0 ; outputnode_num--) {

            dlossdoutput = (*lfunction_pointer)(*(output+(int)(outputnode_num)));

            doutputdz = (*doutputfunction_pointer)(z_keep[node_index]);

            printf("node's index :%d   loop output number : %d\n",node_index,outputnode_num);

            bias_adjust_record[layers][outputnode_num] += (doutputdz)/row_num ;
            printf("rate of change of bias : %f\n",doutputdz);

            z_keep[node_index] = dlossdoutput * doutputdz ;
            printf("dL/dz : %f\n",dlossdoutput*doutputdz);

            node_index--;
        }

        

        printf("\ndloss/dweight loop\n");
        //printf("prepare for backprop -->  0 <= layer <= %d\n",layers-1);
        for (int layer_num = layers ; layer_num > 0 ; layer_num --) { 
            printf("--> layer num for backprop : %d\n",layer_num);

            //printf("prepare for closer loop --> num : %d\n",dimention[layer_num]-1); 
            for (int closer_node = dimention[layer_num-1]-1 ; closer_node >= 0 ; closer_node--) { ///segmentation false//////////////////////////////////////
                printf("colsernode : %d  going to link weight with furter node    ",closer_node);
                

                printf(" closer node's index : %d\n",node_index);

                if (layer_num == layers) {
                    for (int furter_node = output_num-1 ; furter_node >= 0 ; furter_node--) {
                        printf("furter node : %d    at index %d\n",furter_node,node_index+furter_node+1);

                        
                    }
                }else{
                    for (int furter_node = dimention[layer_num]-1 ; furter_node >= 0 ; furter_node --) {

                        printf("furter node : %d    at index %d\n",furter_node,node_index+furter_node+1);

                        dldlast_z = z_keep[node_index+1] ; 

                        dlast_zdvalue = weights[layer_num][furter_node*2+closer_node] ; //segmentation false//////////////////

                        //dvaluedz = (*dfunction_pointer[layer_num])(z_keep[node_index]);/////////Segmentation fault//////////////////////////////////
                        //dvaluedz = (*dfunction_pointer[layer_num])(0); //segmentation fault////////
                        //dvaluedz = (*dfunction_pointer[0])(0); // memory over write ???
                        dvaluedz = 1 ;

                        //dzdw = dzdw_array[w_index--] ; /////segmentation false////////////////////////////////////////////////////////////////
                        dzdw = 1 ;
                                
                        dldz = dldlast_z * dlast_zdvalue * dvaluedz ;

                        z_keep[closer_node] = dldz ; //segmentation false ///////////////////////

                        //weight_adjust_record[layer_num][furter_node*2+closer_node] += (dldz * dzdw)/row_num ; //segmentation false
                        weight_adjust_record[0][0] += (dldz * dzdw)/row_num ;
                        //bias_adjust_record[layer_num][closer_node] += (dldz)/row_num ; //segmentation false
                        bias_adjust_record[0][0] += (dldz)/row_num ;
                            
                    }
                }
                
                node_index--;
                
            }
        }

        printf("--> layer num for backprop :0\n");

        for (int closer_node = feature_n-1 ; closer_node >= 0 ; closer_node--) { ///segmentation false//////////////////////////////////////
            printf("colsernode : %d  going to link weight with furter node\n",closer_node);
                
            for (int furter_node = dimention[0]-1 ; furter_node >= 0 ; furter_node--) {
                printf("furter node : %d\n",furter_node);

                
            }

        }

        

    }

    for (int layer_num = 0 ; layer_num < layers ; layer_num++) { //////////////////////////////////
        for (int next_node = 0 ; next_node < dimention[layer_num] ; next_node++) {
            if (layer_num == 0) {
                for (int previous_node = 0 ; previous_node < feature_n ; previous_node++) {////////////////////////////////////////////
                    weights[layer_num][previous_node*2+next_node] -= weight_adjust_record[layer_num][previous_node*1+next_node] * learning_rate ;
                }
            }else{
                for (int previous_node = 0 ; previous_node < dimention[layer_num-1] ; previous_node++) {////////////////////////////////////////////
                    weights[layer_num][previous_node*2+next_node] -= weight_adjust_record[layer_num][previous_node*1+next_node] * learning_rate ;
                }
            }

            //bias[layer_num][next_node] -= bias_adjust_record[layer_num][next_node] * learning_rate ; //segmentation false ///////////////////////////////////
            bias[layer_num][next_node] -= bias_adjust_record[layer_num][next_node] * learning_rate ;

        }
            
    }

}

printf("\nprinting model \n\n");

printf("%d",dimention[1]);

for (int layer_num = 0 ; layer_num < layers ; layer_num++) { 
    printf("model in layer : %d have %d node\n",layer_num,dimention[layer_num]);
    for (int next_node = 0 ; next_node < dimention[layer_num] ; next_node++) {
        printf("\nnode : %d\n",next_node);
        if (layer_num == 0) {
            for (int previous_node = 0 ; previous_node < feature_n ; previous_node++) {
                printf("final weight : %f\n",weights[layer_num][previous_node*2+next_node]);
            }
        }else {
            for (int previous_node = 0 ; previous_node < dimention[layer_num-1] ; previous_node++) {
                printf("final weight : %f\n",weights[layer_num][previous_node*2+next_node]);
            }
        }
        

        printf("final bias : %f\n",bias[layer_num][next_node]);

    }
            
}


printf("end of program....");
///////////// output ////////////////////

return 0;



}


