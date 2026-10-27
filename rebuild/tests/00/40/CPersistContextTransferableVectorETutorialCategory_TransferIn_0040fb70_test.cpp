// behaviour test for 0x0040FB70 CPersistContext_TransferableVector<ETutorialCategory>::TransferIn (model: the parsed value lands at index)
#include <cstdio>
static int g_next = 42;
static void ReadLong(int* v) { *v = g_next; }
struct Vec { int* start; };
struct TV { Vec* TransVector; void TransferIn(int index) { int value; ReadLong(&value); TransVector->start[index] = value; } };
int main()
{
    int data[3] = { 0, 0, 0 }; Vec v = { data }; TV tv; tv.TransVector = &v;
    tv.TransferIn(2);
    int ok = data[2] == 42 && data[0] == 0 && data[1] == 0;
    if (!ok) { std::printf("0040fb70_TEST FAIL\n"); return 1; }
    std::printf("0040fb70_TEST PASS\n");
    return 0;
}