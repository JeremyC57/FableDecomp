#include <cstdio>
// Behaviour test: CompareDataOffset orders entry indices by data offset, packed (1-based) when the packed array is non-empty.
struct CRuntimeData { unsigned int DataOffset, DataSize; unsigned char DataType; bool Valid; };
struct CPackedUIntArray {
    unsigned int* PackedInts; unsigned int Size; unsigned char Bits; unsigned int Bias;
    unsigned int operator[](unsigned int index) { return PackedInts[index] + Bias; }
};
struct CBankFile { CRuntimeData* RuntimeData; CPackedUIntArray PackedDataOffset; };
inline unsigned int GetEntryDataOffset(CBankFile* bank, unsigned int index)
{
    if (bank->PackedDataOffset.Size != 0)
        return bank->PackedDataOffset[index - 1];
    return bank->RuntimeData[index].DataOffset;
}
struct CompareDataOffset {
    CBankFile* BankFile;
    bool operator()(unsigned int a, unsigned int b) { return GetEntryDataOffset(BankFile, a) < GetEntryDataOffset(BankFile, b); }
};
int main()
{
    CRuntimeData rd[3]; rd[0].DataOffset = 50; rd[1].DataOffset = 10; rd[2].DataOffset = 30;
    unsigned int packed[3] = { 5, 1, 3 };
    CBankFile bank; bank.RuntimeData = rd; bank.PackedDataOffset.PackedInts = packed; bank.PackedDataOffset.Size = 0;
    bank.PackedDataOffset.Bias = 0;
    CompareDataOffset cmp; cmp.BankFile = &bank;
    if (!cmp(1, 2) || cmp(2, 1) || !cmp(2, 0) || cmp(0, 0)) { std::printf("009cc640_TEST FAIL\n"); return 1; }
    bank.PackedDataOffset.Size = 3;                 // now index i reads packed[i - 1]: 1->5, 2->1, 3->3
    if (cmp(1, 2) || !cmp(2, 3) || !cmp(3, 1)) { std::printf("009cc640_TEST FAIL\n"); return 1; }
    std::printf("009cc640_TEST PASS\n");
    return 0;
}