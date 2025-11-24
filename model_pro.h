#ifndef MODEL_H
#include <stdio.h>
#define SELECTION_SIZE 100
#define MAX_COL_NAME_LEN 300
typedef struct {
    float feature_values[SELECTION_SIZE]; // Values of the selected features
    float label_value;                    // Value of the selected label
} DataRow;

int feature_n = 3;                       // Counter for the number of selected features


// variables for data storage and indices
int row_num = 5;      // Total number of rows
DataRow *all_data = NULL;  // Pointer to hold the array of DataRow (Actual data)

                 // 0-based index of the selected label



extern char selected_features[SELECTION_SIZE][MAX_COL_NAME_LEN];
extern char selected_label[MAX_COL_NAME_LEN];



#endif

