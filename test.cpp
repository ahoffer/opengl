
#include<iostream>
#include <stdlib.h>

int main(int argc, char **argv) {
    int numTriangles = 3;
    float * pos = (float *) malloc((numTriangles+1) * 2 *3 * sizeof(float));
    float * ptr = pos;
    float step = 2.0f / numTriangles;
    for (int i = 0; i <= numTriangles; i++) {
        float x = i * step - 1;
        *ptr = x;
        ptr++;
        *ptr = 1.0f;
        ptr++;
        *ptr = 0.0f;
        ptr++;
        *ptr = x;
        ptr++;
        *ptr = 0.0f;
        ptr++;
        *ptr = 0.0f;
        ptr++;
    }
    int len = 24;
    std::cout << "len=" << len << std::endl;
    for (int x=0; x<len; x++) {
        std::cout << pos[x] << " ";
    }
    std::cout << std::endl;
}