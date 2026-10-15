// Behaviour test for CBankFile::GetTotalDataSize (0x009CC450): sum of the sizes of live entries
// 1..Size-1, from the packed arrays when present, else from the runtime records.
#include <cstdio>

struct CRuntimeData { unsigned int DataOffset, DataSize; unsigned char DataType; bool Valid; };
struct CPackedUIntArray {
    const unsigned int* PackedInts; unsigned int Size;
    unsigned int operator[](unsigned int i) const { return PackedInts[i]; }
    bool IsPacked() const { return Size != 0; }
};
struct CBankFile {
    unsigned int Size; CRuntimeData* RuntimeData; CPackedUIntArray PackedDataSize, PackedValid;
    bool EntryExists(unsigned int index) const
    {
        if (index != 0 && index < Size) {
            if (PackedValid.IsPacked()) return PackedValid[index - 1] != 0;
            return RuntimeData[index].Valid;
        }
        return false;
    }
    unsigned int GetEntryDataSize(unsigned int index) const
    {
        if (PackedDataSize.IsPacked()) return PackedDataSize[index - 1];
        return RuntimeData[index].DataSize;
    }
    unsigned int GetTotalDataSize() const
    {
        unsigned int total = 0;
        for (unsigned int index = 0; index < Size; ++index)
            if (EntryExists(index)) total += GetEntryDataSize(index);
        return total;
    }
};

int main()
{
    CRuntimeData rd[4] = { { 0, 1000, 0, true }, { 0, 10, 0, true }, { 0, 20, 0, false }, { 0, 40, 0, true } };
    CBankFile b = { 4, rd, { 0, 0 }, { 0, 0 } };
    if (b.GetTotalDataSize() != 50) { std::printf("009cc450_TEST FAIL\n"); return 1; }
    static const unsigned int sizes[3] = { 1, 2, 4 }, valid[3] = { 0, 1, 1 };
    b.PackedDataSize.PackedInts = sizes; b.PackedDataSize.Size = 3; b.PackedValid.PackedInts = valid; b.PackedValid.Size = 3;
    if (b.GetTotalDataSize() != 6) { std::printf("009cc450_TEST FAIL\n"); return 1; }
    b.Size = 0;
    if (b.GetTotalDataSize() != 0) { std::printf("009cc450_TEST FAIL\n"); return 1; }
    std::printf("009cc450_TEST PASS\n");
    return 0;
}
