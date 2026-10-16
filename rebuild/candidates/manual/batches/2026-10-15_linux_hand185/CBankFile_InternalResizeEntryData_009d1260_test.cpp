// Behaviour test for 009d1260: CBankFile::InternalResizeEntryData -- solid append, in-place fit, relocation when it outgrows the block.
#include <cstdio>
struct TRec { unsigned long DataOffset, DataSize; };
struct TBank {
    TRec rec[4]; unsigned long allocated[4]; bool Solid; unsigned long Alignment; unsigned long fileLength; unsigned long freeOffset; int blockCount;
    unsigned long AlignOffset(unsigned long v) const { return (v + Alignment - 1) / Alignment * Alignment; }
    void InternalResizeEntryData(unsigned long index, unsigned long size)
    {
        TRec& r = rec[index];
        unsigned long aligned = AlignOffset(size);
        unsigned long alloc = allocated[index];
        if (alloc == 0 && Solid) { r.DataOffset = fileLength; r.DataSize = size; return; }
        Solid = false;
        if (aligned > alloc) { r.DataOffset = freeOffset; blockCount = 0; }
        r.DataSize = size;
    }
};
int main()
{
    TBank b; b.Alignment = 16; b.Solid = true; b.fileLength = 4096; b.freeOffset = 512; b.blockCount = 3;
    b.allocated[1] = 0; b.allocated[2] = 32; b.rec[2].DataOffset = 64;
    b.InternalResizeEntryData(1, 10);
    if (b.rec[1].DataOffset != 4096 || b.rec[1].DataSize != 10 || !b.Solid) { std::printf("009d1260_TEST FAIL\n"); return 1; }
    b.InternalResizeEntryData(2, 30);
    if (b.rec[2].DataOffset != 64 || b.rec[2].DataSize != 30 || b.Solid || b.blockCount != 3) { std::printf("009d1260_TEST FAIL\n"); return 1; }
    b.InternalResizeEntryData(2, 40);
    if (b.rec[2].DataOffset != 512 || b.rec[2].DataSize != 40 || b.blockCount != 0) { std::printf("009d1260_TEST FAIL\n"); return 1; }
    std::printf("009d1260_TEST PASS\n");
    return 0;
}
