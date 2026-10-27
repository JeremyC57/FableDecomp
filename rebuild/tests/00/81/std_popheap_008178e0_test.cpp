// Behaviour test for 0x008178E0: STLport __pop_heap core over 32-byte hit-location elements: result takes the root, the value sifts down
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SElem { float Distance; long Pad[7]; };
static bool Pred(const SElem& a, const SElem& b) { return a.Distance > b.Distance; }   // min-heap on distance
static void PushHeap(SElem* f, int hole, int top, SElem v)
{
    int parent = (hole - 1) / 2;
    while (hole > top && Pred(f[parent], v)) { f[hole] = f[parent]; hole = parent; parent = (hole - 1) / 2; }
    f[hole] = v;
}
static void AdjustHeap(SElem* f, int hole, int len, SElem v)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) {
        if (Pred(f[child], f[child - 1])) child--;
        f[hole] = f[child]; hole = child; child = 2 * (child + 1);
    }
    if (child == len) { f[hole] = f[child - 1]; hole = child - 1; }
    PushHeap(f, hole, top, v);
}
static void PopHeap(SElem* first, SElem* last, SElem* result, SElem val)
{
    *result = *first;
    AdjustHeap(first, 0, (int)(last - first), val);
}
int main()
{
    SElem h[6];
    float d[6] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f };
    for (int i = 0; i < 6; ++i) { h[i].Distance = d[i]; for (int k = 0; k < 7; ++k) h[i].Pad[k] = i; }
    SElem val = h[5];
    PopHeap(h, h + 5, h + 5, val);
    CHECK(h[5].Distance == 1.0f && h[5].Pad[0] == 0);
    CHECK(h[0].Distance == 2.0f);
    for (int i = 1; i < 5; ++i) CHECK(!Pred(h[(i - 1) / 2], h[i]));
    if (!g_fail) printf("008178e0_TEST PASS\n");
    return g_fail;
}