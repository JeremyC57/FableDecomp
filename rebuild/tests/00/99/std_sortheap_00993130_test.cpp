// Behaviour test for 0x00993130 std::sort_heap over (long key, sound-bank record) pairs: after make_heap, sort_heap
// leaves the range ascending by key, each record still attached to its key, and every counted copy released.
#include <cstdio>

static int g_live = 0;      // live string copies beyond the originals
static int g_refs = 0;      // outstanding counted-pointer references taken by copies

class CCharString { public:
    int v;
    CCharString(int x = 0) : v(x) { }
    CCharString(const CCharString& o) : v(o.v) { ++g_live; }
    CCharString& operator=(const CCharString& o) { v = o.v; return *this; }
    ~CCharString() { --g_live; }
};
struct CInfo { long RefCount; };
struct CCountedPointer {
    int* Data; CInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) { ++Info->RefCount; ++g_refs; } }
    CCountedPointer& operator=(const CCountedPointer& o) {
        if (Info != o.Info) { if (Info) { --Info->RefCount; --g_refs; } Data = o.Data; Info = o.Info; if (Info) { ++Info->RefCount; ++g_refs; } }
        else Data = o.Data;
        return *this;
    }
    ~CCountedPointer() { if (Info) { --Info->RefCount; --g_refs; } }
};
struct CSoundBankRecord {
    CCountedPointer Bank, BankData;
    CCharString Name0, Name1, Name2, Name3;
};
class CSoundBankEntry { public: long first; CSoundBankRecord second; };
struct CSoundBankEntryLess { bool operator()(const CSoundBankEntry& a, const CSoundBankEntry& b) const { return a.first < b.first; } };

// the model: STLport 4.6 heap algorithms
template <class T, class C> void push_heap_(T* first, long hole, long top, T value, C comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = value;
}
template <class T, class C> void adjust_heap_(T* first, long hole, long len, T value, C comp)
{
    long top = hole, child = 2 * hole + 2;
    while (child < len) { if (comp(first[child], first[child - 1])) child--; first[hole] = first[child]; hole = child; child = 2 * (child + 1); }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    push_heap_(first, hole, top, value, comp);
}
template <class T, class C> void make_heap_(T* first, T* last, C comp)
{
    long len = last - first; if (len < 2) return;
    for (long parent = (len - 2) / 2; ; --parent) { adjust_heap_(first, parent, len, T(first[parent]), comp); if (parent == 0) return; }
}
// the function under test (shape of 0x00993130)
template <class T, class C> void sort_heap(T* first, T* last, C comp)
{
    while (last - first > 1) {
        T* l = last--;
        T val(*(l - 1));
        *(l - 1) = *first;
        adjust_heap_(first, 0L, (long)((l - 1) - first), val, comp);
    }
}

int main()
{
    static const long keys[] = { 40, -7, 13, 99, 0, 13, 5, -100, 61, 2, 77 };
    const int n = sizeof(keys) / sizeof(keys[0]);
    CInfo infos[n];
    int payload[n];
    CSoundBankEntry e[n];
    int ok = 1;
    for (int i = 0; i < n; ++i) {
        infos[i].RefCount = 1; payload[i] = (int)keys[i] * 3;
        e[i].first = keys[i];
        e[i].second.Bank.Data = &payload[i]; e[i].second.Bank.Info = &infos[i];
        e[i].second.Name0.v = (int)keys[i] + 1000; e[i].second.Name3.v = (int)keys[i] - 1000;
    }
    int live0 = g_live;
    CSoundBankEntryLess less;
    make_heap_(e, e + n, less);
    sort_heap(e, e + n, less);
    for (int i = 0; i < n; ++i) {
        if (i && e[i - 1].first > e[i].first) ok = 0;
        if (e[i].second.Name0.v != (int)e[i].first + 1000 || e[i].second.Name3.v != (int)e[i].first - 1000) ok = 0;
        if (*e[i].second.Bank.Data != (int)e[i].first * 3) ok = 0;
        if (e[i].second.Bank.Info->RefCount != 1) ok = 0;
    }
    if (e[0].first != -100 || e[n - 1].first != 99) ok = 0;
    if (g_live != live0 || g_refs != 0) ok = 0;
    // one- and zero-element ranges are left alone
    CSoundBankEntry one[1]; one[0].first = 5;
    sort_heap(one, one + 1, less); sort_heap(one, one, less);
    if (one[0].first != 5) ok = 0;
    for (int i = 0; i < n; ++i) { e[i].second.Bank.Info = 0; }
    std::printf(ok ? "00993130_TEST PASS\n" : "00993130_TEST FAIL\n");
    return ok ? 0 : 1;
}