// CPersistContext::GetDefaultVal<long>() -- the default value a named Transfer passes when the setting is absent: 0
// Leaf: ??$GetDefaultVal@J@CPersistContext@@KIJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040E160 -- a protected static __fastcall member template (no arguments).
// NF39 (2026-11-16): renamed from its descriptive stand-in onto the PDB member template (FableWin.pdb ??$GetDefaultVal@J@CPersistContext@@KIJXZ); its landed callers
//   NScript::CQ_SunnyvaleMasterData::OnPersist 0x00CDC070 and CScriptDef::Transfer 0x00F2D6E0 already bind this symbol through
//   `context.Transfer(name, member)` = `Transfer(name, member, GetDefaultVal<T>())` (engine/CPersistContext.h). Same source shape as the
//   landed siblings GetDefaultVal<unsigned long> 0x0040E260 / GetDefaultVal<bool> 0x0040E240.
// Body: the PDB gives no source; `T value = T(); return value;` returns the value-initialised default.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "engine/CPersistContext.h"

template <class T> T FABLE_FASTCALL CPersistContext::GetDefaultVal()
{
    T value = T();
    return value;
}

template long FABLE_FASTCALL CPersistContext::GetDefaultVal<long>();
