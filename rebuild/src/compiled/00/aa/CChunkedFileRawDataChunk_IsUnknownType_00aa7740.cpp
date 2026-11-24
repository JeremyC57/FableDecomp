// Leaf: ?IsUnknownType@CChunkedFileRawDataChunk@@UBEHXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CChunkedFileRawDataChunk.h (class-model switch FABLE_RETAIL_CCHUNKEDFILERAWDATACHUNK; was ?IsUnknownType@CChunkedFileRawDataChunk_Methods@@QAEKXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILERAWDATACHUNK
#include "engine/CChunkedFileRawDataChunk.h"  // retyped onto the PDB layout; byte parity re-verified
#include "rebuild_abi.h"
// CChunkedFileRawDataChunk::IsUnknownType @ 0x00aa7740
// mov eax,1; ret  -> always true
int CChunkedFileRawDataChunk::IsUnknownType() const{ return 1; }