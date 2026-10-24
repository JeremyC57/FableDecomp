// Behaviour test for 0x00758C90 _STL::__make_heap over the 0x2C-byte wound-morph entries
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

static int g_adjustCalls = 0;
void __push_heap(TWoundPair* first, int holeIndex, int topIndex, TWoundPair val, CKeyPairCompareLess comp)
{
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
    ++g_adjustCalls;
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

void __make_heap(TWoundPair* first, TWoundPair* last, CKeyPairCompareLess comp, TWoundPair*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, TWoundPair(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}

int main()
{
    int ok = 1;
    int src[11] = { 3, -7, 12, 5, 5, 0, 99, -2, 41, 8, 1 };
    for (int n = 0; n <= 11; ++n) {
        TWoundPair a[11];
        for (int i = 0; i < n; ++i) { a[i].first = src[i]; for (int k = 0; k < 10; ++k) a[i].second.Words[k] = src[i] + k; }
        int before = g_adjustCalls;
        __make_heap(a, a + n, CKeyPairCompareLess(), (TWoundPair*)0, (int*)0);
        int expectCalls = n < 2 ? 0 : (n - 2) / 2 + 1;
        if (g_adjustCalls - before != expectCalls) ok = 0;
        for (int i = 1; i < n; ++i) if (a[(i - 1) / 2].first < a[i].first) ok = 0;
        int sum = 0, want = 0;
        for (int i = 0; i < n; ++i) { sum += a[i].first; want += src[i]; for (int k = 0; k < 10; ++k) if (a[i].second.Words[k] != a[i].first + k) ok = 0; }
        if (sum != want) ok = 0;
    }
    if (!ok) { std::printf("00758c90_TEST FAIL\n"); return 1; }
    std::printf("00758c90_TEST PASS\n");
    return 0;
}