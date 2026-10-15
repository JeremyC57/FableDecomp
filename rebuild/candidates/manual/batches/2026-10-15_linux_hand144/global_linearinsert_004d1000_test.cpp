// Behaviour test for 0x004d1000 (__linear_insert (28-byte float-keyed records)): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct SRec { float key; long payload[6]; };
struct SLess { bool operator()(const SRec& a, const SRec& b) const { return a.key < b.key; } };
static SRec* CopyBackward(SRec* first, SRec* last, SRec* result) { while (last != first) *--result = *--last; return result; }
static void UnguardedLinearInsert(SRec* last, SRec value, SLess comp) { SRec* next = last - 1; while (comp(value, *next)) { *last = *next; last = next; --next; } *last = value; }
static void LinearInsert(SRec* first, SRec* last, SRec value, SLess comp)
{
    if (comp(value, *first)) { CopyBackward(first, last, last + 1); *first = value; }
    else UnguardedLinearInsert(last, value, comp);
}
static void Run()
{
    SRec r[5]; float keys[5] = { 3, 1, 4, 0.5f, 2 };
    for (int i = 0; i < 5; ++i) { r[i].key = keys[i]; for (int j = 0; j < 6; ++j) r[i].payload[j] = i * 10 + j; }
    SLess less;
    for (int i = 1; i < 5; ++i) LinearInsert(r, r + i, r[i], less);
    for (int i = 1; i < 5; ++i) CHECK(r[i - 1].key <= r[i].key);
    CHECK(r[0].key == 0.5f && r[0].payload[0] == 30 && r[4].key == 4.0f && r[4].payload[5] == 25);
}

int main()
{
    Run();
    if (g_fail) { std::printf("004d1000_TEST FAIL\n"); return 1; }
    std::printf("004d1000_TEST PASS\n");
    return 0;
}
