// behaviour test for 00a5a580: _Adjust_heap over a float array (less-than = max-heap), driven
// through make_heap/sort_heap; sorting must give ascending floats.
#include <cstdio>
struct SFloatLess {
    char m_tag;
    bool operator()(float a, float b) const { return a < b; }
};
static void PushHeap(float* first, long hole, long top, float value, SFloatLess pred)
{
    long parent = (hole - 1) / 2;
    while (top < hole && pred(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void AdjustHeap(float* first, long hole, long bottom, float value, SFloatLess pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    PushHeap(first, hole, top, value, pred);
}
int main()
{
    float a[10] = { 3.5f, -1.0f, 9.25f, 4.0f, 4.0f, 0.5f, 7.0f, 2.0f, -8.0f, 100.0f };
    SFloatLess pred;
    pred.m_tag = 0;
    long i = 0;
    long n = 10;
    long last = 0;
    for (i = n / 2; 0 < i; ) { --i; AdjustHeap(a, i, n, a[i], pred); }
    int ok = 1;
    if (a[0] != 100.0f) ok = 0;
    for (last = n; 1 < last; --last) {
        float value = a[last - 1];
        a[last - 1] = a[0];
        AdjustHeap(a, 0, last - 1, value, pred);
    }
    for (i = 1; i < n; ++i) if (a[i - 1] > a[i]) ok = 0;
    if (a[0] != -8.0f || a[9] != 100.0f) ok = 0;
    if (!ok) { std::printf("00a5a580_TEST FAIL\n"); return 1; }
    std::printf("00a5a580_TEST PASS\n");
    return 0;
}