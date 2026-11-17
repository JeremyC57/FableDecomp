// CPersistContext::Transfer<CFloatRange>(CFloatRange&) -- one-argument transfer helper (member template; see the landed 0x00431061 Transfer<float>):
//   CheckCRC("") (0x00404500, the empty key 0x0122D70E), then the binary load (mode 2) / save (mode 3) switch.
//   namepass: one name for 0x00458F2B -- its callers call CPersistContext::Transfer<T>; the catalog label
//   "CEngineLightingManager::UpdateShadowScene" / Transfer*_xxx was propagated or descriptive.
// Leaf: ??$Transfer@VCFloatRange@@@CPersistContext@@QAEXAAVCFloatRange@@@Z
#pragma optimize("s", on)
// CPersistContext::TransferFloatRange_00458f2b(CFloatRange* value) -- CheckCRC(""), then in mode 3 (save) write the range's two floats (max first, then min) to the save stream, in mode 2 (load) read them through a zeroed two-float reader local
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00458F2B, length 94 bytes.
// Callees: CPersistContext::CheckCRC @ 0x00404500 (the empty key @ 0x0122D70E), CDataOutputStream::WriteFloat @ 0x00993EE0 twice (the save stream +0x28 kept in esi;
// each float pushed through the x87), the reader's TransferBinaryIn @ 0x00458F89 (`this` = the local, the load stream +0x24 and the value on the stack).
// Shape: as CPersistContext::TransferBytePair_004e8fcd @ 0x004E8FCD (landed): the reader local's two zero floats are written (`fldz ; fstp`) before the mode
// ladder (`dec ; dec ; je ; dec ; jne`: cases 2 and 3); edi is saved only around the save arm. The chunk label "CPersistContext::UpdateShadowScene" is
// propagated from an unrelated body (the same label sits on 0x004E8FCD).
// Name fix (agentNF54): the local CFloatRange / CDataOutputStream / CPersistContext / two-float reader stand-ins replaced by the shared
//   engine/CPersistContext.h (the one-argument Transfer<T> body, FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY), engine/CPersistTraits.h
//   (CPersistTraits<CFloatRange>: DefaultVal zero-initialised, the inline TransferBinaryOut writing LowValue then HighValue) and
//   engine/CFloatRange.h: the "reader" local is the CPersistTraits<CFloatRange> traits object, and its load call 0x00458F89 is
//   ?TransferBinaryIn@?$CPersistTraits@VCFloatRange@@@@QAEXAAVCMemoryDataInputStream@@AAVCFloatRange@@@Z (Ego_r 0x00467F5B + FableWin
//   0x019C33B6; was ?TransferBinaryIn@CFloatRangeReader_00458f89@@QAEXPAVCDataInputStream@@PAVCFloatRange@@@Z).
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BINARY_BODY
#define FABLE_RETAIL_CPERSISTTRAITS_CREATURE_FULL_TYPES_PASS22
#define FABLE_RETAIL_CFLOATRANGE_ORIGINAL_CTOR_PASS22
#define FABLE_RETAIL_CPERSISTTRAITS_CFLOATRANGE_BINARYOUT_FORCEINLINE
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"

template void CPersistContext::Transfer<CFloatRange>(CFloatRange& value);
