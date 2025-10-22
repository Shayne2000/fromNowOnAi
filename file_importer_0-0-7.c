#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLS 10           
#define MAX_COL_NAME_LEN 30   
#define SELECTION_SIZE 10     
#define MAX_ROW_LENGTH 250   
#define MAX_VAL_LEN 50       
    
// โครงสร้างสำหรับเก็บข้อมูลของแต่ละแถว (Data Structure to store actual values)
typedef struct {
    char feature_values[SELECTION_SIZE][MAX_VAL_LEN]; // Values of the selected features
    char label_value[MAX_VAL_LEN];                    // Value of the selected label
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
int row_num = 0;           // Total number of data rows
DataRow *all_data = NULL;  // Pointer to hold the array of DataRow (Actual data)
int selected_feature_indices[SELECTION_SIZE]; // 0-based index of selected features
int selected_label_index = -1;                // 0-based index of the selected label


/**
 * @brief Splits the first row (header) of the CSV file into individual column names.
 * @param first_row The string containing the first row of the CSV.
 */
void split_first_row(const char *first_row) {
    char temp_row[MAX_ROW_LENGTH];
    // Copy the row to a temporary variable for modification (strtok)
    strncpy(temp_row, first_row, sizeof(temp_row) - 1);
    temp_row[sizeof(temp_row) - 1] = '\0';
    
    // Remove trailing newline or carriage return characters
    temp_row[strcspn(temp_row, "\r\n")] = '\0';

    char *token = strtok(temp_row, ",");
    num_columns = 0;
    while (token != NULL && num_columns < MAX_COLS) {
        // Check and remove surrounding quotes if present (e.g., "Student_ID (1)")
        size_t len = strlen(token);
        if (len > 0 && token[0] == '"' && token[len - 1] == '"') {
            // Copy content without quotes
            strncpy(all_column_names[num_columns], token + 1, len - 2);
            all_column_names[num_columns][len - 2] = '\0';
        } else {
            // Copy content as is
            strncpy(all_column_names[num_columns], token, len);
            all_column_names[num_columns][len] = '\0';
        }
        num_columns++;
        token = strtok(NULL, ",");
    }
}

/**
 * @brief Displays the available features for selection, including their index.
 */
void display_features() {
    printf("\nAvailable Columns:\n");
    for (int i = 0; i < num_columns; i++) {
        printf("%s (%d) ", all_column_names[i], i + 1); // Display column name and its 1-based index
    }
    printf("\n");
}

/**
 * @brief Counts the total number of rows in the CSV file, excluding the header.
 * @param filename The name of the CSV file.
 * @return The number of data rows.
 */
int count_rows_in_csv(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0; // Return 0 if file cannot be opened
    }

    int count = 0;
    char line[MAX_ROW_LENGTH];
    
    // Read and discard the header row
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return 0; // File is empty or only has header
    }

    // Count the rest of the rows (data rows)
    while (fgets(line, sizeof(line), file) != NULL) {
        // Check if the line is not just a newline/empty line
        if (strspn(line, " \t\n\r") != strlen(line)) {
            count++;
        }
    }

    fclose(file);
    return count;
}

/**
 * @brief Finds the 0-based index of a column name.
 * @param col_name The name of the column to find.
 * @return The 0-based index of the column, or -1 if not found.
 */
