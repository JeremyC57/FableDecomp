// CEnvironmentDef::CEnvironmentDef() -- the def constructor: the out-of-line CParentDefClassBase() 0x00430370, the CEnvironmentDef vtable, then CCharString() on ColourLookupTexture
// Leaf: ??0CEnvironmentDef@@QAE@XZ
// Retail: Fable.exe @ 0x004558BF. Identity: the vtable it stores, 0x012335A4, is RTTI .?AVCEnvironmentDef@@ : CParentDefClassBase
//   (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb ??0CEnvironmentDef@@QAE@XZ; engine/CEnvironmentDef.h places it here.
// Callees: CParentDefClassBase::CParentDefClassBase 0x00430370 (Ego_r.pdb / FableWin.pdb ??0CParentDefClassBase@@QAE@XZ),
//   CCharString::CCharString 0x0099E4B0 (??0CCharString@@QAE@XZ, bound by landed callers).
// Model: the shared engine/CEnvironmentDef.h (hdr overlay opt-in FABLE_RETAIL_CENVIRONMENTDEF_PARENT, the retail def class) over
//   engine/CParentDefClassBase.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CENVIRONMENTDEF_PARENT
#include "fable_string.h"
#include "engine/CEnvironmentDef.h"

CEnvironmentDef::CEnvironmentDef()
{
}
