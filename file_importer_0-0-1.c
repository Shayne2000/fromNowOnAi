#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {

    system("cls");
    char file_name[100];
    while (1) {

        //input file name
        printf("Input your file name (Must be .csv file): ");
        fgets(file_name, sizeof(file_name), stdin);
        file_name[strcspn(file_name, "\n")] = '\0';  
        system("cls"); // clear console

        // Check if file has .csv 
        if (strstr(file_name, ".csv") == NULL) {
            printf("==============================================\n");
            printf("Invalid file type. Please enter (.csv) file.\n");
            printf("==============================================\n");
            continue;
        }

        // check file name in the folder
        FILE *import_file = fopen(file_name, "r"); // read file as a reader "r"
        if (import_file == NULL) {
            
            printf("==============================================\n");
            printf("There is no file named: %s\nPlease input again.\n", file_name);
            printf("==============================================\n");
            continue;
        } else {
            system("cls"); // clear console
            printf("==============================================\n");
            printf("File \"%s\" opened successfully!\n", file_name);
            printf("==============================================\n\n");
            char first_row[250];
            fgets(first_row, sizeof(first_row), import_file);
            first_row[strcspn(first_row, "\n")] = '\0';
            
            // show selection feature and label 
            int j = 1;
            for(int i = 0; first_row[i] != '\0'; i++){

                if(first_row[i] != ',' && first_row[i] != '\"'){
                    printf("%c", first_row[i], i);
                }else if(first_row[i] == ',')printf(" (%d) ", j++);
            }
            printf("(%d) ", j);

            fclose(import_file);
            break;  // Exit the loop after finish processing
        }
    }


    return 0;
}
