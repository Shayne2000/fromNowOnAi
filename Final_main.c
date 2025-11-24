#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#define MAX_COLS 100		  // Maximum number of columns expected
#define MAX_COL_NAME_LEN 300  // Maximum length of a column name
#define SELECTION_SIZE 100	  // Maximum number of features that can be selected
#define MAX_ROW_LENGTH 250000 // Maximum expected length of a single row
#define FILE_NAME_LEN 100	  // Maximum expected length file name

typedef struct
{
	float feature_values[SELECTION_SIZE]; // Values of the selected features
	float label_value;					  // Value of the selected label
} DataRow;

// Global variable for file name
char file_name[FILE_NAME_LEN];

// Global variables to store column names
char all_column_names[MAX_COLS][MAX_COL_NAME_LEN];
int num_columns = 0;

// Global variables for selected features and label names
char selected_features[SELECTION_SIZE][MAX_COL_NAME_LEN];
char selected_label[MAX_COL_NAME_LEN];
int feature_n = 0; // Counter for the number of selected features (Used as feature_num)

// New global variables for data storage and indices
int row_num = 0;							// Total number of data rows (Used as row_Num)
DataRow *all_data = NULL;					// Pointer to hold the array of DataRow (Actual data)
int selected_feature_index[SELECTION_SIZE]; // 0-based index of selected features
int selected_label_index = -1;				// 0-based index of the selected label

// ---------------------- UTILITY FUNCTIONS (PART 1) ----------------------

// Function to clear the console in a cross-platform way
void clear_console()
{
#ifdef _WIN32
	system("cls");
#else
	// ANSI escape code for clearing console
	printf("\033[2J\033[1;1H");
#endif
}

// Function to trim leading/trailing whitespace and newlines
char *trim_whitespace(char *str)
{
	char *end;
	while (isspace(*str))
		str++;
	if (*str == '\0')
		return str; // check if string " "
	end = str + strlen(str) - 1;
	while (end > str && isspace(*end))
		end--;
	end[1] = '\0';
	return str;
}

// ---------------------- FUNCTION DEFINITIONS (PART 1) ----------------------

void split_first_row(const char *first_row)
{
	char temp_row[MAX_ROW_LENGTH];
	strncpy(temp_row, first_row, sizeof(temp_row) - 1);
	temp_row[sizeof(temp_row) - 1] = '\0';

	// Replace newline return with null terminator
	temp_row[strcspn(temp_row, "\n")] = '\0';

	char *token = strtok(temp_row, ",");
	num_columns = 0;
	while (token != NULL && num_columns < MAX_COLS)
	{
		char *trimmed_token = trim_whitespace(token); // Trim token
		size_t len = strlen(trimmed_token);

		// skiped " ex ("column name")
		if (len > 0 && trimmed_token[0] == '"' && trimmed_token[len - 1] == '"')
		{
			strncpy(all_column_names[num_columns], trimmed_token + 1, len - 2);
			all_column_names[num_columns][len - 2] = '\0';
		}
		else
		{
			strcpy(all_column_names[num_columns], trimmed_token);
		}
		num_columns++;
		token = strtok(NULL, ",");
	}
}

void display_features()
{
	printf("\nAvailable Columns:\n");
	for (int i = 0; i < num_columns; i++)
	{
		printf("%s (%d) ", all_column_names[i], i + 1);
	}
	printf("\n");
}

int count_rows_in_csv(const char *filename)
{
	FILE *file = fopen(filename, "r");

	int count = 0;
	char line[MAX_ROW_LENGTH];

	// Skip header
	fgets(line, sizeof(line), file);

	// Count and skip empty line
	while (fgets(line, sizeof(line), file) != NULL)
	{
		if (strspn(line, " \t\n") != strlen(line))
		{
			count++;
		}
	}

	fclose(file);
	return count;
}

int get_column_index(const char *col_name)
{
	for (int i = 0; i < num_columns; i++)
	{
		if (strcmp(all_column_names[i], col_name) == 0)
		{
			return i;
		}
	}
	return -1;
}

int read_selected_data()
{
	for (int i = 0; i < feature_n; i++)
	{
		selected_feature_index[i] = get_column_index(selected_features[i]);
	}
	selected_label_index = get_column_index(selected_label);

	// Allocate memory for data
	all_data = (DataRow *)malloc(row_num * sizeof(DataRow));
    if (all_data == NULL) return -1; // Added error check

	FILE *file = fopen(file_name, "r");
    if (file == NULL) return -1; // Added error check

	char line[MAX_ROW_LENGTH];
	fgets(line, sizeof(line), file); // Skip header

	for (int row = 0; row < row_num && fgets(line, sizeof(line), file) != NULL; row++)
	{

		char temp_line_buffer[MAX_ROW_LENGTH];
		strcpy(temp_line_buffer, line);

		char *token;
		int col_index = 0;
		int feature_count = 0;

		token = strtok(temp_line_buffer, ",");

		while (token != NULL && col_index < num_columns)
		{
			// Trim whitespace from token
			char *trimmed_token = trim_whitespace(token);

			// Features
			for (int i = 0; i < feature_n; i++)
			{
				if (col_index == selected_feature_index[i])
				{
					all_data[row].feature_values[feature_count] = atof(trimmed_token);
					feature_count++;
					break;
				}
			}

			// Label
			if (col_index == selected_label_index)
			{
				all_data[row].label_value = atof(trimmed_token);
			}

			col_index++;
			token = strtok(NULL, ",");
		}
	}

	fclose(file);
	return 0;
}

