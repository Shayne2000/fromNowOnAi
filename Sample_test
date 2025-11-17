#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SELECTION_SIZE 100
#define MAX_COL_NAME_LEN 300

typedef struct {
    float feature_values[SELECTION_SIZE]; 
    float label_value;                   
} DataRow;

DataRow *all_data = NULL;
int row_num = 0;
int feature_n = 0;
char selected_features[SELECTION_SIZE][MAX_COL_NAME_LEN];
char selected_label[MAX_COL_NAME_LEN];

int main() {

    feature_n = 3; // เปลี่ยนได้เลย
    row_num = 5;   // เปลี่ยนได้เลย

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

   
    free(all_data);

    return 0;
}
