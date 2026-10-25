// Behaviour test for 0x009D3B90 std::__insertion_sort over bank entry indices with CBankFile::CompareDataOffset (packed or runtime data offsets; trivial copy_backward through memmove).
#include <cstdio>
#include <cstring>

template <class T> struct CArray {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};
struct CRuntimeData { unsigned long DataOffset; unsigned long DataSize; unsigned char DataType; bool Valid; };
struct CPackedUIntArray {
    unsigned long* PackedInts; unsigned long Size; unsigned char Bits; unsigned long Bias;
    unsigned long operator[](unsigned long index) const { return PackedInts[index] + Bias; }   // test stand-in for 0x00A629F0
    bool IsPacked() const { return Size != 0; }
};
struct CBankFile_Retail {
    CArray<CRuntimeData> RuntimeData;
    CPackedUIntArray     PackedDataOffset;
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

static int g_memmoves = 0;
static void __unguarded_linear_insert(unsigned long* last, unsigned long value, CompareDataOffset comp)
{
    unsigned long* next = last;
    --next;
    while (comp(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}
static void* __copy_trivial_backward(const void* first, const void* last, void* result)
{
    const int num = (const char*)last - (const char*)first;
    if (num > 0) { ++g_memmoves; return std::memmove((char*)result - num, first, num); }
    return result;
}
static void __linear_insert(unsigned long* first, unsigned long* last, unsigned long value, CompareDataOffset comp)
{
    if (comp(value, *first)) {
        __copy_trivial_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value, comp);
}
static void __insertion_sort(unsigned long* first, unsigned long* last, CompareDataOffset comp)
{
    if (first == last)
        return;
    for (unsigned long* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    // runtime-data path: entry index -> RuntimeData[index].DataOffset
    CRuntimeData rd[6];
    unsigned long offs[6] = { 0, 400, 100, 300, 50, 200 };
    for (int k = 0; k < 6; ++k) { rd[k].DataOffset = offs[k]; rd[k].DataSize = 1; rd[k].DataType = 0; rd[k].Valid = true; }
    CBankFile_Retail bank;
    bank.RuntimeData.m_pFirst = rd; bank.RuntimeData.m_pLast = rd + 6; bank.RuntimeData.m_pEnd = rd + 6;
    bank.PackedDataOffset.PackedInts = 0; bank.PackedDataOffset.Size = 0; bank.PackedDataOffset.Bits = 0; bank.PackedDataOffset.Bias = 0;
    CompareDataOffset comp = { &bank };
    unsigned long idx[5] = { 1, 2, 3, 4, 5 };
    __insertion_sort(idx, idx + 5, comp);
    unsigned long want[5] = { 4, 2, 5, 3, 1 };   // 50 100 200 300 400
    for (int j = 0; j < 5; ++j)
        if (idx[j] != want[j]) { std::printf("009d3b90_TEST FAIL\n"); return 1; }
    // 100 and 50 go to the front through the trivial copy_backward (memmove); 300 and 200 take the unguarded path
    if (g_memmoves != 2) { std::printf("009d3b90_TEST FAIL\n"); return 1; }
    // packed path: entry index -> PackedDataOffset[index - 1] (+ bias)
    unsigned long packed[4] = { 30, 10, 40, 20 };
    bank.PackedDataOffset.PackedInts = packed; bank.PackedDataOffset.Size = 4; bank.PackedDataOffset.Bias = 7;
    unsigned long idx2[4] = { 1, 2, 3, 4 };
    __insertion_sort(idx2, idx2 + 4, comp);
    unsigned long want2[4] = { 2, 4, 1, 3 };
    for (int j = 0; j < 4; ++j)
        if (idx2[j] != want2[j]) { std::printf("009d3b90_TEST FAIL\n"); return 1; }
    std::printf("009d3b90_TEST PASS\n");
    return 0;
}