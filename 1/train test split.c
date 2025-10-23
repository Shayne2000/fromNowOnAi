#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define numInput 2
#define numHiddenNodes 4
#define numOutput 1

#define Train_Ratio 0.8
#define Sample_Num 10
#define Train_Size ((int)(Sample_Num * Train_Ratio))
#define Test_Size (Sample_Num - Train_Size)
#define features_num 2

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
    float data[Sample_Num][features_num];
    float labels[Sample_Num];

    float train_data[Train_Size][features_num];
    float test_data[Test_Size][features_num];

    float train_labels[Train_Size];
    float test_labels[Test_Size];

    for (int i = 0; i < Sample_Num; i++) {
        for (int j = 0; j < features_num; j++) {
            data[i][j]= all_data[i].feature_values[j];//Collect data from selected features
        }
        labels[i] = all_data[i].label_value;//Collect data from selected labels
    }

    int index[Sample_Num];
    for(int i=0; i<Sample_Num; i++){
        index[i] = i;
    }
    shuffle(index, Sample_Num);

    for (int i=0; i < Train_Size; i++) {
        for (int j = 0; j < features_num; j++) {
            train_data[i][j] = data[index[i]][j];
        }
        train_labels[i] = labels[index[i]];
    }

    for (int i=0; i < Test_Size; i++) {
        for (int j = 0; j < features_num; j++) {
            test_data[i][j] = data[index[i + Train_Size]][j];
        }
        test_labels[i] = labels[index[i + Train_Size]];
    }



    double hiddenLayers[numHiddenNodes];
    double outputLayers[numOutput];

    double hiddenLayersBias[numHiddenNodes];
    double outputLayersBias[numOutput];

    double hiddenWeights[numInput][numHiddenNodes];
    double outputWeights[numHiddenNodes][numOutput];

    for(int i=0; i < numInput; i++){//random input's weight
        for(int j=0; j < numHiddenNodes; j++){
            hiddenWeights[i][j] = init_weights();
        }
    }

    for(int i=0; i < numHiddenNodes; i++){//random output's weight
        for(int j=0; j < numOutput; j++){
            outputWeights[i][j] = init_weights();
        }
    }

    for(int i = 0; i < numOutput; i++){//random output's bias
        outputLayersBias[i] = init_weights();
    }

    for(int i = 0; i < numHiddenNodes; i++){//random hidden's bias
        hiddenLayersBias[i] = init_weights();
    }

    printf("\nFinal Hidden weight\n");
    for(int j=0; j<numHiddenNodes; j++){
        printf("[");
        for(int k=0; k < numInput; k++){
            printf("%f ",hiddenWeights[k][j]);
        }
        printf("]");
    }

    printf("\nFinal Output weight\n");
    for(int j=0; j<numOutput; j++){
        printf("[");
        for(int k=0; k < numHiddenNodes; k++){
            printf("%f ",outputWeights[k][j]);
        }
        printf("]");
    }

    printf("\nFinal Hidden Bias\n");
    for(int j=0; j<numHiddenNodes; j++){
        printf("[");
        printf("%f ",hiddenLayersBias[j]);
        printf("]");
    }

    printf("\nFinal Output Bias\n");
    for(int j=0; j<numOutput; j++){
        printf("[");
        printf("%f ",outputLayersBias[j]);
        printf("]");
    }

    return 0;
}
