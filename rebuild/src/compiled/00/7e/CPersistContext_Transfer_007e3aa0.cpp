// CPersistContext::Transfer<CDefString>(const char*, CDefString&) -- the two-argument member template: Transfer(name, value,
// GetDefaultVal<CDefString>()) with the default def string (TablePos -1) built in a one-dword frame slot
// Leaf: ??$Transfer@VCDefString@@@CPersistContext@@QAEXPBDAAVCDefString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E3AA0.
// Callees: CPersistContext::Transfer<CDefString>(const char*, CDefString&, const CDefString&) @ 0x007E39D0.
// NF39 (2026-11-16): moved off its local CPersistContext stand-in (Transfer(const char*, long&)) onto the shared engine/CPersistContext.h:
//   Ego_r.pdb / FableWin.pdb name sets ??$Transfer@VCDefString@@@CPersistContext@@QAEXPBDAAVCDefString@@@Z; its landed caller (probe:
//   0x00F2D6E0, ten CDefString members) already binds it. GetDefaultVal's body is the one the landed GetDefaultVal rows define.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#include "engine/CDefString.h"
#include "engine/CPersistContext.h"

template <class T> T FABLE_FASTCALL CPersistContext::GetDefaultVal()
{
    T value = T();
    return value;
}

template void CPersistContext::Transfer<CDefString>(const char* name, CDefString& value);
