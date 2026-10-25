// Behaviour test for 0x00758910 std::__adjust_heap over the 0x2C-byte wound-morph entries
#include <cstdio>

struct CWoundMorphsEntry { long Words[10]; };
template <class A, class B> struct pair {
    A first;
    B second;
    pair() {}
    pair(const pair& other) : first(other.first), second(other.second) {}
};
typedef pair<int, CWoundMorphsEntry> TWoundPair;
struct CKeyPairCompareLess {
    bool operator()(const TWoundPair& a, const TWoundPair& b) const { return a.first < b.first; }
};

static int g_pushCalls = 0;
void __push_heap(TWoundPair* first, int holeIndex, int topIndex, TWoundPair val, CKeyPairCompareLess comp)
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

void __adjust_heap(TWoundPair* first, int holeIndex, int len, TWoundPair val, CKeyPairCompareLess comp)
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

static bool IsHeap(const TWoundPair* a, int n)
{
    for (int i = 1; i < n; ++i) if (a[(i - 1) / 2].first < a[i].first) return false;
    return true;
}

int main()
{
    int ok = 1;
    for (int n = 1; n <= 12; ++n) {
        // a valid max-heap of descending keys, then replace the root with a small value
        TWoundPair a[12];
        for (int i = 0; i < n; ++i) { a[i].first = 100 - i * 3; for (int k = 0; k < 10; ++k) a[i].second.Words[k] = a[i].first + k; }
        TWoundPair v;
        v.first = 50 - n;
        for (int k = 0; k < 10; ++k) v.second.Words[k] = v.first + k;
        int before = g_pushCalls;
        int sum = 0;
        for (int i = 1; i < n; ++i) sum += a[i].first;
        __adjust_heap(a, 0, n, v, CKeyPairCompareLess());
        if (g_pushCalls != before + 1) ok = 0;
        if (!IsHeap(a, n)) ok = 0;
        int sum2 = 0;
        for (int i = 0; i < n; ++i) { sum2 += a[i].first; for (int k = 0; k < 10; ++k) if (a[i].second.Words[k] != a[i].first + k) ok = 0; }
        if (sum2 != sum + v.first) ok = 0;
    }
    if (!ok) { std::printf("00758910_TEST FAIL\n"); return 1; }
    std::printf("00758910_TEST PASS\n");
    return 0;
}