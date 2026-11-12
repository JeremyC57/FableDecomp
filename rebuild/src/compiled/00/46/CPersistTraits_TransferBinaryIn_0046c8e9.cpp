// CPersistTraits<ECombatSequenceIsValidType>::TransferBinaryIn(CMemoryDataInputStream&, ECombatSequenceIsValidType&) -- binary persist read of one 4-byte enum: inlined CDataInputStream::Read, ReadWithSrcChunkOverflow @ 0x00993CA0 when the chunk runs out
// Leaf: ?TransferBinaryIn@?$CPersistTraits@W4ECombatSequenceIsValidType@@@@QAEXAAVCMemoryDataInputStream@@AAW4ECombatSequenceIsValidType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046C8E9, oracle length 99 bytes, `ret 8` (traits `this` unused).
// Identity: the landed caller 0x0046C8A8 calls this address as this symbol (rebuild/oracles/reloc_bindings.tsv), and
//   Ego_r.pdb / FableWin.pdb name this exact symbol.
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
enum ECombatSequenceIsValidType { };   // the Ego_r enumeration (only its 4-byte size matters here)

template void CPersistTraits<ECombatSequenceIsValidType>::TransferBinaryIn(CMemoryDataInputStream&, ECombatSequenceIsValidType&);
