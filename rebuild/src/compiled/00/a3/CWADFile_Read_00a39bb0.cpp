// CWADFile::Read(void* buffer, unsigned long size, unsigned long flags) -- read `size` bytes of this WAD entry from the bank at the current position, then advance the position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A39BB0, length 43 bytes (+ int3 padding). CWADFile vtable 0x0129CF8C slot 3.
// Callee: CBankFile::ReadEntryData(unsigned long index, unsigned long offset, void* buffer, unsigned long size) @ 0x009CC2A0.
// Shape: `ret 0Ch` with the third stack argument never read; the size (second argument) is loaded into edi and reused for
// the `FilePos += size` after the call (FilePos re-read from memory). Members from the engine header (Bank +4, Index +8, FilePos +0xc).

// Leaf: ?Read@CWADFile@@UAEXPAXJPAVCASuspendableProcess@@@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CWADFile.h under FABLE_RETAIL_CWADFILE with the PDB parameter / return types (was ?Read@CWADFile_Methods@@QAEXPAXKK@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CWADFILE
#include "engine/CWADFile.h"
class CBankFile
{
public:
    void ReadEntryData(unsigned long, unsigned long, void*, unsigned long);   // 0x009CC2A0
};

class CBankFileAsync : public CBankFile { public:
};
void CWADFile::Read(void* buffer, long size, CASuspendableProcess*) {
    Bank->ReadEntryData(Index, FilePos, buffer, size);
    FilePos += size;
}
