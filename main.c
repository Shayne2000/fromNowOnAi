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
int output_num = 1 ;

float test (float a) {
    
   // printf("activated a function\n");

    return a;
}

float test1(float a ) {
    return 1 ;
}

float testl (float *a) {
    float sum = 0 ,size;
    size = output_num;
    for (int i = 0 ; i < size ; i ++) {
        sum += *(a+i)/size ;
    }
    return sum ;
}

void print (int number , float list[] ) {
    printf("\n--------------------------- test -------------------------------\n\n");
    for (int i = 0 ; i < number ; i ++) {
        printf("==> %f\n",list[i]);
    }
    printf("\ntest finished--------------- \n\n");
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
        {25.0, 320.0, 1.0},
        {30.0, 450.0, 3.0},
        {22.0, 280.0, 0.5},
        {40.0, 600.0, 10.0},
        {35.0, 520.0, 7.0}
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

float losses[adjust_times] ; //for plot graph
float loss ;

int layers = 3 ; //get from user input

float (*functions_pointer[layers])(float) ;
float (*dfunction_pointer[layers])(float) ;
float (*lfunction_pointer)(float *) ;
float (*dlfunction_pointer)(float) ;
float (*outputfunctions_pointer)(float) ;
float (*doutputfunction_pointer)(float) ;


functions_pointer[0] = &test;
functions_pointer[1] = &test;
functions_pointer[2] = &test;
dfunction_pointer[0] = &test1;
dfunction_pointer[1] = &test1;
dfunction_pointer[2] = &test1;
lfunction_pointer = &testl;
dlfunction_pointer = &test1;
outputfunctions_pointer = &test;
doutputfunction_pointer = &test1;

// printf("%d",functions_pointer[0]());

// (*dfunction_pointer)(0) ;
///////////// train test split ///////////


float value ; // just keep things around


////////////// main program /////////////




const int dimention[3] = {3,3,3}; // set of nodes inputed from user   [3,4,1]
//float features_train[1][1] = {{1}}; //get from csv's data   
float weights[4][9] = {{0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,0.1},{1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9},{2.0,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8},{2.9,3.0,3.1}}; 
float bias[4][3] = {{3.2,3.3,3.4},{3.5,3.6,3.7},{3.8,3.9,4.0},{4.1}}; //from random

float sum ; //declare here is fine
int w_size = 0; //for set z_value-array size 
int number_of_node = 0 ; // for set size of array according to nodes



for (int layer_num = 0 ; layer_num < layers ; layer_num ++ ) {
    
    if (layer_num == 0) {
        w_size += feature_n*dimention[layer_num] ;
    }else if (layer_num+1 == layers) {
        w_size += output_num * dimention[layer_num] ;
    }else {
        w_size += dimention[layer_num-1] * dimention[layer_num] ;
    }


    number_of_node += dimention[layer_num] ;
    printf("%d\n",number_of_node);

}

number_of_node += output_num ;


// float dllastzda_array[] ;
float z_keep [number_of_node] ;
printf("n : %d",number_of_node);


int z_lastlayer_index = -1 ;

float *output ;
output = malloc(output_num*sizeof(float)) ;

float learning_rate = 0.1 ;

for (int adjust_time_count = 0 ; adjust_time_count < adjust_times ; adjust_time_count ++) {

    ///////printf("loop number : %d\n",adjust_time_count); test memory over flow

    float weight_adjust_record[4][9] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0}}; //fix dimention//////////////////////////////////////////////
    float bias_adjust_record[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0}}; //fix dimention////////////////////////////////////////////////

    //printf("there is %d rows of data\n",row_num);
    printf("\n");

    for (int row = 0 ; row < row_num ; row ++ ) {

        printf("\n             run trought row : %d\n",row);

        int w_index = w_size; //for tracking index easily
        int node_index = 0; // using with activationfunction_output
        int previouslayernode_sum = 0 ;//for tracking previous node during interaction between layers

        for (int layer_num = 0 ; layer_num < layers ; layer_num ++ ) {

            for (int node_num = 0 ; node_num < dimention[layer_num] ; node_num++){
                sum = 0;
                if (layer_num == 0) {
                    for (int previous_node = 0 ; previous_node < feature_n ; previous_node++ ) {

                        
                        sum += all_data[row].feature_values[previous_node] * weights[layer_num][node_num*dimention[layer_num]+previous_node] ;
                        // printf("(%f * %f) + ",all_data[row].feature_values[previous_node],weights[layer_num][node_num*dimention[layer_num]+previous_node]);

                        // printf("%d +* %d = ",node_num,previous_node);
                        //printf("weight index : %d\n",node_num*dimention[layer_num]+previous_node);
                        //dzdw_array[w_index++] = all_data[row].feature_values[previous_node] ;
                        
                    }
                    
                }else{

                    for (int previous_node = 0 ; previous_node < dimention[layer_num-1]; previous_node ++ ) {

                        //print(number_of_node,z_keep);
                        
                        value = (*functions_pointer[layer_num-1])(z_keep[previouslayernode_sum + previous_node]) ; //last z

                        //printf("weight index : %d\n",node_num*dimention[layer_num]+previous_node);
                        // printf("z index that use for calculation : %d + %d = %d\n",previouslayernode_sum,previous_node,previouslayernode_sum+previous_node);
                        // printf("%f\n",z_keep[previouslayernode_sum + previous_node]);
                        // printf("%f",z_keep[0]);

                        // printf("(%f * %f) + ",value,weights[layer_num][node_num*dimention[layer_num]+previous_node]);
                        //printf("value : %f    previous_sum : %d    previous_node : %d\n",value,previouslayernode_sum,previous_node);
                        sum += value * weights[layer_num][node_num*dimention[layer_num]+previous_node] ;
                        //dzdw_array[w_index++] = value ;//?
                    
                        //printf("using value of node %d",previouslayernode_sum+previous_node);
                    }
                    
                }
                //printf("\n");

                
                //printf("base add : %d     layer : %d\n",dimention[layer_num],layer_num);
                //printf("bias at layer : %d   node : %d     value : %f\n",layer_num,node_num,bias[layer_num][node_num]);
                sum += bias[layer_num][node_num] ; 
                // printf("%f\n",bias[layer_num][node_num]);
                z_keep[node_index] = sum ;
                sum = (*functions_pointer[layer_num])(sum) ; 

                //printf("w index as in fp : %d\n",w_index);
                

                printf("output in this node : %f at index %d\n",sum,node_index);

                node_index++;
                
            }

            //printf("finished one layer\n");
            if (layer_num != 0) {
                previouslayernode_sum += dimention[layer_num] ;
            }
            

        }
        print(number_of_node,z_keep);

        printf("\n-----finished forward propagation--------\n");
        //print(number_of_node,z_keep);

        for (int outputnode_num = 0 ; outputnode_num < output_num ; outputnode_num ++) { 
            
            sum = 0 ;
            for (int lasthiddennode_num = 0 ; lasthiddennode_num < dimention[layers-1]; lasthiddennode_num++){

                //printf("%d %d\n",layers-1,lasthiddennode_num);
                
                printf("z index that use for calculation : %d - %d + %d = %d\n",node_index,dimention[layers-1],lasthiddennode_num,node_index-dimention[layers-1]+lasthiddennode_num);
                value = (*functions_pointer[layers-1])(z_keep[node_index-dimention[layers-1]+lasthiddennode_num]); //value of last hidden

                //printf("%d * %d + %d = \n",outputnode_num,output_num,lasthiddennode_num);
                //printf("weight index : %d\n",outputnode_num*output_num+lasthiddennode_num);
                //printf("weight index : %d\n",w_index);
                //printf("value * weight = %f * %f \n",value,weights[layers][w_index]);
                sum += value * weights[layers][outputnode_num*output_num+lasthiddennode_num]; 
                //dzdw_array[w_index++] = value; ////////////////////////////////////////////////////////////////////////////
                //printf("%d\n",lasthiddennode_num);
            }

            sum += bias[layers][outputnode_num] ; 
            z_keep[node_index+outputnode_num] = sum ;
            sum = (*outputfunctions_pointer)(sum);
            *(output+outputnode_num) = sum ;

            printf("output node%d's answer : %f\n",outputnode_num,sum);
        }

        printf("-------finish answering----------\n");
        // print(number_of_node,z_keep);


///--------------------- form here, z_keep will keep dl/dz value  no ----------------------

        losses[adjust_time_count] = (*lfunction_pointer)(output); ////////////////////////////////////////

        float dzdw,dzdz,dzdb,dldlast_z,dvaluedz,dlast_zdvalue,dldz,dlda ;
        printf("\n---------start backpropagation-------------\n");

        node_index = number_of_node-1;// using with z_keep
        //printf("w index start at %d\n\n",--w_index);
        //w_index-- ;

        float dlossdoutput,doutputdz ;
        // float dldzs[layers+1][];

        // dldas[layers] = 0;

        //printf("finding dloss/doutputnode loop\n");
        for (int outputnode_num = output_num-1 ; outputnode_num >= 0 ; outputnode_num--) {

            dlossdoutput = (*dlfunction_pointer)(*(output+(int)(outputnode_num)));
            //printf("dL/doutput : %f\n",dlossdoutput);

            doutputdz = (*doutputfunction_pointer)(z_keep[node_index]);

            

            bias_adjust_record[layers][outputnode_num] += (doutputdz)/row_num ;
            printf("furtest ==> dloss / dbias : %f\n",doutputdz);

            z_keep[number_of_node-output_num+outputnode_num] = dlossdoutput * doutputdz ;
            //printf("z of outputs at index ####################### %d\n",number_of_node-output_num+outputnode_num);
            
            // printf("dloss/dz : %f\n",doutputdz * dlossdoutput);

            node_index--;
        }

        //print(number_of_node,z_keep);

        int base = number_of_node -output_num - 1; //closssss
        node_index;
        // printf("base : %d\n",base);

        //printf("\ndloss/dweight loop\n");
        //printf("prepare for backprop -->  0 <= layer <= %d\n",layers-1);
        for (int layer_num = layers ; layer_num > 0 ; layer_num --) { 
            printf("--> layer num for backprop : %d\n",layer_num);

            // dldas[layer_num-1] = 0 ;

            //printf("prepare for closer loop --> num : %d\n",dimention[layer_num]-1); 

            //value = number_of_node

            for (int closer_node = dimention[layer_num-1]-1 ; closer_node >= 0 ; closer_node--) { ///segmentation false//////////////////////////////////////
                // printf("colsernode : %d  going to link weight with furter node    \n",closer_node);

                //printf("base : %d\n",base);
                

                //printf(" closer node's index : %d\n",node_index);
                dlda = 0;
                if (layer_num == layers) {

                    

                    for (int furter_node = output_num-1 ; furter_node >= 0 ; furter_node--) {
                        //printf("furter node : %d    at index %d\n",furter_node,node_index+furter_node+1);
                        //printf("node index : %d\n",node_index);

                        dldlast_z = z_keep[base+dimention[layer_num-1]+furter_node] ; 
                        //printf("lastz index : %d + %d + %d = %d\n",base,dimention[layer_num-1],furter_node,base+dimention[layer_num-1]+furter_node);
                        //printf("dloss/dlastz : %f\n",dldlast_z);
                        //print(number_of_node,z_keep);

                        dzdw = (*functions_pointer[layer_num-1])(z_keep[node_index+closer_node]);
                        //printf("last z/a index : %d\n",node_index+closer_node);
                        //print(number_of_node,z_keep);
                        //printf("dlastz / dweight : %f\n",dzdw);
                        //printf("closer a : %f\n",z_keep[node_index+closer_node]);
                        

                        printf("furtest ==> dloss/dw%d : %f\n",closer_node,dldlast_z*dzdw);

                        z_keep[base+closer_node] = dldlast_z ;
                        //printf("szve z at index : %d + %d = %d\n",base,closer_node,base+closer_node);


                        weight_adjust_record[layer_num][w_index--] += dldlast_z * dzdw ;


                        bias_adjust_record[layer_num][closer_node] += dldlast_z ;


                    }
                }else{
                    //printf("dl/da : ");
                    for (int furter_node = dimention[layer_num]-1 ; furter_node >= 0 ; furter_node--) {

                        // dlda += z_keep[base+dimention[layer_num-1]+furter_node] * weights[layer_num+1][furter_node*dimention[layer_num+1]] ; //segmentation false
                        printf("z at : %d and w at (layer,index) : (%d,%d)\n",base+dimention[layer_num-1]+furter_node,layer_num+1,furter_node*dimention[layer_num+1]);
                        // printf("(%f * %f) + ",z_keep[base+dimention[layer_num-1]+furter_node],weights[layer_num+1][closer_node*dime]);

                    }//printf("0\n");
                    for (int furter_node = dimention[layer_num]-1 ; furter_node >= 0 ; furter_node --) {

                        //printf("furter node : %d    at index %d\n",furter_node,node_index+furter_node+1);

                        //dldlast_z = z_keep[base+furter_node+1] ;
                        //print(number_of_node,z_keep);
                        //printf("z index : %d + %d + 1 = %d\n",base,furter_node,base+furter_node+1);
                        //printf("dloss / dlastz : %f\n",dldlast_z);

                        //dlast_zdvalue = weights[layer_num+1][furter_node*dimention[layer_num]+closer_node] ;
                        //printf("dlastZ / da : %f\n",dlast_zdvalue);
                        // printf("dloss / da : %f\n",dlda);

                        dvaluedz = (*dfunction_pointer[layer_num])(z_keep[node_index]);


                        dldz = dlda * dvaluedz ;
                        // printf("dloss / dz : %f * %f = %f\n",dlda,dvaluedz,dldz);

                        //print(number_of_node,z_keep);
                        dzdw = (*functions_pointer[layer_num])(z_keep[node_index+furter_node]);
                        //printf("dz / dw : %f\n",dzdw);

                        z_keep[node_index+1] = dldz ;

                        weight_adjust_record[layer_num][w_index] += (dldz * dzdw)/row_num ;
                        printf("==> dloss / dweight%d : %f   keep at %d\n",furter_node,(dldz * dzdw),w_index);
                        
                        bias_adjust_record[layer_num][closer_node] += (dldz)/row_num ;
                        printf("==> dloss / dbias%d : %f\n",furter_node,(dldz));
                        
                        w_index--;
                            
                    }
                    base -= dimention[layer_num];
                    //printf("base - (num of %d)\n",layer_num);
                }
                
                node_index--;
                //printf("node index : %d\n",node_index);
                
                
            }
        }
        print(number_of_node,z_keep);
        printf("\n--> layer num for backprop :0\n\n");
        // printf("base variable in final phase : %d\n",base);

        
        for (int closer_node = feature_n-1 ; closer_node >= 0 ; closer_node--) { ///segmentation false//////////////////////////////////////
            //printf("colsernode : %d  going to link weight with furter node\n",closer_node);

            dlda = 0;
            printf("dl/da : ");
            for (int furter_node = dimention[0]-1 ; furter_node >= 0 ; furter_node--) {

                dlda += z_keep[dimention[0]+furter_node] * weights[1][furter_node*dimention[1]] ;
                printf("z at : %d and w at (layer,index) : (%d,%d)\n",furter_node+dimention[0],0,furter_node*dimention[1]);
                printf("(%f * %f) + ",z_keep[furter_node+dimention[0]],weights[1][dimention[1]+closer_node]);

            }
            printf("0\n");
                
            for (int furter_node = dimention[0]-1 ; furter_node >= 0 ; furter_node--) {
                //printf("furter node : %d\n",furter_node);

                // dldlast_z = z_keep[base+furter_node+1] ;
                // printf("dloss / dlastz : %f\n",dldlast_z);

                // dlast_zdvalue = weights[1][furter_node*dimention[0]+closer_node] ;
                // printf("dlastz / da : %f\n",dlda);
                //printf("weight index : %d * %d + %d = %d\n",furter_node,dimention[0],closer_node,furter_node*dimention[0]+closer_node);

                printf("dloss / da : %f\n",dlda);
                //printf("dzdw at weight index : %d\n",w_index--);

                dvaluedz = (*dfunction_pointer[0])(z_keep[base+closer_node]);
                printf("da / dz : %f\n",dvaluedz);

                dzdw = all_data[row].feature_values[closer_node] ;
                printf("row : %d   column : %d\n",row,closer_node);
                printf("dz / dw : %f\n",dzdw);



                printf("==> dloss / dw%d : %f\n\n",closer_node+1,dzdw*dvaluedz*dlda);

            }

        }

        

    }

    printf("\n######## change weight-bias value ##############\n") ;

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
    printf("model in layer : %d    have %d node\n",layer_num,dimention[layer_num]);
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


