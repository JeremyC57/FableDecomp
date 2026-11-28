#ifndef FABLE_ENGINE_CWIDESCREENMAGICPAUSEENTITIES_H
#define FABLE_ENGINE_CWIDESCREENMAGICPAUSEENTITIES_H
#include "engine/CGameScriptInterfaceBase.h"
// Layout: original Ego_r TPI2871: one protected CGameScriptInterfaceBase* Game, sizeof4.
// Native Outro e1384f caches receiver through paired PauseAllNonScriptedEntities calls.
class CWideScreenMagicPauseEntities {
protected:
    CGameScriptInterfaceBase* Game;
public:
#if defined(FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR) && defined(_MSC_VER)
    // Per-TU inline opt-in (Codex pass 13; like FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE): retail has one declaration, and VC7.1 expanded it in some
    // TUs and called the COMDAT in others (Ego_r 0x00C9EC04 / FableWin 0x0359FD38 keep out-of-line copies; 14 Codex pass 13 NScript TUs
    // expand both sites where a plain inline ctor stays a call).
    __forceinline
#endif
    CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& game):Game(&game){Game->PauseAllNonScriptedEntities(true);}
#ifdef FABLE_RETAIL_ORCHARD_FARMER_INLINE_VIRTUAL
    ~CWideScreenMagicPauseEntities() { Game->PauseAllNonScriptedEntities(false); }
#else
    ~CWideScreenMagicPauseEntities(){Game->PauseAllNonScriptedEntities(false);}
#endif
};
FABLE_STATIC_ASSERT_AT(19, sizeof(CWideScreenMagicPauseEntities)==4);
#endif
