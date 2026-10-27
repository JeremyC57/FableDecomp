// behaviour test for 0x0040FD40: GFSerialiseVectorBinaryOut writes the count then each element
#include <cstdio>
static long g_out[16]; static int g_n = 0;
class CDataOutputStream { public: void WriteSLONG(long v) { g_out[g_n++] = v; } };
static void Serialise(CDataOutputStream& out, long* first, long* last)
{
    long count = (long)(last - first); out.WriteSLONG(count);
    for (long i = 0; i < count; ++i) out.WriteSLONG(first[i]);
}
int main()
{
    long v[3] = { 4, 7, 2 }; CDataOutputStream out;
    Serialise(out, v, v + 3);
    Serialise(out, v, v);
    if (g_n != 5 || g_out[0] != 3 || g_out[1] != 4 || g_out[3] != 2 || g_out[4] != 0) { std::printf("0040fd40_TEST FAIL\n"); return 1; }
    std::printf("0040fd40_TEST PASS\n");
    return 0;
}