// -------------------------------------------  part 2  ------------------------------------------- //

#define numOutput 1
#define Train_Ratio 0.8
#define MAX_ROWS 1500

float init_weights()
{
	return ((float)rand()) / ((float)RAND_MAX) * 2.0 - 1.0;
}

struct data
{
	float feature_value[SELECTION_SIZE];
	float label_value;
};

//-----------------------------------------
struct dataset
{
	struct data rows[MAX_ROWS];
	int size;
};
//-----------------------------------------

void shuffle(int *array, int size_training)
{ // array and size of array
	if (size_training > 1)
	{
		for (int i = 0; i < size_training; i++)
		{
			int j = i + (rand() % (size_training - i)); // random between 0 to sizeof the rest than +i to index to swap

			////// swap ///////
			int t = array[j];
			array[j] = array[i];
			array[i] = t;
			////////////////////
		}
	}
}

void train_test_split(DataRow all_data_1[], int total_rows, struct dataset *train_set, struct dataset *test_set)
{
	int Train_Size = (int)(total_rows * Train_Ratio); // changed from define to int
	int Test_Size = total_rows - Train_Size;

	int index[total_rows];
	for (int i = 0; i < total_rows; i++)
	{ // shuffle index
		index[i] = i;
	}
	shuffle(index, total_rows);

	train_set->size = Train_Size;
	test_set->size = Test_Size;

    // Use all_data_1 (DataRow struct) to populate train_set/test_set (data struct)
	for (int i = 0; i < Train_Size; i++)
	{ // train data set
        for(int j=0; j<feature_n; j++){
            train_set->rows[i].feature_value[j] = all_data_1[index[i]].feature_values[j];
        }
		train_set->rows[i].label_value = all_data_1[index[i]].label_value;
	}

	for (int i = 0; i < Test_Size; i++)
	{ // test data set
        for(int j=0; j<feature_n; j++){
            test_set->rows[i].feature_value[j] = all_data_1[index[i + Train_Size]].feature_values[j];
        }
		test_set->rows[i].label_value = all_data_1[index[i + Train_Size]].label_value;
	}
}
// ------------------------------------------- END part 2 ------------------------------------------- //

// ------------------------------------------- Part 3 -------------------------------------------

float linear(float x) { return x; }
float binary_step(float x) { return x >= 0 ? 1 : 0; }
float sigmoid(float x) { return 1.0f / (1.0f + expf(-x)); }
float tanh_act(float x) { return tanhf(x); }
float relu(float x) { return x > 0 ? x : 0; }
float leaky_relu(float x) { return x > 0 ? x : 0.01f * x; }
float softsign(float x) { return x / (1.0f + fabsf(x)); }
float elu(float x) { return x >= 0 ? x : expf(x) - 1.0f; }
float softplus(float x) { return x > 0 ? x + log1pf(expf(-x)) : log1pf(expf(x)); }
float swish(float x) { return x / (1.0f + expf(-x)); }

float mish(float x) { 
    float sp = x > 0 ? x + log1pf(expf(-x)) : log1pf(expf(x)); 
    return x * tanhf(sp); 
}

float hard_sigmoid(float x) { 
    float r = 0.2f * x + 0.5f; 
    if(r < 0) r = 0; 
    if(r > 1) r = 1; 
    return r; 
}
float hard_swish(float x) { return x * hard_sigmoid(x); }

// ---------------- Derivatives ----------------
float dlinear(float x) { return 1; }
float dbinary_step(float x) { return 0; }
float dsigmoid(float x) { float s = sigmoid(x); return s * (1 - s); }
float dtanh(float x) { float t = tanh_act(x); return 1 - t * t; }
float drelu(float x) { return x > 0 ? 1 : 0; }
float dleaky_relu(float x) { return x > 0 ? 1 : 0.01f; }
float dsoftsign(float x) { float denom = 1 + fabsf(x); return 1 / (denom * denom); }
float delu(float x) { return x >= 0 ? 1 : expf(x); }
float dsoftplus(float x) { return 1 / (1 + expf(-x)); }
float dswish(float x) { float s = sigmoid(x); return s + x * s * (1 - s); }

float dmish(float x) { 
    float sp = (x > 0) ? x + log1pf(expf(-x)) : log1pf(expf(x)); 
    float tsp = tanhf(sp);
    float s = 1.0f / (1.0f + expf(-x));
    return tsp + x * s * (1.0f - tsp * tsp);
}

float dhard_sigmoid(float x) { return (x >= -2.5f && x <= 2.5f) ? 0.2f : 0; }
float dhard_swish(float x) { return hard_sigmoid(x) + x * dhard_sigmoid(x); }

// ---------------- Loss  ----------------
// MSE
float mse(float actual[], float predicted[], int n) {
    float sum_squared_error = 0.0f;
    for (int i = 0; i < n; i++) {
        float error = actual[i] - predicted[i];
        sum_squared_error += error * error;
    }
    return sum_squared_error / n;
}

void mse_derivative(float y[], float y_hat[], float grad[], int n) {
    float factor = 2.0f / n;
    for (int i = 0; i < n; i++) {
        grad[i] = factor * (y_hat[i] - y[i]);
    }
}

