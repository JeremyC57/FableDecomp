#ifndef FABLE_ENGINE_CWIDESCREENMAGICPAUSEENTITIES_H
#define FABLE_ENGINE_CWIDESCREENMAGICPAUSEENTITIES_H
#include "engine/CGameScriptInterfaceBase.h"
// Layout: original Ego_r TPI2871: one protected CGameScriptInterfaceBase* Game, sizeof4.
// Native Outro e1384f caches receiver through paired PauseAllNonScriptedEntities calls.
class CWideScreenMagicPauseEntities {
protected:
    CGameScriptInterfaceBase* Game;
public:
    CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& game):Game(&game){Game->PauseAllNonScriptedEntities(true);}
    ~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}
};
FABLE_STATIC_ASSERT(sizeof(CWideScreenMagicPauseEntities)==4);
#endif
