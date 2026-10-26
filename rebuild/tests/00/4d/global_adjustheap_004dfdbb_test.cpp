// behaviour test for 004dfdbb: SGI __adjust_heap over the registered-TC table entries (CCharString key, CRegisteredTCInfo) by name
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

struct CRegisteredTCInfo { void* AllocFunc; unsigned long DriverType; CCharString Name; };
struct TEntry { CCharString first; CRegisteredTCInfo second; };
struct TCompare { char m_tag; bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; } };
typedef TEntry T;
typedef TCompare C;
static C MakePred() { C c; c.m_tag = 0; return c; }
static void Fill(T& t, int key, long i) { MakeString(t.first, key, i); t.second.AllocFunc = 0; t.second.DriverType = (unsigned long)(key + 40); t.second.Name = t.first; }
static int Key(const T& t) { return StringKey(t.first); }
static bool Intact(const T& t, const int*) { return t.second.DriverType == (unsigned long)(Key(t) + 40) && t.second.Name.PStringData == t.first.PStringData; }

static void PushHeap(T* first, long holeIndex, long topIndex, T value, C comp)
{
    long parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}
static void AdjustHeap(T* first, long holeIndex, long len, T value, C comp)
{
    long topIndex = holeIndex;
    long secondChild = 2 * holeIndex + 2;
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
    PushHeap(first, holeIndex, topIndex, value, comp);
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

    if (!ok) { std::printf("004dfdbb_TEST FAIL\n"); return 1; }
    std::printf("004dfdbb_TEST PASS\n");
    return 0;
}