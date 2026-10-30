// CGameScriptInterface::SetToKeepHeroAbilitiesDuringCutscenes @ 0x00890b50
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CMainGameComponent { public:
    char pad[0x1c];
    void* PPlayerManager;      // +0x1c
};

// helper @ 0x449970 : __fastcall(void* that) -> void* (object)
class CPlayer;
class CPlayerManager { public: CPlayer& GetMainPlayer(); }; /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */

void __fastcall SetToKeepHeroAbilitiesDuringCutscenes(CGameScriptInterface* self, int /*edx*/, bool keep)
{
    unsigned char* obj = (unsigned char*)((void*)&((CPlayerManager*)self->Component->PPlayerManager)->GetMainPlayer());
    obj[0x224] = (unsigned char)keep;
}