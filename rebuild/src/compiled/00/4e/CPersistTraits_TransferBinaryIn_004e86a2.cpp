// CPersistTraits<EVillageTaskType>::TransferBinaryIn(CMemoryDataInputStream&, EVillageTaskType&) -- binary persist read of one 4-byte enum: inlined CDataInputStream::Read, ReadWithSrcChunkOverflow @ 0x00993CA0 when the chunk runs out
// Leaf: ?TransferBinaryIn@?$CPersistTraits@W4EVillageTaskType@@@@QAEXAAVCMemoryDataInputStream@@AAW4EVillageTaskType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E86A2, oracle length 99 bytes, `ret 8` (traits `this` unused).
// Identity: the landed caller 0x004E860E calls this address (rebuild/oracles/reloc_bindings.tsv) with this T, and
//   the PDBs name the caller template for this T, ??$GFSerialiseVectorBinaryIn@W4EVillageTaskType@@@@ (Ego_r.pdb), and the same
//   CPersistTraits<enum>::TransferBinaryIn(CMemoryDataInputStream&, E&) member for other enums (the T's own instance is not in the PDB).
// Body: the generic template body (engine/CPersistTraits.h FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY): a local filled by the
//   inline CDataInputStream::Read and copied to the output.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_BINARYIN_BODY
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
enum EVillageTaskType { };   // the Ego_r enumeration (only its 4-byte size matters here)

template void CPersistTraits<EVillageTaskType>::TransferBinaryIn(CMemoryDataInputStream&, EVillageTaskType&);
