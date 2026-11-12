// CPersistContext::GetDefaultVal<unsigned long>() -- the default value a named Transfer passes when the setting is absent: 0
// Leaf: ??$GetDefaultVal@K@CPersistContext@@KIKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040E260, 3 bytes: `xor eax,eax ; ret` -- a protected static __fastcall member template (no arguments).
// Names (agentFAM10, 2026-11-12; was _global::sub_0040e260, a behavioural "constant-return member" T::Get stand-in): FableWin.pdb
//   ??$GetDefaultVal@K@CPersistContext@@KIKXZ (KI = protected static __fastcall); its 3 call sites are all in the landed
//   NScript::CQ_SunnyvaleMasterData::OnPersist 0x00CDC070, which already binds this symbol (reloc_bindings) through
//   `context.Transfer(name, member)` = `Transfer(name, member, GetDefaultVal<T>())` (engine/CPersistContext.h); the member persisted right
//   after the call has the matching Ego_r.pdb type, e.g. AchievementsWorthyOfSong (+0xDC, ulong). Its neighbour 0x0040E1E0 is the landed GetDefaultVal<CCharString>.
// Body: the PDB gives no source; `T value = T(); return value;` returns the value-initialised default (the CCharString instance's local +
//   copy-out shape, no NRVO in VC7.1).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "engine/CPersistContext.h"

template <class T> T FABLE_FASTCALL CPersistContext::GetDefaultVal()
{
    T value = T();
    return value;
}

template unsigned long FABLE_FASTCALL CPersistContext::GetDefaultVal<unsigned long>();
