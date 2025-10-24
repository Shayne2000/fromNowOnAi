#include <stdio.h>
#include <math.h>

float maximum(float x, float y){

    if(x > y) return x;
    else return y;
}

float sigmoid(float z){

    float result = 1 / (1 + exp(-z));
    return result;
}

#define LAYER_LEN 5 // Maximum expected length layer (change this later)
// function operator
float softmax(float layer[LAYER_LEN], float z){
    float exp_layer_sum = 0;
    for(int i = 0; i < LAYER_LEN; i++) {
        exp_layer_sum += exp(layer[i]);
    }
    return exp(z) / exp_layer_sum;
}

int main(){
    float probabilities[LAYER_LEN]; 
    float Output_layer[LAYER_LEN] = {1.3, 5.1, 2.2, 0.7, 1.1};

    // display all Output_layer
    printf("==================================================\n");
    printf("Output_layer : {");
    for(int i = 0; i < LAYER_LEN; i++){
        printf("%.3f", Output_layer[i]);
        if(i != LAYER_LEN - 1) printf(", ");
    }
    printf("}\n");
    printf("==================================================\n");

    // display all probability
    printf("probabilities : {");
    for(int i = 0; i < LAYER_LEN; i++){
        probabilities[i] = softmax(Output_layer, Output_layer[i]);
        printf("%.2f", probabilities[i]);
        if(i != LAYER_LEN - 1) printf(", ");
    }
    printf("}\n");
    printf("==================================================\n");

    return 0;
}
