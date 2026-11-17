// CPersistTraits<CFloatRange>::TransferBinaryIn(CMemoryDataInputStream&, CFloatRange&) -- read LowValue then HighValue
// Retail: Fable.exe (TLC, VC7.1) @ 0x00458F89, 185 bytes, `ret 8` (thiscall, the empty traits `this` is unused).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (x2, the slow path of the inlined Read).
// Layout: engine/CDataInputStream.h; PDB CFloatRange (8 bytes, engine/CFloatRange.h): HighValue +0, LowValue +4.
// Shape: two inlined CDataInputStream::Read calls into one 4-byte local homed in the dead stream-argument slot.
//   Size-optimised TU: #pragma optimize("s", on) (ebp frame).
// Name fix (agentNF54): the row is ?TransferBinaryIn@?$CPersistTraits@VCFloatRange@@@@QAEXAAVCMemoryDataInputStream@@AAVCFloatRange@@@Z
//   (Ego_r 0x00467F5B + FableWin 0x019C33B6: class-keyed traits, the stream and value by reference), declared by the shared
//   engine/CPersistTraits.h; ported onto it, engine/CMemoryDataInputStream.h (the inline bounds-checked CDataInputStream::Read /
//   ReadFloat, so the local CDataInputStreamReader friend helper is gone) and engine/CFloatRange.h. Was the pointer-parameter
//   ?TransferBinaryIn@?$CPersistTraits@VCFloatRange@@@@QAEXPAVCDataInputStream@@PAVCFloatRange@@@Z; its only caller 0x00458F2B follows.
// Leaf: ?TransferBinaryIn@?$CPersistTraits@VCFloatRange@@@@QAEXAAVCMemoryDataInputStream@@AAVCFloatRange@@@Z
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FABLE_RETAIL_CPERSISTTRAITS_CREATURE_FULL_TYPES_PASS22
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"

void CPersistTraits<CFloatRange>::TransferBinaryIn(CMemoryDataInputStream& is, CFloatRange& value)
{
    value.LowValue = is.ReadFloat();
    value.HighValue = is.ReadFloat();
}