int get_column_index(const char *col_name) {
    for (int i = 0; i < num_columns; i++) {
        if (strcmp(all_column_names[i], col_name) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Reads the actual data values from the CSV file for the selected features and label.
 * @return 0 on success, 1 on failure.
 */
int read_selected_data() {
    // 1. Get 0-based indices for all selected columns
    for (int i = 0; i < feature_n; i++) {
        selected_feature_indices[i] = get_column_index(selected_features[i]);
    }
    selected_label_index = get_column_index(selected_label);

    // 2. Allocate memory for the data array
    all_data = (DataRow *)malloc(row_num * sizeof(DataRow));
    if (all_data == NULL) {
        printf("Error: Memory allocation failed for data storage!\n");
        return 1;
    }

    // 3. Re-open the file and read data
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error: Could not re-open file %s.\n", file_name);
        free(all_data); // Clean up allocated memory
        all_data = NULL;
        return 1;
    }

    char line[MAX_ROW_LENGTH];
    fgets(line, sizeof(line), file); // Skip the header row

    for (int row = 0; row < row_num && fgets(line, sizeof(line), file) != NULL; row++) {
        // Create a copy of the line because strtok modifies the string
        char *temp_line = strdup(line); 
        char *token;
        int col_index = 0;
        int feature_count = 0;
        
        // Remove trailing newline or carriage return characters from the end of the line
        temp_line[strcspn(temp_line, "\r\n")] = '\0';
        
        // Start tokenizing the line
        token = strtok(temp_line, ",");
        while (token != NULL && col_index < num_columns) {

            // Check if current column (col_index) matches any selected feature index
            for (int i = 0; i < feature_n; i++) {
                if (col_index == selected_feature_indices[i]) {
                    // Store the feature value
                    strncpy(all_data[row].feature_values[feature_count], token, MAX_VAL_LEN - 1);
                    all_data[row].feature_values[feature_count][MAX_VAL_LEN - 1] = '\0';
                    feature_count++;
                    break; 
                }
            }

            // Check if current column (col_index) matches the selected label index
            if (col_index == selected_label_index) {
                // Store the label value
                strncpy(all_data[row].label_value, token, MAX_VAL_LEN - 1);
                all_data[row].label_value[MAX_VAL_LEN - 1] = '\0';
            }

            col_index++;
            token = strtok(NULL, ",");
        }
        free(temp_line); // Free the memory allocated by strdup
    }

    fclose(file);
    return 0; // Success
}


int main(void) {
    system("cls"); // Clear console

    // --- Part 1: File Input and Validation ---
    while (1) {
        printf("Input your file name (Must be .csv file): ");
        fgets(file_name, sizeof(file_name), stdin);
        file_name[strcspn(file_name, "\n")] = '\0'; 
        system("cls"); 

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

            row_num = count_rows_in_csv(file_name);

            system("cls"); 
            printf("==============================================\n");
            printf("File \"%s\" opened successfully!\n", file_name);
            printf("Total data rows (excluding header): %d\n", row_num); 
            printf("==============================================\n\n");
            break; 
        }
    }
    
    // --- Part 2: Feature Selection ---
    int selection = -1;
    printf("\n===================================================================\n");
    printf("Select features for input (Feature). Input 0 to stop selecting features.\n");
    printf("===================================================================\n");

    while (1) {
        display_features(); 

        printf("\nSelected Features (%d): ", feature_n);
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
                // Check if the feature is already selected
                int already_selected = 0;
                for(int i = 0; i < feature_n; i++) {
                    if (strcmp(selected_features[i], all_column_names[selection - 1]) == 0) {
                        already_selected = 1;
                        break;
                    }
                }

                if (already_selected) {
                    printf("Feature %s is already selected. Please choose another.\n", all_column_names[selection - 1]);
                } else {
                    strncpy(selected_features[feature_n], all_column_names[selection - 1], MAX_COL_NAME_LEN - 1);
                    selected_features[feature_n][MAX_COL_NAME_LEN - 1] = '\0';
                    feature_n++;
                    printf("Added feature: %s\n", all_column_names[selection - 1]);
                }
            } else {
                printf("Maximum number of features (%d) reached.\n", SELECTION_SIZE);
            }
        } else {
            printf("Invalid selection. Please choose a number between 1 and %d, or 0 to stop.\n", num_columns);
        }
    }

    // --- Part 3: Label Selection ---
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
            for (int i = 0; i < feature_n; i++) {
                if (strcmp(selected_features[i], all_column_names[label_selection - 1]) == 0) {
                    is_feature = 1;
                    break;
                }
            }
            
            if (is_feature) {
                printf("Error: The label cannot be one of the selected features. Please choose again.\n");
            } else {
                strncpy(selected_label, all_column_names[label_selection - 1], MAX_COL_NAME_LEN - 1);
                selected_label[MAX_COL_NAME_LEN - 1] = '\0';
                printf("Selected label: %s\n", selected_label);
                break; 
            }
        } else {
            printf("Invalid selection. Please choose a number between 1 and %d.\n", num_columns);
        }
    }
    
    // --- Part 4: Read Actual Data (New/Modified Part) ---
    printf("\n==============================================\n");
    printf("Reading actual data for selected columns...\n");
    printf("==============================================\n");

    if (read_selected_data() != 0) {
        printf("Data reading failed. Exiting.\n");
        return 1;
    }
    printf("Data successfully loaded into memory.\n");
    
    // --- Part 5: Display Results ---
    printf("\n==============================================\n");
    printf("Summary of Selections (Names and Values):\n");
    printf("==============================================\n");
    
    printf("feature_n = %d\n", feature_n); 
    printf("label[] = { %s }\n", selected_label); 
    printf("row_num = %d\n", row_num); 
    
    // Display actual data values for the first few rows
    printf("\nDisplaying first 3 rows of data (for verification):\n");
    int display_rows = (row_num > 3) ? 3 : row_num;
    
    for (int r = 0; r < display_rows; r++) {
        printf("Row %d:\n", r + 1);
        
        // Display features
        printf("  Features: {");
        for (int i = 0; i < feature_n; i++) {
            printf("%s: \"%s\"", selected_features[i], all_data[r].feature_values[i]);
            if (i < feature_n - 1) {
                printf(", ");
            }
        }
        printf("}\n");

        // Display label
        printf("  Label:    {%s: \"%s\"}\n", selected_label, all_data[r].label_value);
    }

    // --- Part 6: Cleanup ---
    if (all_data != NULL) {
        free(all_data); // Crucial: Release the dynamically allocated memory
        all_data = NULL;
    }
    
    return 0;
}

// ------------------------------------------- END part 1 ------------------------------------------- //





