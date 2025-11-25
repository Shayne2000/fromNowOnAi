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

	FILE *file = fopen(file_name, "r");

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
#define Max_node 10 // thong nee chai tum ah rai kub
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

void train_test_split(DataRow all_data_2[], int total_rows, struct dataset *train_set, struct dataset *test_set)
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

	for (int i = 0; i < Train_Size; i++)
	{ // train data set
		for(int j=0; j<feature_n; j++){
            train_set->rows[i].feature_value[j] = all_data_2[index[i]].feature_values[j];
        }
		train_set->rows[i].label_value = all_data_2[index[i]].label_value;
	}

	for (int i = 0; i < Test_Size; i++)
	{ // test data set
        for(int j=0; j<feature_n; j++){
            test_set->rows[i].feature_value[j] = all_data_2[index[i + Train_Size]].feature_values[j];
        }
		test_set->rows[i].label_value = all_data_2[index[i + Train_Size]].label_value;
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
		printf("Error allocating memory for data. Exiting.\n");
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

	struct data *all_data_2 = (struct data *)malloc(row_num * sizeof(struct data));

	for (int i = 0; i < row_num; i++)
	{
		for (int j = 0; j < feature_n; j++)
		{
			all_data_2[i].feature_value[j] = all_data[i].feature_values[j];
		}
		all_data_2[i].label_value = all_data[i].label_value;
	}
	

	printf("\nHidden Layer: "); // ask amount of hidden layer
	scanf("%d", &hiddenLayers_num);
	if (hiddenLayers_num < 1)
	{
		printf("Invalid hidden layer number. Exiting.\n");
		free(all_data_2);
		return -1; // kill the program if hiddenlayer doesn't provied
	}

	// allocate array to store number of nodes in each hidden layer
	int *HiddenNode_num = malloc(hiddenLayers_num * sizeof(int)); // for HiddenNode_num

	for (int i = 0; i < hiddenLayers_num; i++)
	{
		printf("Layer %d Number of Hidden Nodes: ", i + 1); // ask node of each layer
		scanf("%d", &HiddenNode_num[i]);
		if (HiddenNode_num[i] < 1)
		{
			printf("Invalid hidden node number in layer %d. Exiting.\n");
			free(HiddenNode_num);
			free(all_data_2);
			return -1;
		}
	}

	// --------------- Hot fixed add adjust time -------------------------
	int Adjust_time;
	printf("How many time do you want to train: ");
	scanf("%d", &Adjust_time);
	while (getchar() != '\n')
		;
	float *losses = (float *)malloc(Adjust_time * sizeof(float)); // updated
	//--------------------------------------------------

	// train test split
	int Train_Size = (int)(row_num * Train_Ratio);
	int Test_Size = row_num - Train_Size;

	struct dataset train_set, test_set;

	// float (*train_features)[feature_n] = (float (*)[feature_n])malloc(Train_Size * feature_n * sizeof(float));
	// float *train_labels = (float *)malloc(Train_Size * sizeof(float));
	// float (*test_features)[feature_n] = (float (*)[feature_n])malloc(Test_Size * feature_n * sizeof(float));
	// float *test_labels = (float *)malloc(Test_Size * sizeof(float));

	train_test_split(all_data_2, row_num, &train_set, &test_set);

	// free(all_data_2); // Free

	// split train/test set
	// for (int i = 0; i < train_set.size; i++)
	// {
	// 	for (int j = 0; j < feature_n; j++)
	// 	{
	// 		train_features[i][j] = train_set.rows[i].feature_value[j];
	// 	}
	// 	train_labels[i] = train_set.rows[i].label_value;
	// }

	// for (int i = 0; i < test_set.size; i++)
	// {
	// 	for (int j = 0; j < feature_n; j++)
	// 	{
	// 		test_features[i][j] = test_set.rows[i].feature_value[j];
	// 	}
	// 	test_labels[i] = test_set.rows[i].label_value;
	// }

	// printf("##################### test2 ###################\n");

	//-------------show train-test features, labels-------------------------
	// printf("\nTrain features:\n");
	// for (int i = 0; i < Train_Size; i++)
	// {
	// 	for (int j = 0; j < feature_n; j++)
	// 		printf("%f ", train_features[i][j]);
	// 	printf(" Label: %f\n", train_labels[i]);
	// }
	// printf("Test features:\n");
	// for (int i = 0; i < Test_Size; i++)
	// {
	// 	for (int j = 0; j < feature_n; j++)
	// 		printf("%f ", test_features[i][j]);
	// 	printf(" Label: %f\n", test_labels[i]);
	// }
	//---------------------------------------------------------------------------------------
	// int total_hidden_nodes = 0; // random hidden bias
	// for (int i = 0; i < hiddenLayers_num; i++)
	// 	total_hidden_nodes += HiddenNode_num[i];

	// float *hiddenLayersBias = (float *)malloc(total_hidden_nodes * sizeof(float));

	//---------------------------------------------------------------------------------------------------
	// int total_hidden_weight = 0;		// random hidden weight
	// int weight_index[hiddenLayers_num]; // VLA
	// weight_index[0] = 0;
	// int input_size;
	// for (int i = 0; i < hiddenLayers_num; i++)
	// {
	// 	if (i == 0)
	// 		input_size = feature_n; // Use feature_n
	// 	else
	// 		input_size = HiddenNode_num[i - 1];
	// 	total_hidden_weight += HiddenNode_num[i] * input_size;
	// }

	// float **weights = (float *)malloc((total_hidden_weight+HiddenNode_num[hiddenLayers_num - 1]) * sizeof(float));

	// printf("##################### test3 ###################\n");
	float **bias;
	float **weights;
	// float weight_adjust_record[6][3] = {{0,0,0},{0,0},{0,0},{0},{0},{0}}; //fix HiddenNode_num//////////////////////////////////////////////
	// float bias_adjust_record[6][2] = {{0},{0,0},{0},{0},{0},{0}};
	float **weight_adjust_record;
	float **bias_adjust_record;

	// printf("layers+1 : %d\n",hiddenLayers_num+1);

	bias = (float **)malloc((hiddenLayers_num + 1) * sizeof(float *));
	bias_adjust_record = (float **)malloc((hiddenLayers_num + 1) * sizeof(float *));
	weights = (float **)malloc((hiddenLayers_num + 1) * sizeof(float *));
	weight_adjust_record = (float **)malloc((hiddenLayers_num + 1) * sizeof(float *));

	for(int i = 0; i < hiddenLayers_num; i++){
		//printf("loop in layer : %d\n",i);

		bias[i] = (float *)malloc(HiddenNode_num[i] * sizeof(float));
		bias_adjust_record[i] = (float *)malloc(HiddenNode_num[i] * sizeof(float));
		//printf("bias have : %d\n",HiddenNode_num[i]);

		if(i == 0){
			//printf("%d * %d = %d\n",feature_n,HiddenNode_num[i],feature_n * HiddenNode_num[i]);
			//weights[0] = malloc(sizeof(float));
			weights[i] = (float *)malloc((feature_n * HiddenNode_num[i]) * sizeof(float));
			weight_adjust_record[i] = (float *)malloc((feature_n * HiddenNode_num[i]) * sizeof(float));
			//printf("test\n");
		} 
		else{
			//printf("%d * %d = %d\n",HiddenNode_num[i-1],HiddenNode_num[i],HiddenNode_num[i - 1] * HiddenNode_num[i]);
			weights[i] = (float *)malloc((HiddenNode_num[i - 1] * HiddenNode_num[i]) * sizeof(float));
			weight_adjust_record[i] = (float *)malloc((HiddenNode_num[i - 1] * HiddenNode_num[i]) * sizeof(float));
		} 
		//printf("finish a loop\n\n");
	}
	//printf("pass the loop\n");
	bias[hiddenLayers_num] = (float *)malloc(numOutput*sizeof(float));
	bias_adjust_record[hiddenLayers_num]= (float *)malloc(numOutput*sizeof(float));
	weights[hiddenLayers_num] = (float *)malloc(HiddenNode_num[hiddenLayers_num - 1] * numOutput * sizeof(float));
	weight_adjust_record[hiddenLayers_num] = malloc(HiddenNode_num[hiddenLayers_num - 1] * numOutput * sizeof(float));
	// printf("##################### test4 ###################\n");
	for (int i = 0; i < hiddenLayers_num; i++){

		for(int j = 0; j < HiddenNode_num[i]; j++){
			bias[i][j] = init_weights();
			bias_adjust_record[i][j] = 0;
			// printf("random bias at (layer,index) : %d,%d\n",i,j);
		}
		int input_size;
		if (i == 0)
			input_size = feature_n; // Use feature_n
		else
			input_size = HiddenNode_num[i - 1];
		// int index = weight_index[i];
		for (int node = 0; node < HiddenNode_num[i]*input_size; node++)
		{
			
			// printf("weight at (layer, index) : %d, %d", i, node);
			weights[i][node] = init_weights();
			weight_adjust_record[i][node] = 0;
			
		}
	}

    for (int k = 0; k < HiddenNode_num[hiddenLayers_num - 1]; k++)
    {
        weights[hiddenLayers_num][k] = init_weights();
        weight_adjust_record[hiddenLayers_num][k] = 0.0f;
    }
	
	bias[hiddenLayers_num][0] = init_weights();
	bias_adjust_record[hiddenLayers_num][0] = 0;


	//-------------------------------------------------------------------------------

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

	// free allocated memory

	// -------------------------- Trainer ---------------------------------------

	int output_num = numOutput ;


	// printf("main program start\n");

	int adjust_times = Adjust_time; // have to get user input first -----> do it later

	/////////////// read file, show to user and make user select (feture,row,label) ////////// output --> features,label,features_num ////////


	int layers = hiddenLayers_num ; //get from user input

	float value ; // just keep things around

	float sum ; //declare here is fine

	int number_of_node = 0 ; // for set size of array according to nodes


	for (int layer_num = 0 ; layer_num < layers ; layer_num ++ ) {

		number_of_node += HiddenNode_num[layer_num] ;
		// printf("%d\n",number_of_node);

	}

	number_of_node += output_num ;


	// float dllastzda_array[] ;
	float z_keep [number_of_node] ;
	// printf("n : %d",number_of_node);


	int z_lastlayer_index = -1 ;

	float *output ;
	output = (float *)malloc(output_num*sizeof(float)) ;

	float learning_rate = 0.1 ;

	for (int adjust_time_count = 0 ; adjust_time_count < adjust_times ; adjust_time_count ++) {

		float total_loss_epoch = 0 ;
		
			for (int layer = 0; layer < layers+1; layer++) {
				int size_w;
				if (layer == 0) {
					size_w = feature_n * HiddenNode_num[0];
				} else if (layer == layers) {
					size_w = HiddenNode_num[layers-1] * output_num;  // ใช้ numOutput / output_num
				} else {
					size_w = HiddenNode_num[layer-1] * HiddenNode_num[layer];
				}
				memset(weight_adjust_record[layer], 0, size_w * sizeof(float));
			}

			// reset bias_adjust_record
			for (int layer = 0; layer < layers+1; layer++) {
				int size_b;
				if (layer == layers) {
					size_b = output_num;          // ไม่ใช่ HiddenNode_num[layers]
				} else {
					size_b = HiddenNode_num[layer];
				}
				memset(bias_adjust_record[layer], 0, size_b * sizeof(float));
			}

		for (int row = 0 ; row < train_set.size ; row ++ ) {

			// printf("\n             run trought row : %d\n",row);


			int node_index = 0; // using with activationfunction_output
			int previouslayernode_sum = 0 ;//for tracking previous node during interaction between layers

			for (int layer_num = 0 ; layer_num < layers ; layer_num ++ ) {

				for (int node_num = 0 ; node_num < HiddenNode_num[layer_num] ; node_num++){
					sum = 0;
					if (layer_num == 0) {
						for (int previous_node = 0 ; previous_node < feature_n ; previous_node++ ) {

							
							sum += train_set.rows[row].feature_value[previous_node] * weights[layer_num][node_num*feature_n+previous_node];
							// printf("(%f * %f) + ",all_data[row].feature_values[previous_node],weights[layer_num][node_num*feature_n+previous_node]);

							// printf("weight index : %d * %d + %d = %d\n",node_num,feature_n,previous_node,node_num*feature_n+previous_node);
							//dzdw_array[w_index++] = all_data[row].feature_values[previous_node] ;
							
						}
						
					}else{

						for (int previous_node = 0 ; previous_node < HiddenNode_num[layer_num-1]; previous_node ++ ) {

							// print(number_of_node,z_keep);
							
							value = (*functions_pointer[layer_num-1])(z_keep[previouslayernode_sum + previous_node]) ; //last z

							// printf("weight index : %d\n",node_num*HiddenNode_num[layer_num-1]+previous_node);
							// printf("z index that use for calculation : %d + %d = %d\n",previouslayernode_sum,previous_node,previouslayernode_sum+previous_node);
							// printf("%f\n",z_keep[previouslayernode_sum + previous_node]);
							// printf("%f",z_keep[0]);

							// printf("(%f * %f) + ",value,weights[layer_num][node_num*HiddenNode_num[layer_num-1]+previous_node]);
							//printf("value : %f    previous_sum : %d    previous_node : %d\n",value,previouslayernode_sum,previous_node);
							sum += value * weights[layer_num][node_num*HiddenNode_num[layer_num-1]+previous_node] ;
							//dzdw_array[w_index++] = value ;//?
						
							//printf("using value of node %d",previouslayernode_sum+previous_node);
						}
						
					}
					//printf("\n");

					
					//printf("base add : %d     layer : %d\n",HiddenNode_num[layer_num],layer_num);
					//printf("bias at layer : %d   node : %d     value : %f\n",layer_num,node_num,bias[layer_num][node_num]);
					sum += bias[layer_num][node_num] ; 
					// printf("%f\n",bias[layer_num][node_num]);
					z_keep[node_index] = sum ;
					// printf("output in this node : %f at index %d\n",sum,node_index);
					sum = (*functions_pointer[layer_num])(sum) ; 

					//printf("w index as in fp : %d\n",w_index);
					
					// printf("after function : %f at index %d\n",sum,node_index);
					

					node_index++;
					
				}

				//printf("finished one layer\n");
				if (layer_num != 0) {
					previouslayernode_sum += HiddenNode_num[layer_num-1] ;
					// printf("previousnode sum += %d\n",HiddenNode_num[layer_num-1]);
				}
				

			}
			// print(number_of_node,z_keep);

			// printf("\n-----finished forward propagation--------\n");
			// print(number_of_node,z_keep);

			for (int outputnode_num = 0 ; outputnode_num < output_num ; outputnode_num ++) { 
				
				sum = 0 ;
				for (int lasthiddennode_num = 0 ; lasthiddennode_num < HiddenNode_num[layers-1]; lasthiddennode_num++){

					//printf("%d %d\n",layers-1,lasthiddennode_num);
					
					// printf("z index that use for calculation : %d - %d + %d = %d\n",node_index,HiddenNode_num[layers-1],lasthiddennode_num,node_index-HiddenNode_num[layers-1]+lasthiddennode_num);
					value = (*functions_pointer[layers-1])(z_keep[node_index-HiddenNode_num[layers-1]+lasthiddennode_num]); //value of last hidden

					//printf("%d * %d + %d = \n",outputnode_num,output_num,lasthiddennode_num);
					//printf("weight index : %d\n",outputnode_num*output_num+lasthiddennode_num);
					//printf("weight index : %d\n",w_index);
					//printf("value * weight = %f * %f \n",value,weights[layers][w_index]);
					sum += value * weights[layers][outputnode_num*HiddenNode_num[layers-1]+lasthiddennode_num]; 
					//printf("%d\n",lasthiddennode_num);
				}

				sum += bias[layers][outputnode_num] ; 
				z_keep[node_index+outputnode_num] = sum ;
				sum = (*outputfunction_pointer)(sum);
				*(output+outputnode_num) = sum ;

				// printf("output node%d's answer : %f\n",outputnode_num,sum);
			}

			// printf("-------finish answering----------\n");
			// print(number_of_node,z_keep);


			///--------------------- form here, z_keep will keep dl/dz value  no ----------------------

			// losses[adjust_time_count] = (*lfunction_pointer)(label_samples,output,output_num);
			float label_arr[1];
			label_arr[0] = train_set.rows[row].label_value;

			total_loss_epoch += lfunction_pointer(label_arr, output, 1);
			



			float dzdw,dzdz,dzdb,dldlast_z,dvaluedz,dlast_zdvalue,dldz,dlda ;
			// printf("\n---------start backpropagation-------------\n");

			node_index = number_of_node-1;// using with z_keep
			//printf("w index start at %d\n\n",--w_index);
			//w_index-- ;

			float dlossdoutput,doutputdz;
			// float *grad ;
			// grad = (float *)malloc(sizeof(float)*output_num);
			// float dldzs[layers+1][];

			
			float grad[1];
			dlfunction_pointer(label_arr, output, grad, 1);
			dlossdoutput = grad[0]; 
			
			// (*dlfunction_pointer)(feature_samples,output,grad,output_num);

			// dldas[layers] = 0;

			//printf("finding dloss/doutputnode loop\n");
			for (int outputnode_num = output_num-1 ; outputnode_num >= 0 ; outputnode_num--) {
				// printf("========== you shall pass =========================\n");

				//dlossdoutput = (*grad+outputnode_num);

				doutputdz = (*doutputfunction_pointer)(z_keep[node_index]);
				// printf("========== you shall pass 2 =========================\n");

				

				bias_adjust_record[layers][outputnode_num] += (dlossdoutput*doutputdz)/train_set.size ;
				// printf("%d %d",layers,outputnode_num);
				// bias_adjust_record[0][outputnode_num] += 1 ;

				// printf("furtest ==> dloss / dbias : %f\n",dlossdoutput);

				z_keep[number_of_node-output_num+outputnode_num] = dlossdoutput * doutputdz ;


				node_index--;

			}

			// print(number_of_node,z_keep);
			// print(3,bias_adjust_record[layers]);

			int base = number_of_node -output_num - 1 - HiddenNode_num[layers-1]; //closssss
			int change_base = 0;
			node_index;
		

			//printf("\ndloss/dweight loop\n");
			//printf("prepare for backprop -->  0 <= layer <= %d\n",layers-1);
			for (int layer_num = layers ; layer_num > 0 ; layer_num --) { 
				// printf("\n--> layer num for backprop : %d\n\n",layer_num);



				for (int closer_node = HiddenNode_num[layer_num-1]-1 ; closer_node >= 0 ; closer_node--) { 

					dlda = 0;
					if (layer_num == layers) {

						node_index = number_of_node-output_num-HiddenNode_num[layers-1]+1;

						// print(number_of_node,z_keep);

						for (int furter_node = output_num-1 ; furter_node >= 0 ; furter_node--) {

							dldlast_z = z_keep[base+furter_node+HiddenNode_num[layers-1]+1] ; 

							dzdw = (*functions_pointer[layer_num-1])(z_keep[base+closer_node+1]);

							// printf("furtest ==> dloss/dw%d : %f\n",closer_node,dldlast_z*dzdw);

							weight_adjust_record[layer_num][furter_node*HiddenNode_num[layer_num-1]+closer_node] += (dldlast_z * dzdw)/train_set.size ;



							// printf("\nfind dlda for furter  z index : %d + %d + %d + 1 = %d\n",base,furter_node,HiddenNode_num[layer_num-1],base+furter_node+HiddenNode_num[layers-1]+1);
							// printf("find dlda for furter  w (layer,index) : %d,(%d + %d * %d = %d)\n\n",layer_num,closer_node,HiddenNode_num[layer_num-1],furter_node,furter_node*HiddenNode_num[layer_num-1]+closer_node);

							dlda += z_keep[base+furter_node+HiddenNode_num[layers-1]+1] * weights[layer_num][closer_node+furter_node*HiddenNode_num[layer_num-1]] ;
							// printf("dlda += %f * %f",z_keep[base+furter_node+HiddenNode_num[layers-1]+1],weights[layer_num][closer_node+furter_node*HiddenNode_num[layer_num-1]]);
							// printf(" = %f\n",z_keep[base+furter_node+HiddenNode_num[layers-1]+1] * weights[layer_num][closer_node+furter_node*HiddenNode_num[layer_num-1]]);
						}

						dvaluedz = (*dfunction_pointer[layer_num-1])(z_keep[base+closer_node+1]);
						// printf("dvalue/dz = %f\n",dvaluedz);
						// printf("dlda : %f\n",dlda);
						dldz = dlda * dvaluedz;
						bias_adjust_record[layer_num-1][closer_node] = dldz;
						// printf("==> dloss/ dbias : %f\n",dldz);

						// printf("z index to keep dldz in closer node : %d + %d + 1 = %d    value : %f\n\n",base,closer_node,base+closer_node+1,dldz);
						z_keep[base+closer_node+1] = dldz;

					}else{ //between hidden and hidden

						

						// printf("run at HiddenNode_num %d --> %d node\n",layer_num,HiddenNode_num[layer_num]);
						for (int furter_node = HiddenNode_num[layer_num]-1 ; furter_node >= 0 ; furter_node--) {
							
							dldlast_z = z_keep[base+furter_node+HiddenNode_num[layer_num-1]+1] ; 
							// printf("last z at z index : %d + %d + %d + 1 = %d\n",base,furter_node,HiddenNode_num[layer_num-1],base+furter_node+HiddenNode_num[layer_num-1]+1);

							dzdw = (*functions_pointer[layer_num-1])(z_keep[base+closer_node+1]);

							// printf("==> dloss/dw%d : %f\n",closer_node,dldlast_z*dzdw);

							weight_adjust_record[layer_num][furter_node*HiddenNode_num[layer_num-1]+closer_node] += (dldlast_z * dzdw)/train_set.size ;

							// printf("\nfind dlda for furter  z index : %d + %d + %d + 1 = %d\n",base,furter_node,HiddenNode_num[layer_num-1],base+furter_node+HiddenNode_num[layer_num-1]+1);
							// printf("find dlda for furter  w (layer,index) : %d,(%d + %d * %d = %d)\n\n",layer_num,closer_node,HiddenNode_num[layer_num-1],furter_node,furter_node*HiddenNode_num[layer_num-1]+closer_node);

							dlda += z_keep[base+furter_node+HiddenNode_num[layer_num-1]+1] * weights[layer_num][closer_node+furter_node*HiddenNode_num[layer_num-1]] ;
							// printf("dlda += %f * %f = ",z_keep[base+furter_node+HiddenNode_num[layer_num-1]+1],weights[layer_num][closer_node+furter_node*HiddenNode_num[layer_num-1]]);
							// printf("%f\n",z_keep[base+furter_node+HiddenNode_num[layer_num-1]+1] * weights[layer_num][closer_node+furter_node*HiddenNode_num[layer_num-1]]);
							// printf("");   
						}

						dvaluedz = (*dfunction_pointer[layer_num-1])(z_keep[base+closer_node+1]);

						// printf("dlda = %f\n",dlda);
						dldz = dlda * dvaluedz;
						z_keep[base+closer_node+1] = dldz;
						// print(number_of_node,z_keep);
						bias_adjust_record[layer_num-1][closer_node] = dldz ;

						// printf("==> dloss/ dbias : %f\n",dldz);
						
					
						node_index--;
					//printf("node index : %d\n",node_index);
					}
					// print(3,bias_adjust_record[layer_num]);
					
				}
				if (layer_num>=2) {
					// printf("base - (num of %d)       %d  -->  %d\n",layer_num-1,base,base-HiddenNode_num[layer_num-2]);
					base -= HiddenNode_num[layer_num-2];
				}
				
			}
			// print(number_of_node,z_keep);
			// printf("\n--> layer num for backprop :0\n\n");
			// printf("base variable in final phase : %d\n",base);

			
			for (int closer_node = feature_n-1 ; closer_node >= 0 ; closer_node--) { ///segmentation false//////////////////////////////////////
				//printf("colsernode : %d  going to link weight with furter node\n",closer_node);

				dlda = 0;
				// printf("dl/da : ");
				for (int furter_node = HiddenNode_num[0]-1 ; furter_node >= 0 ; furter_node--) {

					dldlast_z = z_keep[base+furter_node+1] ; 
					// printf("last z at z index : %d + %d + 1 = %d\n",base,furter_node,base+furter_node+1);

					dzdw = train_set.rows[row].feature_value[closer_node];

					// printf("==> dloss/dw%d : %f\n",closer_node,dldlast_z*dzdw);

					weight_adjust_record[0][furter_node*feature_n+closer_node] += (dldlast_z * dzdw)/train_set.size ;

					// printf("\nfind dlda for furter  z index : %d + %d + %d + 1 = %d\n",base,furter_node,HiddenNode_num[layer_num-1],base+furter_node+HiddenNode_num[layer_num-1]+1);
					// printf("find dlda for furter  w (layer,index) : %d,(%d + %d * %d = %d)\n\n",layer_num,closer_node,HiddenNode_num[layer_num-1],furter_node,furter_node*HiddenNode_num[layer_num-1]+closer_node);

				}

			}

		}

		losses[adjust_time_count] = total_loss_epoch/train_set.size;

		// printf("\n######## change weight-bias value ##############\n") ;

		for (int layer_num = 0 ; layer_num < layers+1 ; layer_num++) { 
			// printf("layer : %d\n",layer_num);
			if(layer_num == layers){
				for (int next_node = 0 ; next_node < output_num ; next_node++) {
					for (int previous_node = 0 ; previous_node < HiddenNode_num[layer_num-1] ; previous_node++) {
						weights[layer_num][next_node*HiddenNode_num[layer_num-1]+previous_node] -= weight_adjust_record[layer_num][previous_node+next_node*HiddenNode_num[layer_num-1]] * learning_rate ;
						// printf("add w : %f at (layer,node) : %d,%d\n",weight_adjust_record[layer_num][previous_node+next_node*HiddenNode_num[layer_num-1]] * learning_rate,layer_num,previous_node+HiddenNode_num[layer_num-1]*next_node);
						// printf("layer : %d\n",layer_num);
					}

					bias[layer_num][next_node] -= bias_adjust_record[layer_num][next_node] * learning_rate ;
					// printf("add b : %f at (layer,node) : %d,%d\n\n",bias_adjust_record[layer_num][next_node] * learning_rate,layer_num,next_node);
					// printf("layer : %d\n",layer_num);

				}

			}else{    
				for (int next_node = 0 ; next_node < HiddenNode_num[layer_num] ; next_node++) {
					if (layer_num == 0) {
						for (int previous_node = 0 ; previous_node < feature_n ; previous_node++) {
							weights[layer_num][next_node*feature_n+previous_node] -= weight_adjust_record[layer_num][previous_node+next_node*feature_n] * learning_rate ;
							// printf("add w : %f at (layer,node) : %d,%d\n",weight_adjust_record[layer_num][previous_node+next_node*feature_n] * learning_rate,layer_num,previous_node+feature_n*next_node);
							// printf("layer : %d\n",layer_num);
						}
					}else{
						for (int previous_node = 0 ; previous_node < HiddenNode_num[layer_num-1] ; previous_node++) {
							weights[layer_num][next_node*HiddenNode_num[layer_num-1]+previous_node] -= weight_adjust_record[layer_num][previous_node+next_node*HiddenNode_num[layer_num-1]] * learning_rate ;
							// printf("add w : %f at (layer,node) : %d,%d\n",weight_adjust_record[layer_num][previous_node+next_node*HiddenNode_num[layer_num-1]] * learning_rate,layer_num,previous_node+HiddenNode_num[layer_num-1]*next_node);
							// printf("layer : %d\n",layer_num);
						}
					}

					bias[layer_num][next_node] -= bias_adjust_record[layer_num][next_node] * learning_rate ;
					// printf("add b : %f at (layer,node) : %d,%d\n\n",bias_adjust_record[layer_num][next_node] * learning_rate,layer_num,next_node);
					// printf("layer : %d\n",layer_num);

				}
					
			}
			// printf("\n");
		}
		

	}

	// printf("\nprinting model \n\n");

	// printf("%d",HiddenNode_num[1]);

	for (int layer_num = 0 ; layer_num < layers ; layer_num++) { 
		// printf("model in layer : %d    have %d node\n",layer_num,HiddenNode_num[layer_num]);
		for (int next_node = 0 ; next_node < HiddenNode_num[layer_num] ; next_node++) {
			// printf("\nnode : %d\n",next_node);
			if (layer_num == 0) {
				for (int previous_node = 0 ; previous_node < feature_n ; previous_node++) {
					// printf("final weight : %f\n",weights[layer_num][previous_node+next_node*feature_n]);
				}
			}else {
				for (int previous_node = 0 ; previous_node < HiddenNode_num[layer_num-1] ; previous_node++) {
					// printf("final weight : %f\n",weights[layer_num][previous_node+next_node*HiddenNode_num[layer_num-1]]);
				}
			}
			

			// printf("final bias : %f\n\n\n",bias[layer_num][next_node]);

		}
				
	}
	for (int next_node = 0 ; next_node < output_num ; next_node++) {
			// printf("\nnode : %d\n",next_node);
			// printf("%d",HiddenNode_num[layers]);
			for (int previous_node = 0 ; previous_node < HiddenNode_num[layers-1] ; previous_node++) {
				// printf("final weight : %f\n",weights[layers][previous_node+next_node*HiddenNode_num[layers-1]]);
			}

			// printf("final bias : %f\n\n\n",bias[layers][next_node]);

		}
			


	printf("end of program....");
	///////////// output ////////////////////
	// memset(z_keep, 0, sizeof(float) * number_of_node);
	float z_keep2[number_of_node];
	output[0] = 0;


	float total_loss_epoch = 0 ;
		for (int row = 0 ; row < test_set.size ; row ++ ) {

			// printf("\n             run trought row : %d\n",row);


			int node_index = 0; // using with activationfunction_output
			int previouslayernode_sum = 0 ;//for tracking previous node during interaction between layers

			for (int layer_num = 0 ; layer_num < layers ; layer_num ++ ) {

				for (int node_num = 0 ; node_num < HiddenNode_num[layer_num] ; node_num++){
					sum = 0;
					if (layer_num == 0) {
						for (int previous_node = 0 ; previous_node < feature_n ; previous_node++ ) {
			
							sum += test_set.rows[row].feature_value[previous_node] * weights[layer_num][node_num*feature_n+previous_node];

						}
						
					}else{

						for (int previous_node = 0 ; previous_node < HiddenNode_num[layer_num-1]; previous_node ++ ) {

							value = (*functions_pointer[layer_num-1])(z_keep2[previouslayernode_sum + previous_node]) ; //last z

							sum += value * weights[layer_num][node_num*HiddenNode_num[layer_num-1]+previous_node] ;						
						}
						
					}
					
					sum += bias[layer_num][node_num] ; 
					
					z_keep2[node_index] = sum ;
					
					sum = (*functions_pointer[layer_num])(sum) ; 

					node_index++;
					
				}

				if (layer_num != 0) {
					previouslayernode_sum += HiddenNode_num[layer_num-1] ;
				}
				

			}

			// printf("\n-----finished forward propagation--------\n");

			for (int outputnode_num = 0 ; outputnode_num < output_num ; outputnode_num ++) { 
				
				sum = 0 ;
				for (int lasthiddennode_num = 0 ; lasthiddennode_num < HiddenNode_num[layers-1]; lasthiddennode_num++){

					value = (*functions_pointer[layers-1])(z_keep2[node_index-HiddenNode_num[layers-1]+lasthiddennode_num]); //value of last hidden

					sum += value * weights[layers][outputnode_num*HiddenNode_num[layers-1]+lasthiddennode_num]; 

				}

				sum += bias[layers][outputnode_num] ; 
				z_keep2[node_index+outputnode_num] = sum ;
				sum = (*outputfunction_pointer)(sum);
				*(output+outputnode_num) = sum ;

				// printf("output node%d's answer : %f\n",outputnode_num,sum);
			}

		// printf("-------finish answering----------\n");

		float label_arr[1];
		label_arr[0] = test_set.rows[row].label_value;

		total_loss_epoch += lfunction_pointer(label_arr, output, 1);

	}

		printf("evaluate model   loss : %f\n",total_loss_epoch/test_set.size);

	

	for(int i = 0; i < Adjust_time; i++) printf("Loss epoch %d: %f\n", i+1, losses[i]);

	FILE *graph = fopen("losses.txt", "w");
	
	for(int i = 0; i < Adjust_time; i++){
		fprintf(graph, "%f\n", losses[i]);
	}
	
	fclose(graph);



	free(all_data); // Free Part 1
	free(HiddenNode_num);
	free(weights);
	free(bias);



	return 0;
}
