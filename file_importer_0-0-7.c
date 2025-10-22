#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For isspace

#define MAX_COLS 10           // Maximum number of columns expected
#define MAX_COL_NAME_LEN 30   // Maximum length of a column name
#define SELECTION_SIZE 10     // Maximum number of features that can be selected
#define MAX_ROW_LENGTH 250    // Maximum expected length of a single row

typedef struct {
    float feature_values[SELECTION_SIZE]; // Values of the selected features
    float label_value;                    // Value of the selected label
} DataRow;

// Global variable for file name
char file_name[100];

// Global variables to store column names
char all_column_names[MAX_COLS][MAX_COL_NAME_LEN];
int num_columns = 0;

// Global variables for selected features and label names
char selected_features[SELECTION_SIZE][MAX_COL_NAME_LEN];
char selected_label[MAX_COL_NAME_LEN];
int feature_n = 0; // Counter for the number of selected features

// New global variables for data storage and indices
int row_num = 0;      // Total number of data rows
DataRow *all_data = NULL;  // Pointer to hold the array of DataRow (Actual data)
int selected_feature_indices[SELECTION_SIZE]; // 0-based index of selected features
int selected_label_index = -1;                 // 0-based index of the selected label


// ---------------------- UTILITY FUNCTIONS ----------------------

// Function to clear the console in a cross-platform way (basic approach)
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

// ---------------------- FUNCTION DEFINITIONS ----------------------

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

