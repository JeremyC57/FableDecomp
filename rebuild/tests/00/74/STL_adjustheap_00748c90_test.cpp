// Behaviour test for 0x00748C90 _STL::__adjust_heap(CThing**, int, int, CThing*, CCompare_FurthestFromInitPos2D)
#include <cstdio>

struct CThing { int key; };
struct CCompare_FurthestFromInitPos2D {
    bool operator()(CThing* a, CThing* b) const { return a->key < b->key; }   // stand-in: max-heap on key
};

static int g_pushCalls = 0;
void __push_heap(CThing** first, int holeIndex, int topIndex, CThing* val, CCompare_FurthestFromInitPos2D comp)
{
    ++g_pushCalls;
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}

void __adjust_heap(CThing** first, int holeIndex, int len, CThing* val, CCompare_FurthestFromInitPos2D comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, val, comp);
}

int main()
{
    int ok = 1;
    for (int n = 1; n <= 10; ++n) {
        CThing t[10];
        CThing* p[10];
        for (int i = 0; i < n; ++i) { t[i].key = 100 - 7 * i; p[i] = &t[i]; }
        CThing v; v.key = 40 - n;
        int sum = 0;
        for (int i = 1; i < n; ++i) sum += p[i]->key;
        int before = g_pushCalls;
        __adjust_heap(p, 0, n, &v, CCompare_FurthestFromInitPos2D());
        if (g_pushCalls != before + 1) ok = 0;
        int sum2 = 0, seenV = 0;
        for (int i = 0; i < n; ++i) { sum2 += p[i]->key; if (p[i] == &v) ++seenV; }
        if (sum2 != sum + v.key || seenV != 1) ok = 0;
        for (int i = 1; i < n; ++i) if (p[(i - 1) / 2]->key < p[i]->key) ok = 0;
    }
    if (!ok) { std::printf("00748c90_TEST FAIL\n"); return 1; }
    std::printf("00748c90_TEST PASS\n");
    return 0;
}