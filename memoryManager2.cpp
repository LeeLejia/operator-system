#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <iostream>
using namespace std;
int main() {
    int *string;
    /* Allocate space for a path name */
    string = (int *) malloc(10);
    if (string == NULL)
        printf("Insufficient memory available\n");
    else {
        printf("Memory space allocated for path name\n");
        cout << "string=" << string << endl;
        free(string);
        printf("Memory freed\n");
    }
    int *stringy;

    /* Allocate space for a path name */
    stringy = (int *) malloc(12);
    if (stringy == NULL)
        printf("Insufficient memory available\n");
    else {
        printf("Memory space allocated for path name\n");
        /*printf ("string=%d\n",string);*/
        cout << "stringy=" << stringy << endl;
        free(stringy);
        printf("Memory freed\n");
    }
    return 0;
}