// behaviour test for 00497160: _Adjust_heap over wide strings ordered case-insensitively (_wcsicmp < 0)
#include <cstdio>
#include <cstring>

static int IcmpText(const char* a, const char* b)
{
    for (;; ++a, ++b) {
        int ca = (*a >= 'A' && *a <= 'Z') ? *a + 32 : *a;
        int cb = (*b >= 'A' && *b <= 'Z') ? *b + 32 : *b;
        if (ca != cb) return ca - cb;
        if (ca == 0) return 0;
    }
}
static char g_text[16][8];
struct CWideString { const char* Text; };
struct CCaseInsensitiveStringCompare { char m_tag; bool operator()(const CWideString& a, const CWideString& b) const { return IcmpText(a.Text, b.Text) < 0; } };
typedef CWideString T;
typedef CCaseInsensitiveStringCompare C;
static C MakePred() { C c; c.m_tag = 0; return c; }
static void Fill(T& t, int key, long i) { std::sprintf(g_text[i], (i & 1) ? "K%03d" : "k%03d", key + 100); t.Text = g_text[i]; }
static int Key(const T& t) { int v = 0; std::sscanf(t.Text + 1, "%d", &v); return v - 100; }
static bool Intact(const T& t, const int*) { return t.Text[0] == 'k' || t.Text[0] == 'K'; }

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

    if (!ok) { std::printf("00497160_TEST FAIL\n"); return 1; }
    std::printf("00497160_TEST PASS\n");
    return 0;
}