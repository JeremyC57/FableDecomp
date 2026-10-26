#include <cstdio>
// Behaviour test for 0x009D3040: _Push_heap over bank entry indices ordered by data offset, packed
// (1-based) or from the runtime records.
struct CRuntimeData { unsigned long DataOffset; unsigned long DataSize; unsigned char DataType; bool Valid; };
struct CPackedUIntArray {
    unsigned long* PackedInts; unsigned long Size; unsigned char Bits; unsigned long Bias;
    unsigned long operator[](unsigned long index) { return PackedInts[index]; }
};
struct CRuntimeDataArray { CRuntimeData* First; CRuntimeData* Last; CRuntimeData* End; };
class CBankFile { public: CRuntimeDataArray RuntimeData; CPackedUIntArray PackedDataOffset; };
inline unsigned long GetEntryDataOffset(CBankFile* bank, unsigned long index)
{
    if (bank->PackedDataOffset.Size != 0)
        return bank->PackedDataOffset[index - 1];
    return bank->RuntimeData.First[index].DataOffset;
}
struct CompareDataOffset {
    CBankFile* BankFile;
    bool operator()(unsigned long a, unsigned long b) const { return GetEntryDataOffset(BankFile, a) < GetEntryDataOffset(BankFile, b); }
};
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
static int check(CBankFile* bank, unsigned long* h, int n)
{
    for (int c = 1; c < n; ++c)
        if (GetEntryDataOffset(bank, h[(c - 1) / 2]) < GetEntryDataOffset(bank, h[c])) return 0;
    return 1;
}
int main()
{
    CRuntimeData rd[8];
    unsigned long offs[8] = { 500, 20, 0xF0000000u, 75, 300, 10, 999, 1 };
    for (int i = 0; i < 8; ++i) { rd[i].DataOffset = offs[i]; rd[i].DataSize = 0; rd[i].DataType = 0; rd[i].Valid = true; }
    unsigned long packed[8] = { 7, 70, 700, 3, 30, 300, 9, 90 };   // entry i (1-based) -> packed[i-1]
    CBankFile bank;
    bank.RuntimeData.First = rd; bank.RuntimeData.Last = rd + 8; bank.RuntimeData.End = rd + 8;
    bank.PackedDataOffset.PackedInts = packed; bank.PackedDataOffset.Size = 0; bank.PackedDataOffset.Bits = 0; bank.PackedDataOffset.Bias = 0;
    CompareDataOffset pred; pred.BankFile = &bank;
    unsigned long h[8]; int n = 0;
    for (unsigned long e = 0; e < 8; ++e) { _Push_heap(h, n, 0, e, pred); ++n; if (!check(&bank, h, n)) { std::printf("009d3040_TEST FAIL\n"); return 1; } }
    if (h[0] != 2) { std::printf("009d3040_TEST FAIL\n"); return 1; }
    bank.PackedDataOffset.Size = 8;                                  // packed mode, entries 1..8
    n = 0;
    for (unsigned long e = 1; e <= 8; ++e) { _Push_heap(h, n, 0, e, pred); ++n; if (!check(&bank, h, n)) { std::printf("009d3040_TEST FAIL\n"); return 1; } }
    if (h[0] != 3) { std::printf("009d3040_TEST FAIL\n"); return 1; }
    std::printf("009d3040_TEST PASS\n");
    return 0;
}