// MAE
float mae(float actual[], float predicted[], int n) {
    float sum_abs_error = 0.0f;
    for (int i = 0; i < n; i++) {
        float error = actual[i] - predicted[i];
        sum_abs_error += fabsf(error);
    }
    return sum_abs_error / n;
}

void mae_derivative(float y[], float y_hat[], float grad[], int n) {
    float factor = 1.0f / n;
    for (int i = 0; i < n; i++) {
        if (y_hat[i] > y[i])
            grad[i] = factor * 1.0f;
        else if (y_hat[i] < y[i])
            grad[i] = factor * -1.0f;
        else
            grad[i] = 0.0f;
    }
}

// ---------------- Pointer arrays (FIXED signatures) ----------------
// Note: Maximum 10 layers support for hidden layers
float (*functions_pointer[10])(float);
float (*dfunction_pointer[10])(float);

float (*outputfunction_pointer)(float);
float (*doutputfunction_pointer)(float); 

// Fixed: Loss functions work on Arrays, not single floats
float (*lfunction_pointer)(float*, float*, int);
void (*dlfunction_pointer)(float*, float*, float*, int);

// ---------------- Lists ----------------
const char* activation_names[13] = {
    "linear","binary_step","sigmoid","tanh","relu","leaky_relu",
    "softsign","elu","softplus","swish","mish","hard_sigmoid","hard_swish"
};

const char* loss_names[2] = {"MSE","MAE"};
const char* output_names[3] = {"linear","sigmoid","tanh"};

// ---------------- Set functions ----------------
void set_hidden_function(int layer, int choice){
    choice--;
    if(choice==0){ functions_pointer[layer]=linear; dfunction_pointer[layer]=dlinear; }
    else if(choice==1){ functions_pointer[layer]=binary_step; dfunction_pointer[layer]=dbinary_step; }
    else if(choice==2){ functions_pointer[layer]=sigmoid; dfunction_pointer[layer]=dsigmoid; }
    else if(choice==3){ functions_pointer[layer]=tanh_act; dfunction_pointer[layer]=dtanh; }
    else if(choice==4){ functions_pointer[layer]=relu; dfunction_pointer[layer]=drelu; }
    else if(choice==5){ functions_pointer[layer]=leaky_relu; dfunction_pointer[layer]=dleaky_relu; }
    else if(choice==6){ functions_pointer[layer]=softsign; dfunction_pointer[layer]=dsoftsign; }
    else if(choice==7){ functions_pointer[layer]=elu; dfunction_pointer[layer]=delu; }
    else if(choice==8){ functions_pointer[layer]=softplus; dfunction_pointer[layer]=dsoftplus; }
    else if(choice==9){ functions_pointer[layer]=swish; dfunction_pointer[layer]=dswish; }
    else if(choice==10){ functions_pointer[layer]=mish; dfunction_pointer[layer]=dmish; }
    else if(choice==11){ functions_pointer[layer]=hard_sigmoid; dfunction_pointer[layer]=dhard_sigmoid; }
    else if(choice==12){ functions_pointer[layer]=hard_swish; dfunction_pointer[layer]=dhard_swish; }
    else{ functions_pointer[layer]=linear; dfunction_pointer[layer]=dlinear; }
}

void set_loss_function(int choice){
    choice--;
    // Fixed assignments without incorrect casting
    if(choice==0){ lfunction_pointer=mse; dlfunction_pointer=mse_derivative; }
    else if(choice==1){ lfunction_pointer=mae; dlfunction_pointer=mae_derivative; }
    else{ lfunction_pointer=mse; dlfunction_pointer=mse_derivative; }
}

void set_output_function(int choice){
    choice--;
    if(choice==0){ outputfunction_pointer=linear; doutputfunction_pointer=dlinear; }
    else if(choice==1){ outputfunction_pointer=sigmoid; doutputfunction_pointer=dsigmoid; }
    else if(choice==2){ outputfunction_pointer=tanh_act; doutputfunction_pointer=dtanh; }
    else{ outputfunction_pointer=linear; doutputfunction_pointer=dlinear; }
}

void print (int number , float list[] ) {
    printf("\n--------------------------- test -------------------------------\n\n");
    for (int i = 0 ; i < number ; i ++) {
        printf("==> %f\n",list[i]);
    }
    printf("\ntest finished--------------- \n\n");
}
// --------------------------------------- END Part 3 -------------------------------------------

// ---------------------- MAIN FUNCTION ----------------------

