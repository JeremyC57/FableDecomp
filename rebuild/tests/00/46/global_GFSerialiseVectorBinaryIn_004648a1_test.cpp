// behaviour test for 0x004648A1 GFSerialiseVectorBinaryIn (vector<unsigned long>): the count is read first, the vector resized to it (new elements
// default-constructed, existing ones kept), then each element read in order; a zero count empties the vector
#include <cstdio>
static int gStream[16]; static int gPos = 0;
static int ReadLong() { return gStream[gPos++]; }
struct Vec {
    int Data[8]; int N;
    void resize(int n, int x) { for (int i = N; i < n; ++i) Data[i] = x; N = n; }
};
static void SerialiseIn(Vec& out)                               // the function under test
{
    long count = ReadLong();
    out.resize((int)count, 0);
    for (long i = 0; i < count; ++i)
        out.Data[i] = ReadLong();
}
int main()
{
    int fail = 0;
    Vec v; v.N = 5;
    for (int i = 0; i < 8; ++i) v.Data[i] = 100 + i;
    int data[] = { 3, 7, 8, 9, 0, 1, 5 };
    for (int i = 0; i < 7; ++i) gStream[i] = data[i];
    SerialiseIn(v);
    if (v.N != 3 || v.Data[0] != 7 || v.Data[1] != 8 || v.Data[2] != 9 || gPos != 4) fail |= 1;
    SerialiseIn(v);
    if (v.N != 0 || gPos != 5) fail |= 2;
    SerialiseIn(v);
    if (v.N != 1 || v.Data[0] != 5 || gPos != 7) fail |= 4;
    if (fail) { std::printf("004648a1_TEST FAIL %d\n", fail); return 1; }
    std::printf("004648a1_TEST PASS\n");
    return 0;
}