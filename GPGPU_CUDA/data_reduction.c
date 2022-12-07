#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int max=128;
    int array[max];
    int sum, loop;
    int i;

    for  (i=0;i<=max;i++) {
        array[i] = pow(2,i);
    }
    sum = 0;

    for (loop = max; loop>=0; loop--) {
        sum = sum + array[loop];
    }

    printf("Sum of array is %d.", sum);

    return 0;
}