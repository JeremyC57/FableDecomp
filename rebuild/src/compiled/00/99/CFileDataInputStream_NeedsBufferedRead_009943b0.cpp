
// CFileDataInputStream::NeedsBufferedRead @ 0x009943b0
// __fastcall: returns (requested < this->m_threshold at +0x20), signed-less.
// One stack arg, ret 4.
// Leaf: ?NeedsBufferedRead@CFileDataInputStream@@EAE_NJ@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CFileDataInputStream.h under FABLE_RETAIL_CFILEDATAINPUTSTREAM with the PDB parameter / return types (was ?NeedsBufferedRead@CFileDataInputStream_Methods@@QAE_NH@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#include "engine/CFileDataInputStream.h"
#include "rebuild_abi.h"
bool CFileDataInputStream::NeedsBufferedRead(long requested) {
    return requested < BufferSize;              // +0x20
}
