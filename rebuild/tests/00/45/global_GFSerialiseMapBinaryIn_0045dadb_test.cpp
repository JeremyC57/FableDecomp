// behaviour test for 0x0045DADB GFSerialiseMapBinaryIn (map<ETargetingType, float>): the map is cleared, the count read first, then count (key, value) pairs read in order and inserted; a non-positive count inserts nothing
#include <cstdio>
static int gStream[16]; static int gPos = 0;
static int ReadLong() { return gStream[gPos++]; }
struct Map { int Keys[8], Vals[8], N, Clears; void clear() { N = 0; ++Clears; } void insert(int k, int v) { for (int i = 0; i < N; ++i) if (Keys[i] == k) return; Keys[N] = k; Vals[N] = v; ++N; } };
static void SerialiseIn(Map& out)                            // the function under test
{
    out.clear();
    long count = ReadLong();
    for (long i = 0; i < count; ++i) {
        int key = 0, value = 0;
        key = ReadLong();
        value = ReadLong();
        out.insert(key, value);
    }
}
int main()
{
    int fail = 0;
    Map m; m.N = 3; m.Clears = 0;
    int data[] = { 3, 7, 70, 2, 20, 7, 99, -1 };
    for (int i = 0; i < 8; ++i) gStream[i] = data[i];
    SerialiseIn(m);
    if (m.Clears != 1 || m.N != 2 || m.Keys[0] != 7 || m.Vals[0] != 70 || m.Keys[1] != 2 || m.Vals[1] != 20 || gPos != 7) fail |= 1;
    SerialiseIn(m);
    if (m.Clears != 2 || m.N != 0 || gPos != 8) fail |= 2;
    if (fail) { std::printf("0045dadb_TEST FAIL %d\n", fail); return 1; }
    std::printf("0045dadb_TEST PASS\n");
    return 0;
}