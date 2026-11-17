// CPersistContext::Transfer<CWideString>(const char* name, CWideString& value) -- transfer a string with an empty default
// Leaf: ??$Transfer@VCWideString@@@CPersistContext@@QAEXPBDAAVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00411060, length 76 bytes (+ int3 padding).
// Callees: CWideString::CWideString() @ 0x0099AED0, CWideString::CWideString(const CWideString&) @ 0x0099B720, CWideString::~CWideString() @ 0x0099B510 (twice),
// CPersistContext::Transfer<CWideString>(const char*, CWideString&, const CWideString&) @ 0x00410AC0 (labelled
// "Transfer<CIntelligentPointer<CThing>>", propagated). The CCharString twin @ 0x00411010 has the same shape.
// Shape: the default is copy-initialised from a default-constructed temporary through an inline helper that returns a NAMED
// local (`T value; return value;`): VC7.1 has no NRVO, so the helper's local is built in the low frame slot, copied
// into the caller's named default (the return slot) and destroyed at once,
// then pushes &default, value, name (right to left) with `this` restored into ecx, and destroys the default; `ret 8`.
// Name fix (agentNF54): the local CWideString / CPersistContext stand-ins replaced by the shared fable_string.h and
//   engine/CPersistContext.h. The row is the instance of the inline member template Transfer<T>(const char*, T&) =
//   `Transfer(name, value, GetDefaultVal<T>())` (FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS, GetDefaultVal's traits-local body
//   FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY): Ego_r 0x0040D300 + FableWin 0x018956C0
//   ??$Transfer@VCWideString@@@CPersistContext@@QAEXPBDAAVCWideString@@@Z, the spelling the landed caller 0x0047F58B already binds
//   (was the non-template ?Transfer@CPersistContext@@QAEXPBDAAVCWideString@@@Z); its callee 0x00410AC0 is
//   ??$Transfer@VCWideString@@@CPersistContext@@QAEXPBDAAVCWideString@@ABV1@@Z (Ego_r 0x0040D1C0 + FableWin 0x01895750).
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "fable_string.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistTraits.h"

template void CPersistContext::Transfer<CWideString>(const char* name, CWideString& value);
