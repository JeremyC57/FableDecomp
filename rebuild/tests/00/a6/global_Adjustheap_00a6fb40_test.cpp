// behaviour test for 00a6fb40: _Adjust_heap over (CCharString name, C3DDummyObject) pairs by CCharString::FastCompare
#include <cstdio>
#include <cstring>

// CCharString model: a shared payload pointer; operator< as in retail (same payload -> false,
// null this -> true, null other -> false, else compare the payload text)
struct CBasicStringChar {
    char Text[8];
    bool operator<(const CBasicStringChar& other) const { return std::strcmp(Text, other.Text) < 0; }
};
static CBasicStringChar g_payloads[16];
class CCharString { public:
    CBasicStringChar* PStringData;
    CCharString() : PStringData(0) { }
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData) return false;
        if (!PStringData) return true;
        if (!other.PStringData) return false;
        return *PStringData < *other.PStringData;
    }
};
static void MakeString(CCharString& s, int key, long i)
{
    std::sprintf(g_payloads[i].Text, "%03d", key + 100);
    s.PStringData = &g_payloads[i];
}
static int StringKey(const CCharString& s) { int v = 0; std::sscanf(s.PStringData->Text, "%d", &v); return v - 100; }

static bool FastCompare(const CCharString& a, const CCharString& b) { return a < b; }
class CMatrix3x4 { public: float M[3][4]; };
class C3DDummyObject { public: CMatrix3x4 Transform; long BoneIndex; };
struct TEntry { CCharString first; C3DDummyObject second; };
struct TCompare { char m_tag; bool operator()(const TEntry& a, const TEntry& b) const { return FastCompare(a.first, b.first); } };
typedef TEntry T;
typedef TCompare C;
static C MakePred() { C c; c.m_tag = 0; return c; }
static void Fill(T& t, int key, long i) { MakeString(t.first, key, i); int r = 0; int c = 0; for (r = 0; r < 3; ++r) for (c = 0; c < 4; ++c) t.second.Transform.M[r][c] = (float)(key + r * 4 + c); t.second.BoneIndex = key; }
static int Key(const T& t) { return StringKey(t.first); }
static bool Intact(const T& t, const int*) { return t.second.BoneIndex == Key(t) && t.second.Transform.M[2][3] == (float)(Key(t) + 11); }

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

    if (!ok) { std::printf("00a6fb40_TEST FAIL\n"); return 1; }
    std::printf("00a6fb40_TEST PASS\n");
    return 0;
}