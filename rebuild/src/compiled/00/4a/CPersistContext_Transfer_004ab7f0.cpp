// CPersistContext::Transfer<CWorld::ESaveType>(const char* name, CWorld::ESaveType& value) -- the two-argument overload: forward to the three-argument Transfer with a value-initialised (zero) default
// Leaf: ??$Transfer@W4ESaveType@CWorld@@@CPersistContext@@QAEXPBDAAW4ESaveType@CWorld@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AB7F0, 32 bytes, `ret 8`. Unreferenced out-of-line instance.
// Callees: CPersistContext::Transfer<ESaveType>(const char*, ESaveType&, const ESaveType&) 0x004AB640 (catalogued as the
//   propagated "Transfer<signed_char>"; its callers in CWorld's save-state code (0x0049F79C, 0x0049F7BA) pass
//   CWorld::MostRecentSaveType / MostRecentSaveTypeBeforeManualSave, PDB type CWorld::ESaveType, so T = CWorld::ESaveType).
// Shape: the `T()` default is a 4-byte frame temporary (`push ecx`), its address pushed first and the zero stored after the
//   three pushes; `this` stays in ecx; `pop ecx ; ret 8`. The sibling double instance is the landed 0x004AB810.
// Name fix (agentNF54): ESaveType is nested in CWorld (FableWin.pdb 0x01B5E5F0
//   ??$Transfer@W4ESaveType@CWorld@@@CPersistContext@@QAEXPBDAAW4ESaveType@CWorld@@@Z, this two-argument instance; Ego_r / FableWin
//   spell the callee 0x004AB640 ??$Transfer@W4ESaveType@CWorld@@@...ABW412@@Z, the landed row's own leaf). The local enum and
//   CPersistContext stand-ins are replaced by engine/CWorld.h (opt-in FABLE_RETAIL_CWORLD_NESTED_ESAVETYPE) and engine/CPersistContext.h,
//   whose inline Transfer<T>(name, value) is `Transfer(name, value, GetDefaultVal<T>())` (FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS +
//   GETDEFAULTVAL_BODY; the traits default is the zero). Was ??$Transfer@W4ESaveType@@@CPersistContext@@QAEXPBDAAW4ESaveType@@@Z.
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_NESTED_ESAVETYPE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_DEFAULTVAL_COPYINIT
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#include "fable_string.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CDataOutputStream.h"
#include "engine/CWorld.h"
#include "engine/CPersistContext.h"
#include "engine/CPersistTraits.h"

template void CPersistContext::Transfer<CWorld::ESaveType>(const char* name, CWorld::ESaveType& value);
