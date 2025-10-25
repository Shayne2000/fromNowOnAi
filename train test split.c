#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define feature_num 2 //according to feature_num from part 1
#define numOutput 1
#define Max_node 10

#define Train_Ratio 0.8
#define row_Num 10//row_num from part 1
#define Train_Size ((int)(row_Num * Train_Ratio))
#define Test_Size (row_Num - Train_Size)

double init_weights(){
    return((double)rand()) / ((double)RAND_MAX)* 2.0 - 1.0;
}

void shuffle(int *array ,int size_training){
    if(size_training > 1){
        for(int i=0; i<size_training; i++){
            int j = i + rand() % (size_training-i);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}


int train_test_split(){
    srand((unsigned int)time(NULL));
    float data[row_Num][feature_num];
    float labels[row_Num];

    float train_data[Train_Size][feature_num];
    float test_data[Test_Size][feature_num];

    float train_labels[Train_Size];
    float test_labels[Test_Size];
//------------------------------------------------------------
    int hiddenLayers_num;

    printf("Hidden Layer: ");//ask amount of hidden layer
    scanf("%d", &hiddenLayers_num);
     if (hiddenLayers_num < 1){
        printf("Invalid hidden layer number. Exiting.\n");
        return -1;
    }

    int *HiddenNode_num = malloc(hiddenLayers_num * sizeof(int));
    int hiddenNode_n;

    printf("Number of Hidden Nodes: ");//ask amount of hidden node
    scanf("%d", &hiddenNode_n);
    if (hiddenNode_n < 1) {
            printf("Invalid hidden node number in layer %d. Exiting.\n");
            free(HiddenNode_num);
            return -1;
    }

    for (int i = 0; i<hiddenLayers_num; i++) {
            HiddenNode_num[i] = hiddenNode_n;//amount of node in each hidden layer
    }

    printf("\nNetwork structure:\n"); //just show amount of hidden layer, node
    for (int i = 0; i < hiddenLayers_num; i++) {
        printf(" Hidden Layer %d: %d nodes\n", i + 1, HiddenNode_num[i]);
    }
//-----------------------------------------------------
    /*for (int i = 0; i < row_Num; i++) {
        for (int j = 0; j < feature_num; j++) {
            data[i][j]= all_data[i].feature_values[j]; //Collect data from selected features (from part 1)
        }
        labels[i] = all_data[i].label_value; //Collect data from selected labels (from part 1)
    }*/
//-----------------------------------------------------------------------------
    int index[row_Num]; //shuffle data
    for(int i=0; i<row_Num; i++){
        index[i] = i;
    }
    shuffle(index, row_Num);

    for (int i=0; i < Train_Size; i++) { //train data
        for (int j = 0; j < feature_num; j++) {
            train_data[i][j] = data[index[i]][j];
        }
        train_labels[i] = labels[index[i]];
    }

    for (int i=0; i < Test_Size; i++) { //test data
        for (int j = 0; j < feature_num; j++) {
            test_data[i][j] = data[index[i + Train_Size]][j];
        }
        test_labels[i] = labels[index[i + Train_Size]];
    }

    float hiddenLayersBias[hiddenLayers_num][hiddenNode_n];
    float outputLayersBias[numOutput];

    float hiddenWeights[hiddenLayers_num][hiddenNode_n][Max_node];
    float outputWeights[hiddenNode_n][numOutput];

    for(int i = 0; i < hiddenLayers_num; i++){//random hidden layer weight
        int input_size;
        if(i==0) input_size = feature_num;
        else input_size = hiddenNode_n;
        for(int j=0; j < hiddenNode_n; j++){
            for(int k = 0; k < input_size; k++){
            hiddenWeights[i][j][k] = init_weights();
            }
        }
    }


    for(int i=0; i < hiddenNode_n; i++){//random output's weight
        for(int j=0; j < numOutput; j++){
            outputWeights[i][j] = init_weights();
        }
    }

    for(int i = 0; i < numOutput; i++){//random output's bias
        outputLayersBias[i] = init_weights();
    }

    for(int i = 0; i < hiddenLayers_num; i++){//random hidden's bias
        for(int j = 0; j < hiddenNode_n; j++)
            hiddenLayersBias[i][j] = init_weights();
    }

    printf("\nHidden weight\n");
    for(int i = 0; i < hiddenLayers_num; i++){
        if(i!= 0){
            printf("hidden layer %d weight\n",i+1);
            for(int j = 0; j < hiddenNode_n; j++){
                printf("[");
                for(int k = 0; k < hiddenNode_n; k++){
                    printf("%9.6f ",hiddenWeights[i][j][k]);
                }
            printf("]");
            }
        printf("\n");
        }else if(i == 0){
            printf("first hidden layer weight\n");
            for(int j = 0; j < hiddenNode_n; j++){
                printf("[");
                for(int k = 0; k < feature_num; k++){
                    printf("%9.6f ",hiddenWeights[i][j][k]);
                }
            printf("]");
            }
            printf("\n");
        }
    }

    printf("\nOutput weight\n");
    for(int j = 0; j < hiddenNode_n; j++){
        for(int k = 0; k < numOutput; k++){
            printf("[%f] ",outputWeights[j][k]);
        }
    }printf("\n");

    printf("\nHidden Bias\n");
    for(int i = 0; i < hiddenLayers_num; i++){
        for(int j = 0; j < hiddenNode_n; j++){
            printf("[%9.6f] ",hiddenLayersBias[i][j]);
        }
        printf("\n");

    }

    printf("\nOutput Bias\n");
    for(int j = 0; j < numOutput; j++){
        printf("[%f] ",outputLayersBias[j]);
    }

    free(HiddenNode_num);//free allocated memory

    return 0;
}

int main(){
    train_test_split();
    return 0;
}
