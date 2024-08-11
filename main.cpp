#include "main.h"
#include "restaurant.cpp"
#include <time.h>
int main() {
    string fileName = "test.txt";
    //clock_t start = clock();
    simulate(fileName);
    // clock_t end = clock();
    // double timeuse = double(end-start)/CLOCKS_PER_SEC;
    // cout<<"Time run: "<<timeuse;
    return 0;
}
