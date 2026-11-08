// NScript::CQ_HobbeCaveScript::OnPersist(CPersistContext& context) -- saves/loads QuestStartScreened and SavedBeforeCaves by name (default false)
// Leaf: ?OnPersist@CQ_HobbeCaveScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D8DCC0, 67 bytes, thiscall, `ret 4`; calls CPersistContext::Transfer<bool> 0x004045C0
//   (??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z, already bound) twice, each with a `false` default temporary.
// Identity: outer script vtable 0x012D5540 slot 4 (slot 0 scalar dtor 0x00D99E10, 1 RegisterMain 0x00D8DC00, 2 Main 0x00D8DD10,
//   3 Init 0x00D8D640; the .rdata ref 0x012D5550 is this row's only reference); Ego_r.pdb / FableWin.pdb
//   ?OnPersist@CQ_HobbeCaveScript@NScript@@UAEXAAVCPersistContext@@@Z. The keys "QuestStartScreened" (0x012CABB4) and
//   "SavedBeforeCaves" (0x012D5698) are the PDB member names at +0x69 / +0x49 (engine/CQ_HobbeCaveScript.h; Main 0x00D8DD10
//   reads both at those offsets).
// Class: engine/CQ_HobbeCaveScript.h with the chunk 858 opt-in FABLE_RETAIL_CQ_HOBBECAVESCRIPT_ONPERSIST (declares the override);
//   includes as the landed includer 0x00D8DD10.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CQ_HOBBECAVESCRIPT_ONPERSIST
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_HobbeCaveScript.h"
#include "engine/CPersistContext.h"

void NScript::CQ_HobbeCaveScript::OnPersist(CPersistContext& context)
{
    context.Transfer("QuestStartScreened", QuestStartScreened, false);
    context.Transfer("SavedBeforeCaves", SavedBeforeCaves, false);
}
