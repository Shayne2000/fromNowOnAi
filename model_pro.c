#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For isspace
#include <math.h>
#include <model_pro.h>


///////////////////////////////  declare variables //////////////////////













/////////////////////////////// functions ////////////////////////////////


float linear(float x) {
    return x;
}
float d_linear(float x) {
    return 1.0f;
}

float relu(float x) {
    return (x > 0) ? x : 0;
}
float d_relu(float x) {
    return (x > 0) ? 1.0f : 0.0f;
}

void print (int number , float list[] ) {
    printf("\n--------------------------- test -------------------------------\n\n");
    for (int i = 0 ; i < number ; i ++) {
        printf("==> %f\n",list[i]);
    }
    printf("\ntest finished--------------- \n\n");
}
float mse(float *actual, float *predicted, int n) {
    float sum_squared_error = 0.0f;
    for (int i = 0; i < n; i++) {
        float e = predicted[i] - actual[i];
        sum_squared_error += e * e;
    }
    return sum_squared_error / n;
}

void mse_derivative(float *actual, float *predicted, float *grad, int n) {
    float factor = 2.0f / n;
    for (int i = 0; i < n; i++) {
        grad[i] = factor * (predicted[i] - actual[i]);
    }
}




float train(int feature_n,int row_num, DataRow *all_data) {

    int output_num = 1 ;

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
        {25.0, 320.0, 1.0}
        ,
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

int layers = 5 ; //get from user input

float (*functions_pointer[layers])(float) ;
float (*dfunction_pointer[layers])(float) ;
float (*lfunction_pointer)(float*, float*, int);
void  (*dlfunction_pointer)(float*, float*, float*, int);
float (*outputfunctions_pointer)(float) ;
float (*doutputfunction_pointer)(float) ;


functions_pointer[0] = &relu;
functions_pointer[1] = &relu;
functions_pointer[2] = &relu;
functions_pointer[3] = &relu;
functions_pointer[4] = &relu;
dfunction_pointer[0] = &d_relu;
dfunction_pointer[1] = &d_relu;
dfunction_pointer[2] = &d_relu;
dfunction_pointer[3] = &d_relu;
dfunction_pointer[4] = &d_relu;
lfunction_pointer  = &mse;
dlfunction_pointer = &mse_derivative;
outputfunctions_pointer = &linear;
doutputfunction_pointer = &d_linear;

// printf("%d",functions_pointer[0]());

// (*dfunction_pointer)(0) ;
///////////// train test split ///////////


float value ; // just keep things around


////////////// main program /////////////




const int dimention[5] = {1,2,1,1,1}; // set of nodes inputed from user   [3,4,1]

float weights[6][3] = {{0.2,0.3,0.4},{0.5,0.6},{0.7,1.1},{1.2},{2.0},{2.1}}; 
float bias[6][2] = {{3.2},{3.3,3.5},{3.8},{3.9},{4.0},{1.7}}; //from random

 //fix dimention////////////////////////////////////////////////

float sum ; //declare here is fine

int number_of_node = 0 ; // for set size of array according to nodes



for (int layer_num = 0 ; layer_num < layers ; layer_num ++ ) {

    number_of_node += dimention[layer_num] ;
    // printf("%d\n",number_of_node);

}

number_of_node += output_num ;


// float dllastzda_array[] ;
float z_keep [number_of_node] ;
// printf("n : %d",number_of_node);


int z_lastlayer_index = -1 ;

float *output ;
output = malloc(output_num*sizeof(float)) ;

float learning_rate = 0.1 ;

for (int adjust_time_count = 0 ; adjust_time_count < adjust_times ; adjust_time_count ++) {

    ///////printf("loop number : %d\n",adjust_time_count); test memory over flow

    float weight_adjust_record[6][2] = {{0,0,0},{0,0},{0,0},{0},{0},{0}}; //fix dimention//////////////////////////////////////////////
    float bias_adjust_record[6][2] = {{0},{0,0},{0},{0},{0},{0}};

    //printf("there is %d rows of data\n",row_num);
    printf("\n");

    for (int row = 0 ; row < row_num ; row ++ ) {

        printf("\n             run trought row : %d\n",row);


        int node_index = 0; // using with activationfunction_output
        int previouslayernode_sum = 0 ;//for tracking previous node during interaction between layers

        for (int layer_num = 0 ; layer_num < layers ; layer_num ++ ) {

            for (int node_num = 0 ; node_num < dimention[layer_num] ; node_num++){
                sum = 0;
                if (layer_num == 0) {
                    for (int previous_node = 0 ; previous_node < feature_n ; previous_node++ ) {

                        
                        sum += all_data[row].feature_values[previous_node] * weights[layer_num][node_num*feature_n+previous_node] ;
                        printf("(%f * %f) + ",all_data[row].feature_values[previous_node],weights[layer_num][node_num*feature_n+previous_node]);

                        // printf("weight index : %d * %d + %d = %d\n",node_num,feature_n,previous_node,node_num*feature_n+previous_node);
                        //dzdw_array[w_index++] = all_data[row].feature_values[previous_node] ;
                        
                    }
                    
                }else{

                    for (int previous_node = 0 ; previous_node < dimention[layer_num-1]; previous_node ++ ) {

                        // print(number_of_node,z_keep);
                        
                        value = (*functions_pointer[layer_num-1])(z_keep[previouslayernode_sum + previous_node]) ; //last z

                        // printf("weight index : %d\n",node_num*dimention[layer_num-1]+previous_node);
                        // printf("z index that use for calculation : %d + %d = %d\n",previouslayernode_sum,previous_node,previouslayernode_sum+previous_node);
                        // printf("%f\n",z_keep[previouslayernode_sum + previous_node]);
                        // printf("%f",z_keep[0]);

                        // printf("(%f * %f) + ",value,weights[layer_num][node_num*dimention[layer_num-1]+previous_node]);
                        //printf("value : %f    previous_sum : %d    previous_node : %d\n",value,previouslayernode_sum,previous_node);
                        sum += value * weights[layer_num][node_num*dimention[layer_num-1]+previous_node] ;
                        //dzdw_array[w_index++] = value ;//?
                    
                        //printf("using value of node %d",previouslayernode_sum+previous_node);
                    }
                    
                }
                //printf("\n");

                
                //printf("base add : %d     layer : %d\n",dimention[layer_num],layer_num);
                //printf("bias at layer : %d   node : %d     value : %f\n",layer_num,node_num,bias[layer_num][node_num]);
                sum += bias[layer_num][node_num] ; 
                printf("%f\n",bias[layer_num][node_num]);
                z_keep[node_index] = sum ;
                // printf("output in this node : %f at index %d\n",sum,node_index);
                sum = (*functions_pointer[layer_num])(sum) ; 

                //printf("w index as in fp : %d\n",w_index);
                
                // printf("after function : %f at index %d\n",sum,node_index);
                

                node_index++;
                
            }

            //printf("finished one layer\n");
            if (layer_num != 0) {
                previouslayernode_sum += dimention[layer_num-1] ;
                // printf("previousnode sum += %d\n",dimention[layer_num-1]);
            }
            

        }
        // print(number_of_node,z_keep);

        printf("\n-----finished forward propagation--------\n");
        //print(number_of_node,z_keep);

        for (int outputnode_num = 0 ; outputnode_num < output_num ; outputnode_num ++) { 
            
            sum = 0 ;
            for (int lasthiddennode_num = 0 ; lasthiddennode_num < dimention[layers-1]; lasthiddennode_num++){

                //printf("%d %d\n",layers-1,lasthiddennode_num);
                
                // printf("z index that use for calculation : %d - %d + %d = %d\n",node_index,dimention[layers-1],lasthiddennode_num,node_index-dimention[layers-1]+lasthiddennode_num);
                value = (*functions_pointer[layers-1])(z_keep[node_index-dimention[layers-1]+lasthiddennode_num]); //value of last hidden

                //printf("%d * %d + %d = \n",outputnode_num,output_num,lasthiddennode_num);
                //printf("weight index : %d\n",outputnode_num*output_num+lasthiddennode_num);
                //printf("weight index : %d\n",w_index);
                //printf("value * weight = %f * %f \n",value,weights[layers][w_index]);
                sum += value * weights[layers][outputnode_num*output_num+lasthiddennode_num]; 
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

        // losses[adjust_time_count] = (*lfunction_pointer)(label_samples,output,output_num);
        float label_arr[1];
        label_arr[0] = all_data[row].label_value;

        losses[adjust_time_count] = lfunction_pointer(label_arr, output, 1);
        



        float dzdw,dzdz,dzdb,dldlast_z,dvaluedz,dlast_zdvalue,dldz,dlda ;
        printf("\n---------start backpropagation-------------\n");

        node_index = number_of_node-1;// using with z_keep
        //printf("w index start at %d\n\n",--w_index);
        //w_index-- ;

        float dlossdoutput,doutputdz;
        // float *grad ;
        // grad = (float *)malloc(sizeof(float)*output_num);
        // float dldzs[layers+1][];

        float grad[1];
        dlfunction_pointer(label_arr, output, grad, 1);

        dlossdoutput = grad[0]; 

        // (*dlfunction_pointer)(feature_samples,output,grad,output_num);

        // dldas[layers] = 0;

        //printf("finding dloss/doutputnode loop\n");
        for (int outputnode_num = output_num-1 ; outputnode_num >= 0 ; outputnode_num--) {

            dlossdoutput = (*grad+outputnode_num);

            doutputdz = (*doutputfunction_pointer)(z_keep[node_index]);

            

            bias_adjust_record[layers][outputnode_num] += (dlossdoutput)/row_num ;
            printf("furtest ==> dloss / dbias : %f\n",dlossdoutput);

            z_keep[number_of_node-output_num+outputnode_num] = dlossdoutput * doutputdz ;


            node_index--;
        }

        // print(number_of_node,z_keep);
        // print(3,bias_adjust_record[layers]);

        int base = number_of_node -output_num - 1 - dimention[layers-1]; //closssss
        int change_base = 0;
        node_index;
       

        //printf("\ndloss/dweight loop\n");
        //printf("prepare for backprop -->  0 <= layer <= %d\n",layers-1);
        for (int layer_num = layers ; layer_num > 0 ; layer_num --) { 
            printf("\n--> layer num for backprop : %d\n\n",layer_num);



            for (int closer_node = dimention[layer_num-1]-1 ; closer_node >= 0 ; closer_node--) { 

                dlda = 0;
                if (layer_num == layers) {

                    node_index = number_of_node-output_num-dimention[layers-1]+1;

                    // print(number_of_node,z_keep);

                    for (int furter_node = output_num-1 ; furter_node >= 0 ; furter_node--) {

                        dldlast_z = z_keep[base+furter_node+dimention[layers-1]+1] ; 

                        dzdw = (*functions_pointer[layer_num-1])(z_keep[base+closer_node+1]);

                        printf("furtest ==> dloss/dw%d : %f\n",closer_node,dldlast_z*dzdw);

                        weight_adjust_record[layer_num][furter_node*dimention[layer_num-1]+closer_node] += (dldlast_z * dzdw)/row_num ;



                        // printf("\nfind dlda for furter  z index : %d + %d + %d + 1 = %d\n",base,furter_node,dimention[layer_num-1],base+furter_node+dimention[layers-1]+1);
                        // printf("find dlda for furter  w (layer,index) : %d,(%d + %d * %d = %d)\n\n",layer_num,closer_node,dimention[layer_num-1],furter_node,furter_node*dimention[layer_num-1]+closer_node);

                        dlda += z_keep[base+furter_node+dimention[layers-1]+1] * weights[layer_num][closer_node+furter_node*dimention[layer_num-1]] ;
                        // printf("dlda += %f * %f",z_keep[base+furter_node+dimention[layers-1]+1],weights[layer_num][closer_node+furter_node*dimention[layer_num-1]]);
                        // printf(" = %f\n",z_keep[base+furter_node+dimention[layers-1]+1] * weights[layer_num][closer_node+furter_node*dimention[layer_num-1]]);
                    }

                    dvaluedz = (*dfunction_pointer[layer_num-1])(z_keep[base+closer_node+1]);
                    // printf("dvalue/dz = %f\n",dvaluedz);
                    // printf("dlda : %f\n",dlda);
                    dldz = dlda * dvaluedz;
                    bias_adjust_record[layer_num-1][closer_node] = dldz;
                    printf("==> dloss/ dbias : %f\n",dldz);

                    // printf("z index to keep dldz in closer node : %d + %d + 1 = %d    value : %f\n\n",base,closer_node,base+closer_node+1,dldz);
                    z_keep[base+closer_node+1] = dldz;

                }else{ //between hidden and hidden

                    

                    // printf("run at dimention %d --> %d node\n",layer_num,dimention[layer_num]);
                    for (int furter_node = dimention[layer_num]-1 ; furter_node >= 0 ; furter_node--) {
                        
                        dldlast_z = z_keep[base+furter_node+dimention[layer_num-1]+1] ; 
                        // printf("last z at z index : %d + %d + %d + 1 = %d\n",base,furter_node,dimention[layer_num-1],base+furter_node+dimention[layer_num-1]+1);

                        dzdw = (*functions_pointer[layer_num-1])(z_keep[base+closer_node+1]);

                        printf("==> dloss/dw%d : %f\n",closer_node,dldlast_z*dzdw);

                        weight_adjust_record[layer_num][furter_node*dimention[layer_num-1]+closer_node] += (dldlast_z * dzdw)/row_num ;

                        // printf("\nfind dlda for furter  z index : %d + %d + %d + 1 = %d\n",base,furter_node,dimention[layer_num-1],base+furter_node+dimention[layer_num-1]+1);
                        // printf("find dlda for furter  w (layer,index) : %d,(%d + %d * %d = %d)\n\n",layer_num,closer_node,dimention[layer_num-1],furter_node,furter_node*dimention[layer_num-1]+closer_node);

                        dlda += z_keep[base+furter_node+dimention[layer_num-1]+1] * weights[layer_num][closer_node+furter_node*dimention[layer_num-1]] ;
                        // printf("dlda += %f * %f = ",z_keep[base+furter_node+dimention[layer_num-1]+1],weights[layer_num][closer_node+furter_node*dimention[layer_num-1]]);
                        // printf("%f\n",z_keep[base+furter_node+dimention[layer_num-1]+1] * weights[layer_num][closer_node+furter_node*dimention[layer_num-1]]);
                        // printf("");   
                    }

                    dvaluedz = (*dfunction_pointer[layer_num-1])(z_keep[base+closer_node+1]);

                    // printf("dlda = %f\n",dlda);
                    dldz = dlda * dvaluedz;
                    z_keep[base+closer_node+1] = dldz;
                    // print(number_of_node,z_keep);
                    bias_adjust_record[layer_num-1][closer_node] = dldz ;

                    printf("==> dloss/ dbias : %f\n",dldz);
                    
                
                    node_index--;
                //printf("node index : %d\n",node_index);
                }
                // print(3,bias_adjust_record[layer_num]);
                
            }
            if (layer_num>=2) {
                // printf("base - (num of %d)       %d  -->  %d\n",layer_num-1,base,base-dimention[layer_num-2]);
                base -= dimention[layer_num-2];
            }
            
        }
        // print(number_of_node,z_keep);
        printf("\n--> layer num for backprop :0\n\n");
        // printf("base variable in final phase : %d\n",base);

        
        for (int closer_node = feature_n-1 ; closer_node >= 0 ; closer_node--) { ///segmentation false//////////////////////////////////////
            //printf("colsernode : %d  going to link weight with furter node\n",closer_node);

            dlda = 0;
            // printf("dl/da : ");
            for (int furter_node = dimention[0]-1 ; furter_node >= 0 ; furter_node--) {

                dldlast_z = z_keep[base+furter_node+1] ; 
                // printf("last z at z index : %d + %d + 1 = %d\n",base,furter_node,base+furter_node+1);

                dzdw = all_data[row].feature_values[closer_node];

                printf("==> dloss/dw%d : %f\n",closer_node,dldlast_z*dzdw);

                weight_adjust_record[0][furter_node*feature_n+closer_node] += (dldlast_z * dzdw)/row_num ;

                // printf("\nfind dlda for furter  z index : %d + %d + %d + 1 = %d\n",base,furter_node,dimention[layer_num-1],base+furter_node+dimention[layer_num-1]+1);
                // printf("find dlda for furter  w (layer,index) : %d,(%d + %d * %d = %d)\n\n",layer_num,closer_node,dimention[layer_num-1],furter_node,furter_node*dimention[layer_num-1]+closer_node);

            }

        }

    }

    printf("\n######## change weight-bias value ##############\n") ;

    for (int layer_num = 0 ; layer_num < layers+1 ; layer_num++) { 
        // printf("layer : %d\n",layer_num);
        if(layer_num == layers){
            for (int next_node = 0 ; next_node < output_num ; next_node++) {
                for (int previous_node = 0 ; previous_node < dimention[layer_num-1] ; previous_node++) {
                    weights[layer_num][next_node*dimention[layer_num-1]+previous_node] -= weight_adjust_record[layer_num][previous_node+next_node*dimention[layer_num-1]] * learning_rate ;
                    printf("add w : %f at (layer,node) : %d,%d\n",weight_adjust_record[layer_num][previous_node+next_node*dimention[layer_num-1]] * learning_rate,layer_num,previous_node+dimention[layer_num-1]*next_node);
                    // printf("layer : %d\n",layer_num);
                }

                bias[layer_num][next_node] -= bias_adjust_record[layer_num][next_node] * learning_rate ;
                printf("add b : %f at (layer,node) : %d,%d\n\n",bias_adjust_record[layer_num][next_node] * learning_rate,layer_num,next_node);
                // printf("layer : %d\n",layer_num);

            }

        }else{    
            for (int next_node = 0 ; next_node < dimention[layer_num] ; next_node++) {
                if (layer_num == 0) {
                    for (int previous_node = 0 ; previous_node < feature_n ; previous_node++) {
                        weights[layer_num][next_node*feature_n+previous_node] -= weight_adjust_record[layer_num][previous_node+next_node*feature_n] * learning_rate ;
                        printf("add w : %f at (layer,node) : %d,%d\n",weight_adjust_record[layer_num][previous_node+next_node*feature_n] * learning_rate,layer_num,previous_node+feature_n*next_node);
                        // printf("layer : %d\n",layer_num);
                    }
                }else{
                    for (int previous_node = 0 ; previous_node < dimention[layer_num-1] ; previous_node++) {
                        weights[layer_num][next_node*dimention[layer_num-1]+previous_node] -= weight_adjust_record[layer_num][previous_node+next_node*dimention[layer_num-1]] * learning_rate ;
                        printf("add w : %f at (layer,node) : %d,%d\n",weight_adjust_record[layer_num][previous_node+next_node*dimention[layer_num-1]] * learning_rate,layer_num,previous_node+dimention[layer_num-1]*next_node);
                        // printf("layer : %d\n",layer_num);
                    }
                }

                bias[layer_num][next_node] -= bias_adjust_record[layer_num][next_node] * learning_rate ;
                printf("add b : %f at (layer,node) : %d,%d\n\n",bias_adjust_record[layer_num][next_node] * learning_rate,layer_num,next_node);
                // printf("layer : %d\n",layer_num);

            }
                
        }
        printf("\n");
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
                printf("final weight : %f\n",weights[layer_num][previous_node+next_node*feature_n]);
            }
        }else {
            for (int previous_node = 0 ; previous_node < dimention[layer_num-1] ; previous_node++) {
                printf("final weight : %f\n",weights[layer_num][previous_node+next_node*dimention[layer_num-1]]);
            }
        }
        

        printf("final bias : %f\n\n\n",bias[layer_num][next_node]);

    }
            
}
for (int next_node = 0 ; next_node < output_num ; next_node++) {
        printf("\nnode : %d\n",next_node);
        // printf("%d",dimention[layers]);
        for (int previous_node = 0 ; previous_node < dimention[layers-1] ; previous_node++) {
            printf("final weight : %f\n",weights[layers][previous_node+next_node*dimention[layers-1]]);
        }

        printf("final bias : %f\n\n\n",bias[layers][next_node]);

    }
        




printf("end of program....");
///////////// output ////////////////////

// printf("add w : %f\n",weight_adjust_record[0][2+0*3] * learning_rate);

return 0;



}


