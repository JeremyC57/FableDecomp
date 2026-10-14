// C3DAnimFileXAllocationSizeChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver&) const -- version 1 then the allocation size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA4500, length 48 bytes.
// Callees: CDataOutputStream::WriteULONG @ 0x00993EA0 (x2).
// Modelled on the PDB engine header (AllocSize at +0x10 agrees with the bytes) through the usual _Methods
// derivation. The saver is unused; no super-chunk write (plain chunk base).
#include "engine/C3DAnimFileXAllocationSizeChunk.h"

struct CChunkedFileSaver;

struct CDataOutputStream {
    void WriteULONG(unsigned long value);                          // 0x00993EA0
};

struct C3DAnimFileXAllocationSizeChunk_Methods : C3DAnimFileXAllocationSizeChunk {
    void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;
};

void C3DAnimFileXAllocationSizeChunk_Methods::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& /*saver*/) const
{
    out.WriteULONG(1);
    out.WriteULONG(AllocSize);
}
