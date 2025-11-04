#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For isspace


///////////////////////////////  declare variables //////////////////////


#define MAX_COLS 100           // Maximum number of columns expected
#define MAX_COL_NAME_LEN 300   // Maximum length of a column name
#define SELECTION_SIZE 100     // Maximum number of features that can be selected
#define MAX_ROW_LENGTH 250000  // Maximum expected length of a single row
#define FILE_NAME_LEN 100      // Maximum expected length file name


typedef struct {
    float feature_values[SELECTION_SIZE]; // Values of the selected features
    float label_value;                    // Value of the selected label
} DataRow;


char file_name[FILE_NAME_LEN];// Global variable for file name


// Global variables to store column names
char all_column_names[MAX_COLS][MAX_COL_NAME_LEN];
int num_columns = 0;


char selected_features[SELECTION_SIZE][MAX_COL_NAME_LEN]; //kepp name of features to select as x     max_row_lenght ?
char selected_label[MAX_COL_NAME_LEN];                    //keep naem of label to sclect as y
int feature_n = 0;                       // Counter for the number of selected features


// variables for data storage and indices
int row_num = 0;      // Total number of rows
DataRow *all_data = NULL;  // Pointer to hold the array of DataRow (Actual data)
int selected_feature_indices[SELECTION_SIZE]; // 0-based index of selected features
int selected_label_index = -1;                 // 0-based index of the selected label


/////////////////////////////// functions ////////////////////////////////


int test () {
    for (int i = 0 ; i < 10 ; i ++) {
        printf("test");
    }

    return 0;
}

void display_features() {
    printf("\nAvailable Columns:\n");
    for (int i = 0; i < num_columns; i++) {
        printf("%s (%d) ", all_column_names[i], i + 1);
    }
    printf("\n");
}

void split_first_row(const char *first_row) {
    char temp_row[MAX_ROW_LENGTH];
    strncpy(temp_row, first_row, sizeof(temp_row) - 1);
    temp_row[sizeof(temp_row) - 1] = '\0';
    
    // Replace newline/carriage return with null terminator
    temp_row[strcspn(temp_row, "\r\n")] = '\0';

    char *token = strtok(temp_row, ",");
    num_columns = 0;
    while (token != NULL && num_columns < MAX_COLS) {
        char *trimmed_token = trim_whitespace(token); // Trim token
        size_t len = strlen(trimmed_token);

        // Handle quoted strings (e.g., "column name")
        if (len > 0 && trimmed_token[0] == '"' && trimmed_token[len - 1] == '"') {
            strncpy(all_column_names[num_columns], trimmed_token + 1, len - 2);
            all_column_names[num_columns][len - 2] = '\0';
        } else {
            strncpy(all_column_names[num_columns], trimmed_token, len);
            all_column_names[num_columns][len] = '\0';
        }
        num_columns++;
        token = strtok(NULL, ",");
    }
}

int count_rows_in_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return 0;

    int count = 0;
    char line[MAX_ROW_LENGTH];
    
    // Skip header
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return 0;
    }

    // Count non-empty data rows
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strspn(line, " \t\n\r") != strlen(line)) {
            // Basic check to skip completely empty lines
            count++;
        }
    }

    fclose(file);
    return count;
}


void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    // ANSI escape code for clearing console
    printf("\033[2J\033[1;1H");
#endif
}

