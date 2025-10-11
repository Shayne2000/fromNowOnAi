#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char buffer[1000];
    char *data;
    printf("====================================\n");
    FILE *test;
    test = fopen("test.csv", "r"); // Open a file in read mode
    if (test == NULL) // Check if the file does not exist
    {
        printf("File doesn't exist\n");
        exit(-1);
    }

    /*fgets(buffer, sizeof(buffer), test);
    printf("%s", buffer); */ // Only read fist line

    while (fgets(buffer, sizeof(buffer), test))
    {
        printf("%s", buffer);
    }
    
    data = strtok(buffer,",");
    printf("\nFirst token is %s", data);

    data = strtok(NULL,",");
    printf("\nSecond token is %s", data);

    data = strtok(NULL,",");
    printf("\nThird token is %s", data);

    printf("\n====================================\n");
    fclose(test); // Close the file
    return 0;
}
