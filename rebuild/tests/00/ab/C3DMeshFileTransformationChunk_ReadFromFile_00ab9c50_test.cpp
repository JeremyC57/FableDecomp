#include <cstdio>

struct M { float m[12]; };
struct In { unsigned long pos; In() : pos(10) {} unsigned long GetPosition() { return pos; } M ReadMatrix() { M r; for (int i = 0; i < 12; ++i) r.m[i] = (float)i; pos += 48; return r; } };
struct T { M Transform; unsigned long baseSize;
    void Base(In&, unsigned long size) { baseSize = size; }
    void ReadFromFile(In& in, unsigned long chunkSize) { unsigned long start = in.GetPosition(); Transform = in.ReadMatrix(); Base(in, chunkSize - (in.GetPosition() - start)); } };
int main()
{
    T t; In in; t.ReadFromFile(in, 100); if (t.baseSize != 52 || t.Transform.m[5] != 5.0f) { std::printf("00ab9c50_TEST FAIL\n"); return 1; }
    std::printf("00ab9c50_TEST PASS\n");
    return 0;
}