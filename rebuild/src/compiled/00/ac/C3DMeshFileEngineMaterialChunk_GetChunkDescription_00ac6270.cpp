// Leaf: ?GetChunkDescription@C3DMeshFileEngineMaterialChunk@@UBE?BVCCharString@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/C3DMeshFileEngineMaterialChunk.h (class-model switch FABLE_RETAIL_C3DMESHFILEENGINEMATERIALCHUNK; was ?GetChunkDescription@C3DMeshFileEngineMaterialChunk_Methods@@QBE?AVCCharString@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// C3DMeshFileEngineMaterialChunk_Methods::GetChunkDescription
// retail 0x00ac6270 : const method returning CCharString by value.
// push -1 ; push <lit> ; ecx=retbuf ; call CCharString::ctor(char*, int) ; return retbuf

// Preamble: the define / include block of the landed precedent C3DMeshFileEngineMaterialChunk_ReadFromFile_00ac5770.cpp.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_C3DMESHFILEENGINEMATERIALCHUNK
#include "fable_string.h"
#include "engine/CDataInputStream.h"
#include "engine/C3DMeshFileEngineMaterialChunk.h"

const CCharString C3DMeshFileEngineMaterialChunk::GetChunkDescription() const {
    return CCharString("Engine Material", -1);
}