#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWCOMBATMULTIPLIER_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWCOMBATMULTIPLIER_H
#ifdef FABLE_RETAIL_HUD_COMBAT_MULTIPLIER_PASS30
#include "engine/NPlayerGui_CDrawBase.h"
class CEnginePrimitivePrepareInfo;
namespace NPlayerGui {
// Ego_r and FableWin both place this class in player_gui_draw_modules. All its fields belong to CDrawBase.
class CDrawCombatMultiplier : public CDrawBase {
public:
    void Draw(const CEnginePrimitivePrepareInfo& info);
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawCombatMultiplier)==0x28);
#endif
#endif
