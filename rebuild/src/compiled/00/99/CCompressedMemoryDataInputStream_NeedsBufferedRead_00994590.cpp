
// CCompressedMemoryDataInputStream::NeedsBufferedRead @ 0x00994590
// __fastcall: returns (requested < this->m_threshold at +0x1c), signed-less.
// One stack arg, ret 4.
// Leaf: ?NeedsBufferedRead@CCompressedMemoryDataInputStream@@EAE_NJ@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CCompressedMemoryDataInputStream.h under FABLE_RETAIL_CCOMPRESSEDMEMORYDATAINPUTSTREAM with the PDB parameter / return types (was ?NeedsBufferedRead@CCompressedMemoryDataInputStream_Methods@@QAE_NH@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CCOMPRESSEDMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/CCompressedMemoryDataInputStream.h"
#include "rebuild_abi.h"
bool CCompressedMemoryDataInputStream::NeedsBufferedRead(long requested) {
    return requested < BufferSize;              // +0x1C
}
