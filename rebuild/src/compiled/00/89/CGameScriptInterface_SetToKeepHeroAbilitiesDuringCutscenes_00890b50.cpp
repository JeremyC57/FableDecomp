// CGameScriptInterface::SetToKeepHeroAbilitiesDuringCutscenes @ 0x00890b50
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

void __fastcall SetToKeepHeroAbilitiesDuringCutscenes(CGameScriptInterface* self, int /*edx*/, bool keep)
{
    unsigned char* obj = (unsigned char*)((void*)&((CPlayerManager*)self->Component->PPlayerManager)->GetMainPlayer());
    obj[0x224] = (unsigned char)keep;
}