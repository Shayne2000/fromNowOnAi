#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define hidden 4 //ใส่ไว้ก่อน

//Activaton function
double sigmoid(double x){//sigmoid
    return 1.0/(1.0 + exp(-x));
}



int main(){
    int fea_amount;//จำนวน feature
    scanf("%d",fea_amount);
    double x[fea_amount];


    char *str = (char*)malloc((fea_amount+1) * sizeof(char));//จองหน่วยความจำ feature
    if(str == NULL)free(str);


    srand(time(NULL));

        for(int i=0; i<hidden; i++){
            for(int j=0; j<hidden; j++){
                double w_input_hidden[i][j] = ((double)rand()/RAND_MAX);//random weight *still error*
            }
        }

    double sum;
    for(int i=0; i<fea_amount; i++){
        for(int j=0; j<)
    }



    free(str);
    return 0;
}