int main(void)
{
	clear_console();

	// ---------------------- PART 1 ----------------------------------

	while (1)
	{ // loop for open file name
		printf("Input your file name (Must be .csv file): ");
		fgets(file_name, sizeof(file_name), stdin);

		file_name[strcspn(file_name, "\n")] = '\0';
		clear_console();

		if (strstr(file_name, ".csv") == NULL)
		{ // Check is file .csv type
			printf("==============================================\n");
			printf("Invalid file type. Please enter (.csv) file.\n");
			printf("==============================================\n");
			continue;
		}

		FILE *import_file = fopen(file_name, "r");
		if (import_file == NULL)
		{
			printf("==============================================\n");
			printf("There is no file named: %s\nPlease input again.\n", file_name);
			printf("==============================================\n");
			continue;
		}
		else
		{
			char first_row[MAX_ROW_LENGTH];
			if (fgets(first_row, sizeof(first_row), import_file) != NULL)
			{
				split_first_row(first_row);
			}
			fclose(import_file);

			if (num_columns == 0)
			{
				printf("===========================\n");
				printf("Error: not found first row.\n");
				printf("===========================\n");
				continue;
			}

			row_num = count_rows_in_csv(file_name);
			clear_console();

			printf("Total data rows: %d\n", row_num);
			printf("Available columns: %d\n", num_columns);

			break;
		}
	}

	// --- Feature Selection ---

	int selection;
	printf("\n========================================================================\n");
	printf("Select features for input (Feature). Input 0 to stop selecting features.\n");
	printf("========================================================================\n");

	while (1)
	{
		display_features();
		printf("\nSelected Features (%d/%d): ", feature_n, num_columns - 1);
		for (int i = 0; i < feature_n; i++)
		{
			if (i < feature_n && i != 0)
				printf(", ");
			printf("%s", selected_features[i]);
		}
		printf("\nSelect feature # (1-%d, 0 to stop): ", num_columns);

		if (scanf("%d", &selection) != 1)
		{
			printf("Invalid input. Please enter a number.\n");
			while (getchar() != '\n')
				;
			continue;
		}

		while (getchar() != '\n')
			;

		if (selection == 0)
		{
			if (feature_n == 0)
			{
				clear_console();
				printf("You must select at least one feature.\n");
				continue;
			}
			break;
		}
		else if (selection >= 1 && selection <= num_columns)
		{

			int already_selected = 0;
			for (int i = 0; i < feature_n; i++)
			{
				if (strcmp(selected_features[i], all_column_names[selection - 1]) == 0)
				{
					already_selected = 1;
					break;
				}
			}

			if (already_selected)
			{
				printf("Feature %s is already selected.\n", all_column_names[selection - 1]);
			}
			else
			{

				strcpy(selected_features[feature_n], all_column_names[selection - 1]);
				feature_n++;
				printf("Added feature: %s\n", all_column_names[selection - 1]);
			}
		}
		else
		{
			printf("Invalid selection.\n");
		}
	}

	// --- Label Selection ---
	clear_console();
	printf("\n========================================\n");
	printf("Select the target variable (Label).\n");
	printf("==========================================\n");

	int label_selection;
	while (1)
	{

		display_features();
		printf("\nSelect label # (1-%d): ", num_columns);

		if (scanf("%d", &label_selection) != 1)
		{
			printf("Invalid input. Please enter a number.\n");
			while (getchar() != '\n')
				;
			continue;
		}
		while (getchar() != '\n')
			;

		if (label_selection >= 1 && label_selection <= num_columns)
		{
			int is_feature = 0;
			const char *potential_label = all_column_names[label_selection - 1];

			// Check if the selected column is already one of the features
			for (int i = 0; i < feature_n; i++)
			{
				if (strcmp(selected_features[i], potential_label) == 0)
				{
					is_feature = 1;
					break;
				}
			}

			if (is_feature)
			{
				printf("Error: The label cannot be one of the selected features (%s).\n", potential_label);
			}
			else
			{
				strncpy(selected_label, potential_label, MAX_COL_NAME_LEN - 1);
				selected_label[MAX_COL_NAME_LEN - 1] = '\0';
				printf("Selected label: %s\n", selected_label);
				break;
			}
		}
		else
		{
			printf("Invalid selection.\n");
		}
	}

	// --- Read Actual Data ---
	clear_console();

	if (read_selected_data() != 0)
	{
		printf("Error allocating memory for data or opening file. Exiting.\n");
		return 1;
	}

	// --- Display Results ---

	printf("\n==============================================\n");
	printf("Summary of Selections (Names and Values):\n");
	printf("==============================================\n");
	printf("Features Selected: %d\n", feature_n);
	printf("Label: %s\n", selected_label);
	printf("Total Data Rows: %d\n", row_num);

	printf("\nDisplaying first 3 rows of data:\n");
	int display_rows = (row_num > 3) ? 3 : row_num;

	if (row_num > 0)
	{
		for (int r = 0; r < display_rows; r++)
		{
			printf("Row %d:\n", r + 1);
			printf(" 	Features: {");
			for (int i = 0; i < feature_n; i++)
			{
				printf("%s: %.3f", selected_features[i], all_data[r].feature_values[i]);
				if (i < feature_n - 1)
					printf(", ");
			}
			printf("}\n");
			printf(" 	Label: 	{%s: %.3f}\n", selected_label, all_data[r].label_value);
		}
	}
	else
	{
		printf("No data rows to display.\n");
	}

	// ------------------------------------------- END part 1 ------------------------------------------- //

	// ---------------------- PART 2 ----------------------
	srand((unsigned int)time(NULL)); // genarated random seed
	int hiddenLayers_num;

    // NOTE: all_data (DataRow[]) is used directly for train_test_split. all_data_2 removed.

	printf("\nHidden Layer: "); // ask amount of hidden layer
	scanf("%d", &hiddenLayers_num);
	if (hiddenLayers_num < 1)
	{
		printf("Invalid hidden layer number. Exiting.\n");
		// Free all_data later in the main cleanup
		return -1; // kill the program if hiddenlayer doesn't provied
	}

	// allocate array to store number of nodes in each hidden layer
	int *HiddenNode_num = malloc(hiddenLayers_num * sizeof(int)); // for HiddenNode_num
    if (HiddenNode_num == NULL) { /* handle error */ free(all_data); return 1; }

	for (int i = 0; i < hiddenLayers_num; i++)
	{
		printf("Layer %d Number of Hidden Nodes: ", i + 1); // ask node of each layer
		scanf("%d", &HiddenNode_num[i]);
		if (HiddenNode_num[i] < 1)
		{
			printf("Invalid hidden node number in layer %d. Exiting.\n");
			free(HiddenNode_num);
			free(all_data);
			return -1;
		}
	}

	// --------------- Hot fixed add adjust time -------------------------
	int Adjust_time;
	printf("How many time do you want to train (Epochs): ");
	scanf("%d", &Adjust_time);
	while (getchar() != '\n')
		;
	float *losses = (float *)malloc(Adjust_time * sizeof(float)); // updated to dynamic array
    if (losses == NULL) { free(HiddenNode_num); free(all_data); return 1; }

	//--------------------------------------------------

	// train test split
	int Train_Size = (int)(row_num * Train_Ratio);
	int Test_Size = row_num - Train_Size;

	struct dataset train_set, test_set;

    // Use DataRow[] as input to the split function
	train_test_split(all_data, row_num, &train_set, &test_set);

    // Now all_data can be freed if desired, but we keep it allocated until the end.
    // NOTE: The original train_test_split function signature was changed to accept DataRow[]

	// Allocate space for features/labels (though train_set/test_set structs are used directly now, 
    // the original code allocated these, so we'll adjust the logic to use train_set/test_set structs directly in the trainer loop to save memory/copying)


	printf("##################### Data Split Done ###################\n");

	float **bias;
	float **weights;
	float **weight_adjust_record;
	float **bias_adjust_record;

	// --- CORRECTED DYNAMIC ALLOCATION ---
    // Allocate array of pointers for all layers (Hidden + Output)
	bias = (float **)malloc((hiddenLayers_num + 1) * sizeof(float *));
	bias_adjust_record = (float **)malloc((hiddenLayers_num + 1) * sizeof(float *));
	weights = (float **)malloc((hiddenLayers_num + 1) * sizeof(float *));
	weight_adjust_record = (float **)malloc((hiddenLayers_num + 1) * sizeof(float *));

    if (bias == NULL || bias_adjust_record == NULL || weights == NULL || weight_adjust_record == NULL) { /* handle error */ return 1; }

    int z_keep_size = 0; // Total nodes (for z_keep array size calculation)

	for(int i = 0; i < hiddenLayers_num; i++){
		// Bias: One bias for each node in the hidden layer 'i'
		bias[i] = (float *)malloc(HiddenNode_num[i] * sizeof(float));
		bias_adjust_record[i] = (float *)malloc(HiddenNode_num[i] * sizeof(float));
        if (bias[i] == NULL || bias_adjust_record[i] == NULL) { /* handle error */ return 1; }

        z_keep_size += HiddenNode_num[i];

        // Weights: Input size * Output size (nodes in layer 'i')
		int input_size_for_layer_i = (i == 0) ? feature_n : HiddenNode_num[i - 1];
		int size = input_size_for_layer_i * HiddenNode_num[i];

		weights[i] = (float *)malloc(size * sizeof(float));
		weight_adjust_record[i] = (float *)malloc(size * sizeof(float));
        if (weights[i] == NULL || weight_adjust_record[i] == NULL) { /* handle error */ return 1; }
	}

	// Output Layer (Index 'hiddenLayers_num')
	int lastHiddenNodes = HiddenNode_num[hiddenLayers_num - 1];
	int output_layer_size = lastHiddenNodes * numOutput;

	z_keep_size += numOutput; // Add output nodes to z_keep size

	// Bias: One bias for each output
	bias[hiddenLayers_num] = (float *)malloc(numOutput * sizeof(float));
	bias_adjust_record[hiddenLayers_num] = (float *)malloc(numOutput * sizeof(float));
    if (bias[hiddenLayers_num] == NULL || bias_adjust_record[hiddenLayers_num] == NULL) { /* handle error */ return 1; }

	// Weights: Last Hidden Layer Nodes * Output Nodes
	weights[hiddenLayers_num] = (float *)malloc(output_layer_size * sizeof(float));
	weight_adjust_record[hiddenLayers_num] = (float *)malloc(output_layer_size * sizeof(float));
    if (weights[hiddenLayers_num] == NULL || weight_adjust_record[hiddenLayers_num] == NULL) { /* handle error */ return 1; }


    // --- INITIALIZE WEIGHTS AND BIASES ---
	for (int i = 0; i < hiddenLayers_num; i++){
        // Initialize Bias and Bias Adjustment Record (to 0.0f)
		for(int j = 0; j < HiddenNode_num[i]; j++){
			bias[i][j] = init_weights();
            bias_adjust_record[i][j] = 0.0f;
		}

        // Initialize Weights and Weight Adjustment Record (to 0.0f)
        int input_size_for_layer = (i == 0) ? feature_n : HiddenNode_num[i - 1];
        int total_weights = input_size_for_layer * HiddenNode_num[i];
        for (int k = 0; k < total_weights; k++)
        {	
            weights[i][k] = init_weights();
            weight_adjust_record[i][k] = 0.0f;
        }
	}
	
    // Initialize Output Layer Bias
    for(int j = 0; j < numOutput; j++) {
	    bias[hiddenLayers_num][j] = init_weights();
        bias_adjust_record[hiddenLayers_num][j] = 0.0f;
    }

    // Initialize Output Layer Weights
    int total_output_weights = HiddenNode_num[hiddenLayers_num - 1] * numOutput;
    for (int k = 0; k < total_output_weights; k++)
    {
        weights[hiddenLayers_num][k] = init_weights();
        weight_adjust_record[hiddenLayers_num][k] = 0.0f;
    }


	//---------------------------------------- Part 3 -----------------------------------------------

	int choice;

	printf("List of activation functions:\n");
	for (int i = 0; i < 13; i++)
		printf("%2d) %s\n", i + 1, activation_names[i]);

	printf("\nChoose %d hidden layer functions (1-13):\n", hiddenLayers_num);
	for (int i = 0; i < hiddenLayers_num; i++)
	{
		printf("Layer %d choice: ", i + 1);
		scanf("%d", &choice);
		set_hidden_function(i, choice);
	}

	printf("\nList of loss functions:\n");
	for (int i = 0; i < 2; i++)
		printf("%d) %s\n", i + 1, loss_names[i]);
	printf("Choose loss function (1-2): ");
	scanf("%d", &choice);
	set_loss_function(choice);

	printf("\nList of output functions:\n");
	for (int i = 0; i < 3; i++)
		printf("%d) %s\n", i + 1, output_names[i]);
	printf("Choose output function (1-3): ");
	scanf("%d", &choice);
	set_output_function(choice);

	// test print
	float x = 0.5;
	for (int i = 0; i < hiddenLayers_num; i++)
		printf("Hidden %d: %f (derivative %f)\n", i + 1, functions_pointer[i](x), dfunction_pointer[i](x));

	printf("Output function: %f (derivative %f)\n", outputfunction_pointer(x), doutputfunction_pointer(x));

	// -------------------------- Trainer (FIXED LOGIC) ---------------------------------------

    int output_num = 1 ;
	int layers = hiddenLayers_num ; 
	float sum ; 

	// Corrected: z_keep size is calculated dynamically
	float *z_keep = (float *)malloc(z_keep_size * sizeof(float)); // Activation before activation function (z)
    if (z_keep == NULL) { /* handle error and clean up */ return 1; }

	float *output ;
	output = (float *)malloc(output_num*sizeof(float)) ;
    if (output == NULL) { /* handle error */ return 1; }


	float learning_rate = 0.01 ;

	for (int adjust_time_count = 0 ; adjust_time_count < Adjust_time ; adjust_time_count ++) {

		float total_loss_epoch = 0.0f; // Accumulate loss for this epoch

		// Zero out adjust records for the new epoch
        for(int i = 0; i < hiddenLayers_num + 1; i++) {
            int size_w = 0;
            int size_b = 0;
            if (i < hiddenLayers_num) {
                int input_size_for_layer_i = (i == 0) ? feature_n : HiddenNode_num[i - 1];
                size_w = input_size_for_layer_i * HiddenNode_num[i];
                size_b = HiddenNode_num[i];
            } else {
                size_w = HiddenNode_num[hiddenLayers_num - 1] * numOutput;
                size_b = numOutput;
            }
            
            for (int k = 0; k < size_w; k++) weight_adjust_record[i][k] = 0.0f;
            for (int k = 0; k < size_b; k++) bias_adjust_record[i][k] = 0.0f;
        }

		// Corrected: Loop through the Training Set (train_set) only
		for (int row = 0 ; row < train_set.size ; row ++ ) {

			//printf("\n             run trought row : %d (Training)\n",row);


			int z_keep_index = 0; // Current index in z_keep array
			int previous_layer_z_start = 0 ;//for tracking previous layer's z_keep start index

            // --- FORWARD PROPAGATION (Hidden Layers) ---
			for (int layer_num = 0 ; layer_num < layers ; layer_num ++ ) {

				int input_size_for_layer = (layer_num == 0) ? feature_n : HiddenNode_num[layer_num - 1];

				for (int node_num = 0 ; node_num < HiddenNode_num[layer_num] ; node_num++){
					sum = 0;
					
					for (int previous_node = 0 ; previous_node < input_size_for_layer ; previous_node++ ) {
                        
						float input_value;
						if (layer_num == 0) {
                            // Input is the feature value from the current training row
							input_value = train_set.rows[row].feature_value[previous_node];
						} else {
                            // Input is the activation (a) from the previous layer's z
                            int prev_z_index = previous_layer_z_start + previous_node;
							input_value = (*functions_pointer[layer_num-1])(z_keep[prev_z_index]) ;
						}

                        int w_index = node_num * input_size_for_layer + previous_node;
						sum += input_value * weights[layer_num][w_index];
					}
					
					sum += bias[layer_num][node_num] ; 
					z_keep[z_keep_index] = sum ; // Keep z value for backpropagation

					z_keep_index++;
				}
				
				previous_layer_z_start += HiddenNode_num[layer_num] ; // Update starting index for the next layer's input
			}


            // --- FORWARD PROPAGATION (Output Layer) ---
			for (int outputnode_num = 0 ; outputnode_num < output_num ; outputnode_num ++) { 
				
				sum = 0 ;
				int lasthiddennode_num = HiddenNode_num[layers-1]; // Input size for output layer
                
				for (int prev_node = 0 ; prev_node < lasthiddennode_num ; prev_node++){

					// Get activation (a) from the last hidden layer
					int z_idx = previous_layer_z_start + prev_node; 
					float value = (*functions_pointer[layers-1])(z_keep[z_idx]); 

                    int w_index = outputnode_num * lasthiddennode_num + prev_node; 
					
					sum += value * weights[layers][w_index]; 
				}

				sum += bias[layers][outputnode_num] ; 
				z_keep[z_keep_index + outputnode_num] = sum ; // Keep z value for output layer
				
                // Apply Output Activation Function
                sum = (*outputfunction_pointer)(sum);
				*(output+outputnode_num) = sum ;

				//printf("output node%d's answer : %f\n",outputnode_num,sum);
			}
            
            // --- LOSS CALCULATION ---
			float label_arr[1];
			label_arr[0] = train_set.rows[row].label_value;
			
			total_loss_epoch += lfunction_pointer(label_arr, output, 1);
			

			// --- BACKPROPAGATION (Calculate Gradients) ---

			int base_node_idx = z_keep_size - output_num; // Start index of output layer nodes in z_keep

			float grad[1];
			dlfunction_pointer(label_arr, output, grad, 1);
			float dlossdoutput = grad[0]; 
			
			// 1. BACKPROPAGATION (Output Layer)
			
			for (int outputnode_num = output_num-1 ; outputnode_num >= 0 ; outputnode_num--) {

				float dlda_output = dlossdoutput;

				// Calculate dl/dz (dldz_output)
				float doutputdz = (*doutputfunction_pointer)(z_keep[base_node_idx + outputnode_num]);
				float dldz_output = dlda_output * doutputdz;

				// Store dl/dz in z_keep array for use in previous layer
				z_keep[base_node_idx + outputnode_num] = dldz_output ;

				// Accumulate dl/dbias (dl/dz * 1)
				bias_adjust_record[layers][outputnode_num] += dldz_output ;

				// Accumulate dl/dw (dl/dz * dz/dw = dldz * a_prev)
				int prev_layer_nodes = HiddenNode_num[layers-1];
				for (int lasthiddennode_num = 0 ; lasthiddennode_num < prev_layer_nodes; lasthiddennode_num++){
					// Activation (a) from the last hidden layer
					int z_idx_prev = previous_layer_z_start + lasthiddennode_num; // Correct index to get z of prev layer
					float a_prev = (*functions_pointer[layers-1])(z_keep[z_idx_prev]);

                    int w_index = outputnode_num * prev_layer_nodes + lasthiddennode_num; 
					
					weight_adjust_record[layers][w_index] += dldz_output * a_prev ;
				}
			}

			// 2. BACKPROPAGATION (Hidden Layers)
			
			int current_layer_z_start = base_node_idx; // Start index of dldz in z_keep for current layer (output)
			int current_layer_nodes = numOutput; // Number of nodes in the layer *receiving* the gradient (Output)
			
			for (int layer_num = layers-1 ; layer_num >= 0 ; layer_num --) { 

				int next_layer_z_start = 0; // Start index of z in z_keep for the layer *being* calculated (closer to input)
				for(int k=0; k<layer_num; k++) next_layer_z_start += HiddenNode_num[k];
				
				// The layer we are calculating dL/dz for: HiddenNode_num[layer_num]
				int nodes_in_this_layer = HiddenNode_num[layer_num];

				// The layer feeding *into* this layer: feature_n (if layer 0) or HiddenNode_num[layer_num-1]
				int input_size_for_this_layer = (layer_num == 0) ? feature_n : HiddenNode_num[layer_num - 1];

				// Step 1: Calculate dL/da for each node in layer_num, then dL/dz for each node.
				for (int current_node_num = 0 ; current_node_num < nodes_in_this_layer ; current_node_num++) { 
					
					float dlda = 0.0f;
					
					// Sum (dL/dz * dz/da) from all nodes in the *next* layer (index layer_num+1, i.e., current_layer_nodes)
					for (int next_node_num = 0 ; next_node_num < current_layer_nodes ; next_node_num++) {
						
						// dL/dz of next layer (already calculated and stored in z_keep)
						float dldz_next = z_keep[current_layer_z_start + next_node_num];

						// dz/da (which is weight from current_node to next_node)
						int w_idx = next_node_num * nodes_in_this_layer + current_node_num;
						float w_val = weights[layer_num + 1][w_idx];

						dlda += dldz_next * w_val;
					}

					// Calculate dL/dz for the current node: dL/dz = dL/da * da/dz
					float da_dz = (*dfunction_pointer[layer_num])(z_keep[next_layer_z_start + current_node_num]);
					float dldz = dlda * da_dz;

					// Store dL/dz for use by the previous layer (closer to input)
					z_keep[next_layer_z_start + current_node_num] = dldz;
					
					// Accumulate dl/dbias (dl/dz * 1)
					bias_adjust_record[layer_num][current_node_num] += dldz;

					// Step 2: Accumulate dL/dw for all weights connecting to this node
					for (int previous_node = 0 ; previous_node < input_size_for_this_layer ; previous_node++) {
						
						float a_prev;
						if (layer_num == 0) {
                            // Input is the feature value
							a_prev = train_set.rows[row].feature_value[previous_node];
						} else {
                            // Input is the activation (a) from the previous layer's z
							int prev_layer_nodes = HiddenNode_num[layer_num-1];
							int prev_layer_z_start = next_layer_z_start - prev_layer_nodes;
							float prev_z = z_keep[prev_layer_z_start + previous_node];
							a_prev = (*functions_pointer[layer_num-1])(prev_z);
						}

                        int w_idx = current_node_num * input_size_for_this_layer + previous_node;
						
						// Accumulate dl/dw
						weight_adjust_record[layer_num][w_idx] += dldz * a_prev;
					}
				}
				
				// Move to the next layer (closer to input)
				current_layer_z_start = next_layer_z_start;
				current_layer_nodes = nodes_in_this_layer;
				
			}
			
		} // End of row loop (Training Sample)

        // Store average loss for the epoch
        losses[adjust_time_count] = total_loss_epoch / train_set.size;
        printf("Epoch %d Loss: %f\n", adjust_time_count + 1, losses[adjust_time_count]);


        // --- APPLY WEIGHT AND BIAS UPDATES (SGD/Batch Gradient Descent) ---
        
        // Loop through all layers (Hidden + Output)
		for (int layer_num = 0 ; layer_num < layers+1 ; layer_num++) { 
			
			if(layer_num == layers){
                // Output Layer
				int prev_layer_nodes = HiddenNode_num[layers-1];
				for (int next_node = 0 ; next_node < output_num ; next_node++) {
					
					// Update Weights
					for (int previous_node = 0 ; previous_node < prev_layer_nodes ; previous_node++) {
						int w_idx = next_node * prev_layer_nodes + previous_node;
						float adjustment = weight_adjust_record[layer_num][w_idx] / train_set.size * learning_rate; // Average over all rows
						weights[layer_num][w_idx] -= adjustment;
					}
					
					// Update Bias
					float adjustment = bias_adjust_record[layer_num][next_node] / train_set.size * learning_rate; // Average over all rows
					bias[layer_num][next_node] -= adjustment;
				}

			}else{    
                // Hidden Layers
				int current_layer_nodes = HiddenNode_num[layer_num];
				int input_size_for_layer = (layer_num == 0) ? feature_n : HiddenNode_num[layer_num - 1];

				for (int next_node = 0 ; next_node < current_layer_nodes ; next_node++) {
					
					// Update Weights
					for (int previous_node = 0 ; previous_node < input_size_for_layer ; previous_node++) {
						int w_idx = next_node * input_size_for_layer + previous_node;
						float adjustment = weight_adjust_record[layer_num][w_idx] / train_set.size * learning_rate; // Average over all rows
						weights[layer_num][w_idx] -= adjustment;
					}

					// Update Bias
					float adjustment = bias_adjust_record[layer_num][next_node] / train_set.size * learning_rate; // Average over all rows
					bias[layer_num][next_node] -= adjustment;
				}
					
			}
		}
		

	} // End of adjust_times loop (Epoch)



	printf("\n==============================================\n");
	printf("Final Model Weights and Biases:\n");
	printf("==============================================\n");


    // Print Hidden Layers
	for (int layer_num = 0 ; layer_num < layers ; layer_num++) { 
		printf("Layer %d (Nodes: %d):\n",layer_num + 1, HiddenNode_num[layer_num]);
		
        int current_layer_nodes = HiddenNode_num[layer_num];
		int input_size_for_layer = (layer_num == 0) ? feature_n : HiddenNode_num[layer_num - 1];

		for (int next_node = 0 ; next_node < current_layer_nodes ; next_node++) {
			printf("  Node %d:\n",next_node + 1);
			
			for (int previous_node = 0 ; previous_node < input_size_for_layer ; previous_node++) {
                int w_idx = next_node * input_size_for_layer + previous_node;
				printf("    Weight from input %d: %f\n", previous_node + 1, weights[layer_num][w_idx]);
			}
			
			printf("    Final Bias: %f\n\n",bias[layer_num][next_node]);
		}
				
	}

    // Print Output Layer
	int prev_layer_nodes_out = HiddenNode_num[layers-1];
	printf("Output Layer (Nodes: %d):\n", numOutput);
	for (int next_node = 0 ; next_node < output_num ; next_node++) {
			printf("  Output Node %d:\n",next_node + 1);
			
			for (int previous_node = 0 ; previous_node < prev_layer_nodes_out ; previous_node++) {
                int w_idx = next_node * prev_layer_nodes_out + previous_node;
				printf("    Weight from last hidden node %d: %f\n", previous_node + 1, weights[layers][w_idx]);
			}

			printf("    Final Bias: %f\n\n",bias[layers][next_node]);

		}

			
	// *** Note: To actually evaluate performance, you would add a separate loop here
	// using the 'test_set' data and the trained weights/biases to calculate Test Loss/Metrics.
	printf("\n--- Note: Evaluation on test set is not implemented in this version. ---\n");
	printf("\n==============================================\n");
	printf("Model Training Complete.\n");
	printf("==============================================\n");

	// Print Loss History
	for(int i = 0; i < Adjust_time; i++) printf("Loss epoch %d: %f\n", i+1, losses[i]);


	// --- FREE ALLOCATED MEMORY ---
    
    // Free 2D arrays (inner arrays first, then outer array of pointers)
    for(int i = 0; i < hiddenLayers_num + 1; i++) {
		if (bias[i] != NULL) free(bias[i]);
		if (bias_adjust_record[i] != NULL) free(bias_adjust_record[i]);
		if (weights[i] != NULL) free(weights[i]);
		if (weight_adjust_record[i] != NULL) free(weight_adjust_record[i]);
    }

    free(bias);
    free(bias_adjust_record);
    free(weights);
    free(weight_adjust_record);

    // Free 1D arrays and structures
	free(all_data); // Free Part 1 data
	free(HiddenNode_num);
	free(output); // Allocated in Trainer section
	free(z_keep);
	free(losses);


	return 0;
}
