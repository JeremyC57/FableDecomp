// Behaviour test for 0x0067EAD0: STLport __make_heap over 28-byte stages (descending-health comparator -> min-heap on health)
#include <cstdio>

struct Info { float Health; unsigned char pad[16]; int Emitter; bool Blocks; };
struct GT { bool operator()(const Info& a, const Info& b) const { return a.Health > b.Health; } };

static void AdjustHeap(Info* first, int hole, int len, Info v, GT comp)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) {
        if (comp(first[child], first[child - 1])) child--;
        first[hole] = first[child]; hole = child; child = 2 * (child + 1);
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], v)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = v;
}
static void MakeHeap(Info* first, Info* last, GT comp)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        AdjustHeap(first, parent, len, Info(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}

int main()
{
    float h[10] = { 0.5f, 0.9f, 0.1f, 0.7f, 0.3f, 0.8f, 0.2f, 0.6f, 0.4f, 0.05f };
    Info a[10];
    for (int i = 0; i < 10; ++i) { a[i].Health = h[i]; a[i].Emitter = i; }
    MakeHeap(a, a + 10, GT());
    // heap property under comp: no child "greater-first" than its parent, i.e. parent.Health <= child.Health
    for (int i = 1; i < 10; ++i) if (a[(i - 1) / 2].Health > a[i].Health) { std::printf("0067ead0_TEST FAIL\n"); return 1; }
    if (a[0].Emitter != 9) { std::printf("0067ead0_TEST FAIL\n"); return 1; }
    Info one[1]; one[0].Health = 1.0f; one[0].Emitter = 7;
    MakeHeap(one, one + 1, GT());
    if (one[0].Emitter != 7) { std::printf("0067ead0_TEST FAIL\n"); return 1; }
    std::printf("0067ead0_TEST PASS\n");
    return 0;
}