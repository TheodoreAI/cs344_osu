#include <stdio.h>

// This will be useful for printing purposes
// double num_sol = 100.98;
// double double_one = 98.98;
// double double_two = 87.98;

// int main(void){
// 	printf("The doubles: %f, %f\n", double_one, double_two);
// 	return 0;
// }


float expon(double d, int e){
	if(e == 0){
		return 1;
	}else{
		return d*expon(d, e-1);
	}
}

int main(void){
	printf("The expon function: %f\n", expon(2.0, 0));
	return 0;
}