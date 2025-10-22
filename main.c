#include <stdio.h>
#include <stdlib.h>



int test () {
    for (int i = 0 ; i < 10 ; i ++) {
        printf("test");
    }

    return 0;
}



int main() {

///////////////////////// import file ////////////////////





/////////////// read file, show to user and make user select (feture,row,label) ////////// output --> features,label,features_num ////////



int layers = 1 ; //get from user input

int (*functions_pointer[layers])() ;

functions_pointer[0] = &test; //loop trought user input

// printf("%d",functions_pointer[0]());


///////////// train test split ///////////





////////////// main program /////////////


int adjust_times = 1; // have to get user input first -----> do it later
int row_num = 1 ; // read number of row from file csv
int dimention[1] = {1}; // set of nodes inputed from user
float features_train[1][1] = {{1}}; //get from csv's data
float weights[1][1] = {{1}}; //from random
float bias[1][1] = {{1}}; //from random

int sum ; //declare here is fine


for (int adjust_time_count = 0 ; adjust_time_count < adjust_times ; adjust_time_count ++) {

    for (int row = 0 ; row < row_num ; row ++ ) {

        for (int function_num = 0 ; function_num < layers ; function_num ++ ) {

            sum = 0;

            for (int node_num = 0 ; node_num < dimention[function_num] ; node_num++){

                sum += features_train[row][node_num] * weights[function_num][node_num];
                
            }

            functions_pointer[function_num](sum+bias);

        }

    }

}






///////////// output ////////////////////

return 0;



}


