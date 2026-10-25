#include <cstdio>
#include <cstdlib>
#include <new>

// Host model of CVectorMap<CCharString, CSymbolInfo, CCompareString>::DeallocateMemory: clear the map's vector, then swap
// it (and the Dirty flag) with an empty temporary whose destructor frees the old block.
static int g_destroyed = 0;
static int g_frees = 0;

struct CEntry {
    int Key;
    long Val;
    CEntry(int k, long v) : Key(k), Val(v) { }
    CEntry(const CEntry& o) : Key(o.Key), Val(o.Val) { }
    ~CEntry() { ++g_destroyed; }
};

template <class T> void swap_(T& a, T& b) { T t = a; a = b; b = t; }

struct CEntryVector {
    CEntry* Start; CEntry* Finish; CEntry* End;
    CEntryVector() : Start(0), Finish(0), End(0) { }
    ~CEntryVector()
    {
        for (CEntry* p = Start; p != Finish; ++p) p->~CEntry();
        if (Start) { ++g_frees; std::free(Start); }
    }
    void reserve_(unsigned int n) { Start = Finish = (CEntry*)std::malloc(n * sizeof(CEntry)); End = Start + n; }
    void push_back(const CEntry& e) { new (Finish) CEntry(e); ++Finish; }
    void clear()
    {
        for (CEntry* p = Start; p != Finish; ++p) p->~CEntry();
        Finish = Start;
    }
    void swap(CEntryVector& x) { swap_(Start, x.Start); swap_(Finish, x.Finish); swap_(End, x.End); }
};

struct CVectorMap {
    CEntryVector Map;
    char Compare;
    bool Dirty;
    CVectorMap() : Dirty(false) { }
    void swap(CVectorMap& other) { Map.swap(other.Map); swap_(Dirty, other.Dirty); }
    void DeallocateMemory()
    {
        Map.clear();
        CVectorMap empty;
        swap(empty);
    }
};

int main()
{
    {
        CVectorMap m;
        m.Map.reserve_(8);
        m.Map.push_back(CEntry(1, 10));
        m.Map.push_back(CEntry(2, 20));
        m.Map.push_back(CEntry(3, 30));
        m.Dirty = true;
        g_destroyed = 0;
        m.DeallocateMemory();
        if (m.Map.Start != 0 || m.Map.Finish != 0 || m.Map.End != 0) { std::printf("009e79e0_TEST FAIL\n"); return 1; }
        if (m.Dirty) { std::printf("009e79e0_TEST FAIL\n"); return 1; }          // the empty map's flag swapped in
        if (g_destroyed != 3 || g_frees != 1) { std::printf("009e79e0_TEST FAIL\n"); return 1; }   // elements gone, block freed
        m.DeallocateMemory();                                                    // empty: nothing more to free
        if (g_frees != 1) { std::printf("009e79e0_TEST FAIL\n"); return 1; }
    }
    if (g_frees != 1) { std::printf("009e79e0_TEST FAIL\n"); return 1; }
    std::printf("009e79e0_TEST PASS\n");
    return 0;
}