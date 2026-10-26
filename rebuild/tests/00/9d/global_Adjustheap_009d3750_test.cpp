// behaviour test for 009d3750: _Adjust_heap over a bank's entry indices ordered by each entry's data offset (runtime records; the packed path is checked separately)
// The _Adjust_heap body below is the candidate's; make_heap/sort_heap are built on it and the result
// must come out ordered by the row's comparator with every element's payload intact.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static const long kKeys[9] = { 5, -3, 12, 7, 7, 0, 99, -40, 2 };

template<class T, class C> void AdjustHeap(T* first, long hole, long bottom, T value, C pred);

template <class U> class CArray { public:
    U* m_pFirst;
    U* m_pLast;
    U* m_pEnd;
    U& operator[](unsigned long index) const { return m_pFirst[index]; }
};
struct CRuntimeData {
    unsigned long DataOffset;
    unsigned long DataSize;
    unsigned char DataType;
    bool Valid;
};
struct CPackedUIntArray {
    unsigned long* PackedInts;
    unsigned long Size;
    unsigned char Bits;
    unsigned long Bias;
    unsigned long operator[](unsigned long index) const { return PackedInts[index]; }
    bool IsPacked() const { return Size != 0; }
};
struct CBankFile_Retail {
    CArray<CRuntimeData> RuntimeData;
    CPackedUIntArray PackedDataOffset;
    unsigned long GetEntryDataOffset(unsigned long index) const
    {
        if (PackedDataOffset.IsPacked())
            return PackedDataOffset[index - 1];
        return RuntimeData[index].DataOffset;
    }
};
struct CompareDataOffset {
    const CBankFile_Retail* m_pBank;
    bool operator()(unsigned long a, unsigned long b) const { return m_pBank->GetEntryDataOffset(a) < m_pBank->GetEntryDataOffset(b); }
};
typedef unsigned long T;
typedef CompareDataOffset Pred;
static CRuntimeData g_records[9];
static unsigned long g_packed[9];
static CBankFile_Retail g_bank;
static void InitPred(Pred& p)
{
    long i = 0;
    for (i = 0; i < 9; ++i) { g_records[i].DataOffset = (unsigned long)(kKeys[i] + 1000); g_packed[i] = (unsigned long)(5000 - kKeys[i]); }
    g_bank.RuntimeData.m_pFirst = g_records;
    g_bank.RuntimeData.m_pLast = g_records + 9;
    g_bank.RuntimeData.m_pEnd = g_records + 9;
    g_bank.PackedDataOffset.PackedInts = g_packed;
    g_bank.PackedDataOffset.Size = 0;
    p.m_pBank = &g_bank;
}
static void Make(T& e, long i) { e = (unsigned long)i; }
static long Sig(const T& e) { return (long)e; }
static bool Ok(const T& e) { return e < 9; }
static bool Extra(const T* a)
{
    T b[9];
    long i = 0;
    Pred pred;
    if (g_records[a[0]].DataOffset != 960 || g_records[a[8]].DataOffset != 1099)
        return false;
    pred.m_pBank = &g_bank;
    g_bank.PackedDataOffset.Size = 9;
    for (i = 0; i < 9; ++i)
        b[i] = (unsigned long)(i + 1);
    for (i = 4; 0 < i; ) {
        --i;
        AdjustHeap(b, i, 8, b[i], pred);
    }
    for (i = 1; i < 8; ++i) {
        if (g_packed[b[(i - 1) / 2] - 1] < g_packed[b[i] - 1])
            return false;
    }
    g_bank.PackedDataOffset.Size = 0;
    return true;
}

template<class T, class C> void PushHeap(T* first, long hole, long top, T value, C pred)
{
    long parent = (hole - 1) / 2;
    while (top < hole && pred(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}

template<class T, class C> void AdjustHeap(T* first, long hole, long bottom, T value, C pred)
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

static int RunTest()
{
    long n = 9;
    long i = 0;
    long last = 0;
    long sigBefore = 0;
    long sigAfter = 0;
    int ok = 1;
    T* a = new T[9];
    Pred pred;
    InitPred(pred);
    for (i = 0; i < n; ++i)
        Make(a[i], i);
    for (i = 0; i < n; ++i)
        sigBefore += Sig(a[i]);
    for (i = n / 2; 0 < i; ) {
        --i;
        T v(a[i]);
        AdjustHeap(a, i, n, v, pred);
    }
    for (last = n; 1 < last; --last) {
        T v(a[last - 1]);
        a[last - 1] = a[0];
        AdjustHeap(a, 0, last - 1, v, pred);
    }
    for (i = 1; i < n; ++i) {
        if (pred(a[i], a[i - 1]))
            ok = 0;
    }
    for (i = 0; i < n; ++i) {
        sigAfter += Sig(a[i]);
        if (!Ok(a[i]))
            ok = 0;
    }
    if (sigBefore != sigAfter)
        ok = 0;
    if (!Extra(a))
        ok = 0;
    delete[] a;
    return ok;
}

int main()
{
    if (!RunTest()) {
        std::printf("009d3750_TEST FAIL\n");
        return 1;
    }
    std::printf("009d3750_TEST PASS\n");
    return 0;
}