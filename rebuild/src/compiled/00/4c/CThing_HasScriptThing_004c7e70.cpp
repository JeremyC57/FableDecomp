// CThing::HasScriptThing() const -- ask the quest manager's game script-thing manager whether this thing has a script thing
// Leaf: ?HasScriptThing@CThing@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C7E70, 18 bytes: `mov eax,ecx ; mov ecx,[013B89FCh] ; mov ecx,[ecx+4] ; push eax ;
//   call 0x00704790 ; ret`.
// Callee: CGameScriptThingManager::HasScriptThing(const CThing&) 0x00704790 (Ego_r.pdb 0x00724B80 + FableWin.pdb 0x027C7510) on
//   CQuestManager::PGameScriptThingManager (+0x04) of the CTBaseSingletonNoDefaultAlloc<CQuestManager> singleton (PClass 0x013B89FC).
// Naming (NF108, 2026-11-19, q4487): Ego_r.pdb 0x004C9A30 + FableWin.pdb ?HasScriptThing@CThing@@QBE_NXZ; engine/CThing.h declares it at
//   0x004C7E70; CTCVillage::AddThingToVillage 0x0065B854 calls it `mov ecx,esi ; call` and binds it. Was the __fastcall stand-in
//   CThing_HasScriptThing(CThing*) over local models; re-authored on engine/CThing.h + engine/CQuestManager.h (generated layout:
//   PGameScriptThingManager_Data +0x04) + engine/CTBaseSingletonNoDefaultAlloc.h. CGameScriptThingManager has no shared header: the
//   one-member declaration is the 0x004C7E90 row's.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CQuestManager.h"
#include "engine/CTBaseSingletonNoDefaultAlloc.h"

class CGameScriptThingManager { public:
    bool HasScriptThing(const CThing& thing);   // 0x00704790 ?HasScriptThing@CGameScriptThingManager@@QAE_NABVCThing@@@Z
};

bool CThing::HasScriptThing() const
{
    // The manager is fetched into a local first; retail parks `this` in eax across the loads.
    CGameScriptThingManager* manager = CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().PGameScriptThingManager_Data;
    return manager->HasScriptThing(*this);
}