// Function to trim leading/trailing whitespace and newlines (optional but good practice)
char *trim_whitespace(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

int get_column_index(const char *col_name) {
    for (int i = 0; i < num_columns; i++) {
        if (strcmp(all_column_names[i], col_name) == 0) {
            return i;
        }
    }
    return -1;
}


int main() {

    clear_console(); 

///////////////////////// import file ////////////////////


while (1) {
        printf("Input your file name (Must be .csv file): ");
        if (fgets(file_name, sizeof(file_name), stdin) == NULL) { 
            printf("\nInput error or EOF. Exiting.\n");
            return 1;
        }
        file_name[strcspn(file_name, "\n")] = '\0'; 
        clear_console(); 

        // Trim any trailing carriage return leftover from fgets
        size_t len = strlen(file_name);
        if (len > 0 && file_name[len-1] == '\r') {
             file_name[len-1] = '\0';
        }

        if (strstr(file_name, ".csv") == NULL) { //have .cav in file name
            printf("==============================================\n");
            printf("Invalid file type. Please enter (.csv) file.\n");
            printf("==============================================\n");
            continue;
        }

        FILE *import_file = fopen(file_name, "r"); //read file
        if (import_file == NULL) {
            printf("==============================================\n");
            printf("There is no file named: %s\nPlease input again.\n", file_name);
            printf("==============================================\n");
            continue;
        } else {                               /// can read file now
            char first_row[MAX_ROW_LENGTH];
            if (fgets(first_row, sizeof(first_row), import_file) != NULL) {
                split_first_row(first_row);
            }
            fclose(import_file);               /// dont close file that opened

            if (num_columns == 0) {    // num_column got value from split_first_row function
                 printf("==============================================\n");
                 printf("Error: Could not parse columns from header row.\n");
                 printf("==============================================\n");
                 continue;
            }

            row_num = count_rows_in_csv(file_name);
            clear_console(); 
            printf("==============================================\n");
            printf("File \"%s\" opened successfully!\n", file_name);
            printf("Total data rows (excluding header): %d\n", row_num); 
            printf("Available columns: %d\n", num_columns);
            printf("==============================================\n\n");
            break; 
        }
    }



// --- Feature Selection ---------------------------------------------------



    int selection = -1; 
    printf("\n===================================================================\n");
    printf("Select features for input (Feature). Input 0 to stop selecting features.\n");
    printf("===================================================================\n");

    while (1) {
        display_features();
        printf("\nSelected Features (%d/%d): ", feature_n, SELECTION_SIZE); 
        for(int i = 0; i < feature_n; i++) {  // if not yet selected then don't show any
            printf("%s ", selected_features[i]);
        }
        printf("\nSelect feature # (1-%d, 0 to stop): ", num_columns); //

        if (scanf("%d", &selection) != 1) {  // if scan -- > not true     ทำไมไม่ getchar()
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        if (selection == 0) {   // finish select features case
            if (feature_n == 0) { // not select any features yet
                printf("You must select at least one feature.\n");
                continue;
            }
            break;  // if already select atleast 1 feature, than go to next step



            // select something and within range that can be selected
        } else if (selection >= 1 && selection <= num_columns) {
            if (feature_n < SELECTION_SIZE) {  // not lager than store capasity   ( feature_n + 1 ) ?
                int already_selected = 0; //boolean
                for(int i = 0; i < feature_n; i++) {   // loop in features that already selected
                    if (strcmp(selected_features[i], all_column_names[selection - 1]) == 0) {  // if match what's adready selected
                        already_selected = 1;
                        printf("Feature %s is already selected.\n", all_column_names[selection - 1]);
                        break;  //break from already-selected-features loop
                    }
                }

                if (!already_selected) { // confirm to be a new feature
                    clear_console();

                    /////// save a column as a feature ///////// (not yet add value)

                    strncpy(selected_features[feature_n], all_column_names[selection - 1], MAX_COL_NAME_LEN - 1); // replaced next value of sclected-feature with the feature that just selected
                    selected_features[feature_n][MAX_COL_NAME_LEN - 1] = '\0'; // make the array a string
                    feature_n++;
                    printf("Added feature: %s\n", all_column_names[selection - 1]);

                    ////////////////////////////////////////////
                }
            } else {
                printf("Maximum number of features (%d) reached.\n", SELECTION_SIZE);
            }
        } else { // more than what can be selected
            printf("Invalid selection.\n");
        }
    }
    

    // --- Label Selection ---------------------------------------------------


    clear_console();
    printf("\n========================================\n");
    printf("Select the target variable (Label).\n");
    printf("==========================================\n");

    selection = -1; // to collect index of column to check    can use same variable?
    while (1) {
        
        display_features();
        printf("\nSelect label # (1-%d): ", num_columns);

        if (scanf("%d", &selection) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        

        ////// after scan ////////


        if (selection >= 1 && selection <= num_columns) { // if select within range
            int is_feature = 0;
            const char *potential_label = all_column_names[selection - 1]; //define column to sclect
            
            // Check if the selected column is already one of the features
            for (int i = 0; i < feature_n; i++) {
                if (strcmp(selected_features[i], potential_label) == 0) {
                    is_feature = 1;
                    printf("Error: The label **cannot** be one of the selected features (%s).\n", potential_label);
                    break; // out of search-selected-columns loop
                }
            }
            
            if (!is_feature) {
                strncpy(selected_label, potential_label, MAX_COL_NAME_LEN - 1); //replace selected_label with potensial_label (save)
                selected_label[MAX_COL_NAME_LEN - 1] = '\0'; // is a string
                printf("Selected label: %s\n", selected_label);
                break;  // continue to next stage
            }
        } else {
            printf("Invalid selection.\n");
        }
    }


    // --- Read Actual Data ---
    clear_console();
    printf("\n==============================================\n");
    printf("Reading actual data for selected columns...\n");
    printf("==============================================\n");



    int error = 0;


    for (int i = 0; i < feature_n; i++) { //loop in features
        selected_feature_indices[i] = get_column_index(selected_features[i]); //find index of features-column in all column
        if (selected_feature_indices[i] == -1) {
             printf("Error: Feature '%s' index not found!\n", selected_features[i]);
             error = 1;
        }
    }
    selected_label_index = get_column_index(selected_label);
    if (selected_label_index == -1) {
        printf("Error: Label '%s' index not found!\n", selected_label);
        error = 1;
    }

    // Free previous allocation if exists
    if (all_data != NULL) {
        free(all_data);
        all_data = NULL;
    }
    
    // Allocate memory for data
    all_data = (DataRow *)malloc(row_num * sizeof(DataRow));
    if (all_data == NULL) {
        printf("Error: Memory allocation failed!\n");
        error = 1;
    }

    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s.\n", file_name);
        free(all_data);
        all_data = NULL;
        error = 1;
    }

    char line[MAX_ROW_LENGTH];
    fgets(line, sizeof(line), file); // Skip header

    for (int row = 0; row < row_num && fgets(line, sizeof(line), file) != NULL; row++) {

        char temp_line_buffer[MAX_ROW_LENGTH]; 
        strncpy(temp_line_buffer, line, sizeof(temp_line_buffer) - 1);
        temp_line_buffer[sizeof(temp_line_buffer) - 1] = '\0';
        temp_line_buffer[strcspn(temp_line_buffer, "\r\n")] = '\0'; // Remove newline

        char *token;
        int col_index = 0;
        int feature_count = 0;
        
        token = strtok(temp_line_buffer, ","); 
        
        while (token != NULL && col_index < num_columns) {
            // Trim whitespace from token
            char *trimmed_token = trim_whitespace(token);

            // Features
            for (int i = 0; i < feature_n; i++) {
                if (col_index == selected_feature_indices[i]) {
                    all_data[row].feature_values[feature_count] = atof(trimmed_token);
                    feature_count++;
                    break; 
                }
            }

            // Label
            if (col_index == selected_label_index) {
                all_data[row].label_value = atof(trimmed_token);
            }

            col_index++;
            token = strtok(NULL, ","); 
        }
        

        if (feature_count != feature_n) {
             printf("Warning: Row %d has missing data for selected features.\n", row + 1);
        }
    }

    fclose(file);
    error = 0;










    if (error != 0) {
        printf("Data reading failed? Exiting.\n");
        if (all_data != NULL) free(all_data);
        return 1;
    }
    printf("Data successfully loaded into memory.\n");


/////////////// read file, show to user and make user select (feture,row,label) ////////// output --> features,label,features_num ////////



int layers = 1 ; //get from user input

int (*functions_pointer[layers])() ;

functions_pointer[0] = &test; //loop trought user input

// printf("%d",functions_pointer[0]());


///////////// train test split ///////////





////////////// main program /////////////


int adjust_times = 1; // have to get user input first -----> do it later

int dimention[1] = {1}; // set of nodes inputed from user   [3,4,1]
float features_train[1][1] = {{1}}; //get from csv's data   
float weights[1][1] = {{1}}; //from random
float bias[1][1] = {{1}}; //from random

float sum ; //declare here is fine
float weight_adjust_record[1][1][1] = {{{0}}}; //fix dimention
float bias_adjust_record[1][1][1] = {{{0}}}; //fix dimention


for (int adjust_time_count = 0 ; adjust_time_count < adjust_times ; adjust_time_count ++) {

    for (int row = 0 ; row < row_num ; row ++ ) {

        for (int layer_num = 0 ; layer_num < layers ; layer_num ++ ) {

            for (int node_num = 0 ; node_num < dimention[layer_num] ; node_num++){
                sum = 0;
                if (layer_num == 0) {
                    for (int previous_node = 0 ; previous_node < input_num ; previous_node++ ) {
                        sum += input[p] * weights[layer_num][node_num*2+previous_node] ;
                        keep[layer_num][node_num*2+previous_node] = input[previous_node] ;
                    }
                    
                }else{

                    for (int previous_node = 0 ; previous_node < dimention[layer_num-1]; previous_node ++ ) {
                    
                        sum += input[p] * weights[layer_num][node_num*2+previous_node] ;
                        keep[layer_num][node_num*2+previous_node]
                    }
                }
                
                sum = (*functions_pointer[layer_num])(sum) ;
                pre2[node] = sum ;
                
            }

            pre1[0] = pre2[0] ;
            pre1[1] = pre2[1] ; 

        }

        loss(sum,row);

        weight_adjust_record[][][] += (dLoss/dweight)/row_num ;
        bias_adjust_record[][][] += (dloss/dbias)/row_num ;


        float dzdw,dzdz ;

        for (int layer_num = layers ; layer_num >= 0 ; layer_num --) {
            for (int furter_node = dimention[layer_num]-1 ; furter_node >= 0 ; furter_node --) {
                for (int closer_node = dimention[layer_num-1]-1 ; closer_node >= 0 ; closer_node--) {
                    dzdw = dzdw_function;
                    dzdz = dzdz_function;

                    keep[layer_num][furter_node][closer_node] = dzdz * keep[w_layer+1][furter_node][closer_node] ;
                }
            }
        }


    }

    weights[][][] += weight_adjust_record[][][];
    bias[][][] += bias_adjust_record[][][];

}






///////////// output ////////////////////

return 0;



}


