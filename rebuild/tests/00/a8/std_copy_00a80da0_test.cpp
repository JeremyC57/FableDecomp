// behaviour test for 0x00A80DA0: the copy assigns each pair and keeps counted-pointer references balanced
#include <cstdio>
struct Info { unsigned long refs; };
struct E { long k; void* d; Info* i; };
static int g_deleted = 0;
static void Assign(E& dst, const E& src)
{
    dst.k = src.k; Info* info = src.i; void* data = src.d;
    if (dst.i != info) { if (dst.i && --dst.i->refs == 0) ++g_deleted; dst.d = data; dst.i = info; if (info) ++info->refs; }
}
static E* Copy(E* first, E* last, E* result) { for (long n = (long)(last - first); n > 0; --n) { Assign(*result, *first); ++first; ++result; } return result; }
int main()
{
    Info a = { 1 }, b = { 1 }, c = { 1 };
    E v[3] = { { 1, 0, &a }, { 2, 0, &b }, { 3, 0, &c } };
    E* r = Copy(v + 1, v + 3, v);
    if (r != v + 2 || v[0].k != 2 || v[0].i != &b || v[1].i != &c || b.refs != 1 || c.refs != 2 || g_deleted != 1) { std::printf("00a80da0_TEST FAIL\n"); return 1; }
    std::printf("00a80da0_TEST PASS\n");
    return 0;
}