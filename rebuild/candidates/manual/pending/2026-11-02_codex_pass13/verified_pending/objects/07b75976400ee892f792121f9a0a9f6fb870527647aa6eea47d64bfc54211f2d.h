#ifndef FABLE_ENGINE_CWIDESCREENMAGICPAUSEENTITIES_H
#define FABLE_ENGINE_CWIDESCREENMAGICPAUSEENTITIES_H
#include "engine/CGameScriptInterfaceBase.h"
// Layout: original Ego_r TPI2871: one protected CGameScriptInterfaceBase* Game, sizeof4.
// Native Outro e1384f caches receiver through paired PauseAllNonScriptedEntities calls.
class CWideScreenMagicPauseEntities {
protected:
    CGameScriptInterfaceBase* Game;
public:
#ifdef FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES
    // Retail opt-in (Codex pass 13): original PDB type and donor0359fd38 prove this body.
    // Native00ec5de0 expands both sites; ordinary forms fail. Owner header-helper preference applies.
    // Fidelity: stronger expansion is needed here; the original keyword is not proven.
    __forceinline CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& game):Game(&game){Game->PauseAllNonScriptedEntities(true);}
#else
    CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& game):Game(&game){Game->PauseAllNonScriptedEntities(true);}
#endif
    ~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}
};
FABLE_STATIC_ASSERT(sizeof(CWideScreenMagicPauseEntities)==4);
#endif