void display_features() {
    printf("\nAvailable Columns:\n");
    for (int i = 0; i < num_columns; i++) {
        printf("%s (%d) ", all_column_names[i], i + 1);
    }
    printf("\n");
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

int get_column_index(const char *col_name) {
    for (int i = 0; i < num_columns; i++) {
        if (strcmp(all_column_names[i], col_name) == 0) {
            return i;
        }
    }
    return -1;
}


int read_selected_data() {
    for (int i = 0; i < feature_n; i++) {
        selected_feature_indices[i] = get_column_index(selected_features[i]);
        if (selected_feature_indices[i] == -1) {
             printf("Error: Feature '%s' index not found!\n", selected_features[i]);
             return 1;
        }
    }
    selected_label_index = get_column_index(selected_label);
    if (selected_label_index == -1) {
        printf("Error: Label '%s' index not found!\n", selected_label);
        return 1;
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
        return 1;
    }

    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s.\n", file_name);
        free(all_data);
        all_data = NULL;
        return 1;
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
    return 0;
}


// ---------------------- MAIN FUNCTION ----------------------

int main(void) {
    clear_console(); 

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

        if (strstr(file_name, ".csv") == NULL) {
            printf("==============================================\n");
            printf("Invalid file type. Please enter (.csv) file.\n");
            printf("==============================================\n");
            continue;
        }

        FILE *import_file = fopen(file_name, "r");
        if (import_file == NULL) {
            printf("==============================================\n");
            printf("There is no file named: %s\nPlease input again.\n", file_name);
            printf("==============================================\n");
            continue;
        } else {
            char first_row[MAX_ROW_LENGTH];
            if (fgets(first_row, sizeof(first_row), import_file) != NULL) {
                split_first_row(first_row);
            }
            fclose(import_file); 

            if (num_columns == 0) {
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
    

    // --- Feature Selection ---

    int selection = -1;
    printf("\n===================================================================\n");
    printf("Select features for input (Feature). Input 0 to stop selecting features.\n");
    printf("===================================================================\n");

    while (1) {
        display_features(); 
        printf("\nSelected Features (%d/%d): ", feature_n, SELECTION_SIZE);
        for(int i = 0; i < feature_n; i++) {
            printf("%s ", selected_features[i]);
        }
        printf("\nSelect feature # (1-%d, 0 to stop): ", num_columns);

        if (scanf("%d", &selection) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); 
            continue;
        }
        while (getchar() != '\n'); 

        if (selection == 0) {
            if (feature_n == 0) {
                printf("You must select at least one feature.\n");
                continue;
            }
            break; 
        } else if (selection >= 1 && selection <= num_columns) {
            if (feature_n < SELECTION_SIZE) {
                int already_selected = 0;
                for(int i = 0; i < feature_n; i++) {
                    if (strcmp(selected_features[i], all_column_names[selection - 1]) == 0) {
                        already_selected = 1;
                        break;
                    }
                }

                if (already_selected) {
                    printf("Feature %s is already selected.\n", all_column_names[selection - 1]);
                } else {
                    // Check if selection is the same as the current label
                    if (strcmp(all_column_names[selection - 1], selected_label) == 0) {
                        printf("Error: %s is already selected as the Label. Please select a different column.\n", all_column_names[selection - 1]);
                        continue;
                    }
                    
                    strncpy(selected_features[feature_n], all_column_names[selection - 1], MAX_COL_NAME_LEN - 1);
                    selected_features[feature_n][MAX_COL_NAME_LEN - 1] = '\0';
                    feature_n++;
                    printf("Added feature: %s\n", all_column_names[selection - 1]);
                }
            } else {
                printf("Maximum number of features (%d) reached.\n", SELECTION_SIZE);
            }
        } else {
            printf("Invalid selection.\n");
        }
    }


    // --- Label Selection ---

    printf("\n===================================================================\n");
    printf("Select the target variable (Label).\n");
    printf("===================================================================\n");

    int label_selection = -1;
    while (1) {
        display_features();
        printf("\nSelect label # (1-%d): ", num_columns);

        if (scanf("%d", &label_selection) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        if (label_selection >= 1 && label_selection <= num_columns) {
            int is_feature = 0;
            const char *potential_label = all_column_names[label_selection - 1];
            
            // Check if the selected column is already one of the features
            for (int i = 0; i < feature_n; i++) {
                if (strcmp(selected_features[i], potential_label) == 0) {
                    is_feature = 1;
                    break;
                }
            }
            
            if (is_feature) {
                printf("Error: The label **cannot** be one of the selected features (%s).\n", potential_label);
            } else {
                strncpy(selected_label, potential_label, MAX_COL_NAME_LEN - 1);
                selected_label[MAX_COL_NAME_LEN - 1] = '\0';
                printf("Selected label: %s\n", selected_label);
                break; 
            }
        } else {
            printf("Invalid selection.\n");
        }
    }
    

    // --- Read Actual Data ---

    printf("\n==============================================\n");
    printf("Reading actual data for selected columns...\n");
    printf("==============================================\n");

    if (read_selected_data() != 0) {
        printf("Data reading failed. Exiting.\n");
        if (all_data != NULL) free(all_data);
        return 1;
    }
    printf("Data successfully loaded into memory.\n");

    // --- Display Results ---

    printf("\n==============================================\n");
    printf("Summary of Selections (Names and Values):\n");
    printf("==============================================\n");
    printf("Features Selected: %d\n", feature_n); 
    printf("Label: %s\n", selected_label); 
    printf("Total Data Rows: %d\n", row_num); 
    
    printf("\nDisplaying first 3 rows of data (for verification):\n");
    int display_rows = (row_num > 3) ? 3 : row_num;
    
    if (row_num > 0) {
        for (int r = 0; r < display_rows; r++) {
            printf("Row %d:\n", r + 1);
            printf("  Features: {");
            for (int i = 0; i < feature_n; i++) {
                printf("%s: %.3f", selected_features[i], all_data[r].feature_values[i]);
                if (i < feature_n - 1) printf(", ");
            }
            printf("}\n");
            printf("  Label:    {%s: %.3f}\n", selected_label, all_data[r].label_value);
        }
    } else {
         printf("No data rows to display.\n");
    }


    // --- Cleanup ---

    if (all_data != NULL) {
        free(all_data);
        all_data = NULL;
    }
    
    return 0;
}

// ------------------------------------------- END part 1 ------------------------------------------- //





