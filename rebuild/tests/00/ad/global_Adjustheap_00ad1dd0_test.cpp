// behaviour test for 00ad1dd0: _Adjust_heap over CMatchProbability under a function-pointer predicate (by Probability)
#include <cstdio>
#include <cstring>

struct CMatchProbability { unsigned long Index; float Probability; };
static bool LessProbability(const CMatchProbability& a, const CMatchProbability& b) { return a.Probability < b.Probability; }
typedef bool (*TMatchCompare)(const CMatchProbability&, const CMatchProbability&);
typedef CMatchProbability T;
typedef TMatchCompare C;
static C MakePred() { return LessProbability; }
static void Fill(T& t, int key, long i) { t.Probability = (float)key * 0.25f; t.Index = (unsigned long)(key + 1000); }
static int Key(const T& t) { return (int)(t.Probability * 4.0f); }
static bool Intact(const T& t, const int*) { return t.Index == (unsigned long)(Key(t) + 1000); }

static void PushHeap(T* first, long hole, long top, T value, C pred)
{
    long parent = (hole - 1) / 2;
    while (top < hole && pred(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void AdjustHeap(T* first, long hole, long bottom, T value, C pred)
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
    static const int keys[11] = { 7, -2, 15, 3, 3, 0, 42, -9, 11, 5, 8 };
    T a[11];
    C pred = MakePred();
    long i = 0;
    long n = 11;
    long last = 0;
    for (i = 0; i < n; ++i) Fill(a[i], keys[i], i);
    for (i = n / 2; 0 < i; ) { --i; T v = a[i]; AdjustHeap(a, i, n, v, pred); }
    int ok = 1;
    for (i = 1; i < n; ++i) if (pred(a[0], a[i])) ok = 0;
    for (last = n; 1 < last; --last) {
        T value = a[last - 1];
        a[last - 1] = a[0];
        AdjustHeap(a, 0, last - 1, value, pred);
    }
    for (i = 1; i < n; ++i) if (pred(a[i], a[i - 1])) ok = 0;
    for (i = 0; i < n; ++i) if (!Intact(a[i], keys)) ok = 0;
    if (Key(a[0]) != -9 || Key(a[n - 1]) != 42) ok = 0;

    if (!ok) { std::printf("00ad1dd0_TEST FAIL\n"); return 1; }
    std::printf("00ad1dd0_TEST PASS\n");
    return 0;
}