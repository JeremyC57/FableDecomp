// CWADFile::Read(void* buffer, unsigned long size, unsigned long flags) -- read `size` bytes of this WAD entry from the bank at the current position, then advance the position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A39BB0, length 43 bytes (+ int3 padding). CWADFile vtable 0x0129CF8C slot 3.
// Leaf: Read
// Callee: CBankFile::ReadEntryData(unsigned long index, unsigned long offset, void* buffer, unsigned long size) @ 0x009CC2A0.
// Shape: `ret 0Ch` with the third stack argument never read; the size (second argument) is loaded into edi and reused for
// the `FilePos += size` after the call (FilePos re-read from memory). Members from the engine header (Bank +4, Index +8, FilePos +0xc).
#include "engine/CWADFile.h"

struct CBankFileAsync {
    void ReadEntryData(unsigned long index, unsigned long offset, void* buffer, unsigned long size);   // 0x009CC2A0
};

struct CWADFile_Methods : CWADFile {
    void Read(void* buffer, unsigned long size, unsigned long flags);
};

void CWADFile_Methods::Read(void* buffer, unsigned long size, unsigned long)
{
    Bank->ReadEntryData(Index, FilePos, buffer, size);
    FilePos += size;
}