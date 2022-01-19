
#include<iostream>
#include <stdlib.h>

using namespace std;

void test1();

float *test2();

int main(int argc, char **argv) {
    test2();
}

void test1() {
    float span = 2.0f;
    int divisions = 3;
    int floats_per_point = 3;
    int num_vertices = divisions + 1;
    float pos[num_vertices][num_vertices][floats_per_point];
    float step = span / (float) divisions;
    cout << fixed;
    for (int row = 0; row <= divisions; row++) {
        float y = step * row - 1;
        for (int col = 0; col <= divisions; col++) {
            // -1 to translate it back to {-1,1} space
            float x = step * col - 1;
            cout << x << "," << y << endl;
            pos[row][col][0] = x;
            pos[row][col][1] = y;
            pos[row][col][2] = 0.0f;
        }
    }
}

float *test2() {
    float span = 2.0f;
    int divisions = 3;
    int floats_per_point = 3;
    int num_vertices = divisions + 1;
    int num_floats = num_vertices * num_vertices * floats_per_point;
    float *pos = (float *) malloc(num_floats * sizeof(float));
    float step = span / (float) divisions;
    cout << fixed;
    for (int row = 0; row < num_vertices; row++) {
        float y = step * row - 1;
        for (int col = 0; col < num_vertices; col++) {
            // -1 to translate it back to {-1,1} space
            float x = step * col - 1;
            int vertex_idx = num_vertices * row * floats_per_point+ col *floats_per_point;
            cout << "vert_idx=" << vertex_idx << endl;
            pos[vertex_idx] = x;
            pos[vertex_idx + 1] = y;
            pos[vertex_idx + 2] = 0.0f;
        }
    }
    float *ptr = pos;
    for (int a = 0; a < num_floats; a += 3)
        cout << "a=" << a << " (" << ptr[a] << "," << ptr[a + 1] << "," << ptr[a + 2] << ")" << endl;
    return pos;
}
