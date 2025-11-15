#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define feature_num 2 //according to feature_num from part 1
#define numOutput 1
#define Max_node 10

#define Train_Ratio 0.8
#define row_Num 10 //row_num from part 1
#define Train_Size ((int)(row_Num * Train_Ratio))
#define Test_Size (row_Num - Train_Size)

float init_weights(){
    return((float)rand()) / ((float)RAND_MAX)* 2.0 - 1.0;
}

void shuffle(int *array ,int size_training){ //array and size of array
    if(size_training > 1){
        for(int i=0; i<size_training; i++){
            int j = i + (rand() % (size_training-i));  // random between 0 to sizeof the rest than +i to index to swap

            ////// swap ///////
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
            ////////////////////
        }
    }
}

void train_test_split(float data[row_Num][feature_num], float labels[row_Num], float train_data[Train_Size][feature_num], float train_labels[Train_Size],
                      float test_data[Test_Size][feature_num], float test_labels[Test_Size]){
    int index[row_Num];
    for(int i=0; i<row_Num; i++){//shuffle data
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
}
 //-------------------- main -------------------------------------------------------
int main(){
    srand((unsigned int)time(NULL)); //genarate random seed
    int hiddenLayers_num;

    printf("Hidden Layer: ");//ask amount of hidden layer
    scanf("%d", &hiddenLayers_num);
     if (hiddenLayers_num < 1){
        printf("Invalid hidden layer number. Exiting.\n");
        return -1;  //kill the program if hiddenlayer doesn't provied
    }

    int *HiddenNode_num = malloc(hiddenLayers_num * sizeof(int));  //for dimention
    
    for(int i=0; i<hiddenLayers_num; i++){
      printf("Layer %d Number of Hidden Nodes: ",i+1);//ask node of each layer
      scanf("%d", &HiddenNode_num[i]);
       if (HiddenNode_num[i] < 1) {
            printf("Invalid hidden node number in layer %d. Exiting.\n");
            free(HiddenNode_num);
            return -1;
        }
    }
    //------------------------------------------------
    float data[row_Num][feature_num];
    float labels [row_Num];

    /*for (int i=0; i<row_Num; i++) {
        for (int j=0; j<feature_num; j++) {
            data[i][j]= all_data[i].feature_values[j]; //Collect data from selected features (from part 1)
        }
        labels[i] = all_data[i].label_value; //Collect data from selected labels (from part 1)
    }
    */
    //------------------------------------------------
    float train_data[Train_Size][feature_num];//train test split
    float train_labels[Train_Size];
    float test_data[Test_Size][feature_num];
    float test_labels[Test_Size];

    train_test_split(data, labels, train_data, train_labels, test_data, test_labels);
    //---------------------------------------------------------------------------------------
    int total_hidden_nodes = 0; //random hidden bias
    for(int i=0; i<hiddenLayers_num; i++)
        total_hidden_nodes += HiddenNode_num[i];

    float *hiddenLayersBias = (float *)malloc(total_hidden_nodes *sizeof(float));
    for(int i=0; i<total_hidden_nodes; i++){
            hiddenLayersBias[i] = init_weights();
    }
    //---------------------------------------------------------------------------------------------------
    int total_hidden_weight = 0; //random hidden weight
    int weight_index[hiddenLayers_num];
    weight_index[0] = 0;
    int input_size;
    for(int i=0; i<hiddenLayers_num; i++){
        if(i == 0) input_size = feature_num;
        else input_size = HiddenNode_num[i - 1];
        total_hidden_weight += HiddenNode_num[i] * input_size;
    }

    float *hiddenWeights = (float *)malloc(total_hidden_weight *sizeof(float));

    for(int i=1; i<hiddenLayers_num; i++){//determine weight index of each layer
        int prev_layer_input = (i==1) ? feature_num : HiddenNode_num[i-2];
        weight_index[i] = weight_index[i-1] + HiddenNode_num[i-1] * prev_layer_input;
    }
    for(int i=0; i<hiddenLayers_num; i++){
        if(i == 0) input_size = feature_num;
        else input_size = HiddenNode_num[i - 1];
        int index = weight_index[i];
        for(int node=0; node<HiddenNode_num[i]; node++){
            for(int prev=0; prev<input_size; prev++){
                hiddenWeights[index++] = init_weights();
            }
        }
    }
    //-------------------------------------------------------------------------------
    int lastHidden = HiddenNode_num[hiddenLayers_num - 1]; //random output's weight
    int total_output_weights = lastHidden * numOutput;

    float *outputWeights = (float *)malloc(total_output_weights *sizeof(float));

    for(int i=0; i<lastHidden; i++) {
        for (int j=0; j<numOutput; j++){
            int index = i * numOutput + j;
            outputWeights[index] = init_weights();
        }
    }
    //------------------------------------------------------------------------------
    float *outputLayersBias = (float *)malloc(numOutput *sizeof(float));//random output's bias
    for (int i=0; i<numOutput; i++)
        outputLayersBias[i] = init_weights();

    //---------------show output----------------------------------------------------
    printf("\nHidden weight\n");
    printf("[");
    for(int i=0; i<hiddenLayers_num; i++){
        int input_size;
        if(i == 0) input_size = feature_num;
        else input_size = HiddenNode_num[i - 1];
        int index = weight_index[i]; // start index of this layer
        for(int node=0; node<HiddenNode_num[i]; node++){
            for(int prev=0; prev<input_size; prev++){
                printf("%f ", hiddenWeights[index++]);
            }
        }
    }
    printf("]");
    //-------------------------------------------------------------------------------
    printf("\nOutput Weights:\n");
    printf("[");
    for(int i = 0; i < total_output_weights; i++)
        printf("%f ", outputWeights[i]);
    printf("]");
    //-------------------------------------------------------------------------------
    printf("\nHidden Bias\n");
    printf("[");
    int bias_index = 0;
    for (int i=0; i<hiddenLayers_num; i++) {
        for (int j=0; j<HiddenNode_num[i]; j++) {
            printf("%9.6f ", hiddenLayersBias[bias_index++]);
        }
    }
    printf("]");
    //-------------------------------------------------------------------------------
    printf("\nOutput Bias\n");
    printf("[");
    for (int i=0; i<numOutput; i++) {
        printf("%9.6f ", outputLayersBias[i]);
    }
    printf("]");
    printf("\n");
    //-------------------------------------------------------------------------------
    //free allocated memory
    free(HiddenNode_num);
    free(hiddenWeights);
    free(hiddenLayersBias);
    free(outputWeights);
    free(outputLayersBias);
    return 0;
}

