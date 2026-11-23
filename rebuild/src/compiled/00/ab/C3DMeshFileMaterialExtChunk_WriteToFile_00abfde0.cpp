// Leaf: ?WriteToFile@C3DMeshFileMaterialExtChunk@@UBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z
// Switch route (NF318, 2026-11-23): defined on the real class of engine/C3DMeshFileMaterialExtChunk.h (class-model switch FABLE_RETAIL_C3DMESHFILEMATERIALEXTCHUNK; was ?WriteToFile@C3DMeshFileMaterialExtChunk_Methods@@QBEXAAVCDataOutputStream@@AAVCChunkedFileSaver@@@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// C3DMeshFileMaterialExtChunk::WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const -- version 1 then SelfIllumination; the saver is unused and there is no base call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABFDE0, length 33 bytes.
// Callees: 0x00993EA0 CDataOutputStream::WriteULONG x2
//
// Preamble: the define / include block of the landed precedent C3DMeshFileMaterialExtChunk_GetSelfIllumination_00abfdc0.cpp.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILEMATERIALEXTCHUNK
#define FABLE_RETAIL_C3DMESHFILEMATERIALEXTCHUNK_METHODS
#include "fable_string.h"
#include "engine/C3DMeshFileMaterialExtChunk.h"
class CChunkedFileSaver;
class CDataOutputStream { public:
public:
    void WriteULONG(unsigned long);   // 0x00993EA0
};
void C3DMeshFileMaterialExtChunk::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
{
    out.WriteULONG(1);
    out.WriteULONG(SelfIllumination);
}