#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CSoundMgr {
    void** vtbl;
};
class CFlaggedObject_Retail { public: void func_00991620(bool muted); };   // 0x00991620 (landed definer spelling; bool parameter)

class CASoundManager;
extern CASoundManager* PSoundManager;
extern unsigned char g_soundsEnabledFlag;

typedef void (__fastcall *StopFn)(CSoundMgr*);


void __fastcall CGameScriptInterface_EnableSounds(CGameScriptInterface* self, void* edx, char enable)
{
    CSoundMgr* mgr = ((CSoundMgr*)PSoundManager);
    if (mgr == 0)
        return;
    if (enable) {
        ((CFlaggedObject_Retail*)mgr)->func_00991620(true);
        g_soundsEnabledFlag = 1;
        return;
    }
    (*(StopFn*)((char*)mgr->vtbl + 0x40))(mgr);
    ((CFlaggedObject_Retail*)((CSoundMgr*)PSoundManager))->func_00991620(false);
    g_soundsEnabledFlag = 0;
}