// behaviour test for 0x006144F0: TransferOut writes the indexed element (as a long)
#include <cstdio>
static long g_written = -1;
static void WriteLong(const long& v) { g_written = v; }
struct V { long* s; };
struct TV { V* vec; void TransferOut(long index) { WriteLong((long)vec->s[index]); } };
int main()
{
    long data[3] = { 5, 9, 2 }; V v = { data }; TV t = { &v };
    t.TransferOut(1); long a = g_written; t.TransferOut(2);
    if (a != 9 || g_written != 2) { std::printf("006144f0_TEST FAIL\n"); return 1; }
    std::printf("006144f0_TEST PASS\n");
    return 0;
}