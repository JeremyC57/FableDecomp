// CBankFileEntryUpdateData::ReadInfo(unsigned long, void*, unsigned long) const -- copy `size` bytes of the entry's info buffer starting at `offset` into `dest`, when offset + size is a valid 31-bit position
// Leaf: ?ReadInfo@CBankFileEntryUpdateData@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CBE90, length 67 bytes (+ int3 padding).
// PDB (FableWin): ?ReadInfo@CBankFileEntryUpdateData@@QBEXKPAXK@Z (the only (ulong, void*, ulong) const member in the
// name sets). Layout (PDB): StateBlockCRC +0, InfoSize +4, InfoBuffer +8 (the landed SetInfo 0x009CC6F0 and the
// buffer release at 0x009CBEE0 use the same pair). The catalog label "BankData_CopyRange" is descriptive.
// Callers: CWADFile::GetFileInformation (0x00A39D57: ReadInfo(0, &info, 0x58)), CreateWorldLevelWad (0x00A3A57C),
// CEngineWorldMap::LoadMapThemeData (0x00B41FEB, 0x00B425E2).
// Shape: the end position is widened to 64 bits from the 32-bit unsigned sum (high word `xor eax,eax`, its flags
// feeding the first `jl`/`jg`), then the `p >= 0 && p <= 0x7fffffff` position test of the data streams
// (NGameText::CGroupEntry::ReadFromDataStream 0x009C8470 inlines the same predicate); the copy is the /Oi memcpy.
#include <string.h>

class CBankFileEntryUpdateData
{
public:
    unsigned long StateBlockCRC;             // +0x00
    unsigned long InfoSize;                  // +0x04
    char*         InfoBuffer;                // +0x08

    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    void ReadInfo(unsigned long offset, void* dest, unsigned long size) const;
};

void CBankFileEntryUpdateData::ReadInfo(unsigned long offset, void* dest, unsigned long size) const
{
    if (IsValidPosition((__int64)(offset + size)))
        memcpy(dest, InfoBuffer + offset, size);
}