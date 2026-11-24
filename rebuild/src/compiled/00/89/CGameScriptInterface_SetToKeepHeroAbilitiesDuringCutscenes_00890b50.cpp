// CGameScriptInterface::SetToKeepHeroAbilitiesDuringCutscenes @ 0x00890b50
// Leaf: ?SetToKeepHeroAbilitiesDuringCutscenes@CGameScriptInterface@@UBEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CMainGameComponent { public:
    char pad[0x1c];
    void* PPlayerManager;      // +0x1c
};

// helper @ 0x449970 : __fastcall(void* that) -> void* (object)
class CPlayer;
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */

void CGameScriptInterface::SetToKeepHeroAbilitiesDuringCutscenes(bool keep) const
{
    CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
    unsigned char* obj = (unsigned char*)((void*)&((CPlayerManager*)self->Component->PPlayerManager)->GetMainPlayer());
    obj[0x224] = (unsigned char)keep;
}
// Flatten route (flatten_pilot v6, NF335): CGameScriptInterface_SetToKeepHeroAbilitiesDuringCutscenes -> CGameScriptInterface::SetToKeepHeroAbilitiesDuringCutscenes, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
