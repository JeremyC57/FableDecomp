// CEnvironment::GetGlobalEnvironmentThemeIndexFromLocal(long localIndex) -- translate a local environment-theme index into the global definition index through the definition manager, using the "ENVIRONMENT_THEME_DAY" class name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B2C60, length 56 bytes (the oracle row carries 8 bytes of int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("ENVIRONMENT_THEME_DAY" @ 0x01235EB4, length -1), the definition manager's
// GetGlobalIndexFromLocal(const CCharString&, long) @ 0x009AD390 (manifest "COptimisedPrimitive::ActualToUniqueIndex" is propagated),
// CCharString::~CCharString @ 0x0099EAE0. Shape: `push ecx` reserves the 4-byte string local; a NAMED local (its address re-derived with
// `lea ecx,[esp+8]` after the argument load) rather than a temporary; the result is parked in esi across the destructor. DefinitionManager
// is +4 as in engine/CEnvironment.h.
#include "fable_string.h"

struct CGameDefinitionManager_006b2c60 {
    long GetGlobalIndexFromLocal(const CCharString& className, long localIndex);   // 0x009AD390
};

class CEnvironment { public:
    void*                           __vftable;
    CGameDefinitionManager_006b2c60* DefinitionManager;   // +0x04
    long GetGlobalEnvironmentThemeIndexFromLocal(long localIndex);
};

long CEnvironment::GetGlobalEnvironmentThemeIndexFromLocal(long localIndex)
{
    CCharString className("ENVIRONMENT_THEME_DAY", -1);
    return DefinitionManager->GetGlobalIndexFromLocal(className, localIndex);
}