#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWQUICKACCESSICONS_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWQUICKACCESSICONS_H
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CIntelligentPointer.h"
#include "engine/C2DCoordI.h"
#include "engine/CFadeInFadeOutBase.h"
#include "engine/CGamePlayerInterface.h"
class CEnginePrimitivePrepareInfo;
namespace NParticleEngine { class CParticleEmitter; }

namespace NPlayerGui {
// Ego_d record 0x3DF32 and the Ego_r field layout. Member offsets agree
// with the landed Draw implementation once the STLport vectors are used.
class CDrawQuickAccessIcons : public CDrawBase
{
public:
    virtual void Draw(const CEnginePrimitivePrepareInfo& info);
#ifdef FABLE_RETAIL_NPLAYERGUI_CHARGE_GRAPHIC_PASS26
private:
    // Ego_r and FableWin spell both private members; the native drawing routine calls activation at 0064B5C1.
    float GetActivationTimeForButton(long button_index);
    void DrawChargeGraphic(const CEnginePrimitivePrepareInfo& info, const C2DVector& button_offset, long button_index);
#endif
protected:
    unsigned long ChargingSoundID;
    long ChargingButtonIndex;
    float ChargingButtonTime;
    bool Charging;
    bool FullyCharged;
    float FullyChargedTime;
    EXboxControllerButton FlashButton;
    CFadeInFadeOutBase FlashFade;
    CEnginePrimitiveHandle ChargeUpPrimitiveHandle;
    std::vector<CEnginePrimitiveHandle> SpecialPrimitiveHandles;
    std::vector<CEnginePrimitiveHandle> QuickAccessSpellCrossHandles;
    std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;
    std::vector<C2DCoordI> EffectPositions;
};
}
FABLE_STATIC_ASSERT_AT(35, sizeof(NPlayerGui::CDrawQuickAccessIcons) == 0x94);
#endif
