// Behaviour test for 0x0081AB40 __adjust_heap<pair<long, 36-byte payload>> (model copy): sift-down + push_heap.
#include <cstdio>
struct SPayload36 { unsigned int m_words[9]; };
struct SKeyedBlock40 {
    long       first;
    SPayload36 second;
    SKeyedBlock40() : first(0) { for (int i = 0; i < 9; ++i) second.m_words[i] = 0; }
    SKeyedBlock40(const SKeyedBlock40& other) : first(other.first), second(other.second) { }
};
struct Pred {
    bool operator()(const SKeyedBlock40& a, const SKeyedBlock40& b) const { return a.first < b.first; }
};
typedef SKeyedBlock40 E;
static void push_heap_model(E* first, int holeIndex, int topIndex, E val, Pred comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(first[parent], val)) {
        first[holeIndex] = first[parent];
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    first[holeIndex] = val;
}
static void adjust_heap_model(E* first, int holeIndex, int len, E val, Pred comp)
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
    push_heap_model(first, holeIndex, topIndex, val, comp);
}
int main()
{
    static const long keys[10] = { 5, -1, 9, 3, 7, 2, 8, 6, 4, 0 };
    E v[10];
    int i;
    for (i = 0; i < 10; ++i) { v[i].first = keys[i]; v[i].second.m_words[8] = (unsigned int)(keys[i] + 100); }
    Pred p;
    for (i = (10 - 2) / 2; i >= 0; --i) adjust_heap_model(v, i, 10, v[i], p);    // make_heap
    int ok = 1;
    for (i = 1; i < 10; ++i) if (v[(i - 1) / 2].first < v[i].first) ok = 0;
    if (v[0].first != 9) ok = 0;
    int n;
    for (n = 10; n > 1; --n) { E t = v[n - 1]; v[n - 1] = v[0]; adjust_heap_model(v, 0, n - 1, t, p); }  // sort_heap
    static const long want[10] = { -1, 0, 2, 3, 4, 5, 6, 7, 8, 9 };
    for (i = 0; i < 10; ++i) if (v[i].first != want[i] || v[i].second.m_words[8] != (unsigned int)(want[i] + 100)) ok = 0;
    if (!ok) { std::printf("0081ab40_TEST FAIL\n"); return 1; }
    std::printf("0081ab40_TEST PASS\n");
    return 0;
}