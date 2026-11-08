// NScript::CQ_HeroSoulsThunderScript::OnPersist(CPersistContext& context) -- saves/loads PlayedThunderCS and JackSpoken by name (default false)
// Leaf: ?OnPersist@CQ_HeroSoulsThunderScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D7AE30, 67 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0
//   (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound) twice, each with a `false` default temporary.
// Identity: outer script vtable 0x012D3488 slot 4 (slot 0 scalar dtor 0x00D7C6D0, 1 RegisterMain 0x00D7AD70, 2 Main 0x00D7AE80,
//   3 Init 0x00D7AD10; the .rdata ref 0x012D3498 is this row's only reference); Ego_r.pdb / FableWin.pdb
//   ?OnPersist@CQ_HeroSoulsThunderScript@NScript@@UAEXAAVCPersistContext@@@Z. The keys "PlayedThunderCS" (0x012D34C4) and
//   "JackSpoken" (0x012D34B8) are the PDB member names at +0x48 / +0x4a (engine/CQ_HeroSoulsThunderScript.h).
// Class: engine/CQ_HeroSoulsThunderScript.h (declares OnPersist); includes as the landed includer 0x00D7B510.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "stlport/fable_map.h"
#include "engine/CQ_HeroSoulsThunderScript.h"
#include "engine/CPersistContext.h"

void NScript::CQ_HeroSoulsThunderScript::OnPersist(CPersistContext& context)
{
    context.Transfer("PlayedThunderCS", PlayedThunderCS, false);
    context.Transfer("JackSpoken", JackSpoken, false);
}
