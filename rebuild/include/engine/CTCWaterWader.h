#ifndef FABLE_ENGINE_CTCWATERWADER_H
#define FABLE_ENGINE_CTCWATERWADER_H

#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"

// Ego_r and FableWin give the real 0x20-byte component. Retail 0126D95C identifies
// FrameUpdate at slot 9 and the members read by it at +0x0C, +0x14 and +0x1C.
class CTCWaterWader : public CTCBase {
public:
    CTCWaterWader(CThing&);
    virtual ~CTCWaterWader();
    virtual bool HasFrameUpdate() const;
    virtual void FrameUpdate();
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;

private:
    void IsInWater(bool&,bool&,float&) const;

    CIntelligentPointer<CThing> PParticle;
    CIntelligentPointer<CThing> PWaterfallParticle;
    bool OldWade;
};
FABLE_STATIC_ASSERT(sizeof(CTCWaterWader) == 0x20);

extern const float g_OnGroundTolerance;


#ifdef FABLE_RETAIL_CTCWATERWADER_ISINWATER_PASS30
inline void CTCWaterWader::IsInWater(bool& ripple,bool& wade,float& water_height) const
{
    ripple = false;
    wade = false;
    water_height = 0.0f;
    if (!GetWorld().GetGameScriptInterface().IsInMovieSequence()) {
        if ((GetParentThing().GetPos() - GetWorld().GetGameLODCentre()).GetSquaredMagnitude() > GFSqr(20.0f))
            return;
        FABLE_RETAIL_ASSERT(GetParentThing().GetTCGraphicAppearance());
        if (!GetParentThing().GetTCGraphicAppearance()->WasRenderedLastFrame())
            return;
    }
    if (GetParentThing().GetThingStandingOn() != 0)
        return;
    const CTCCreatureModeManager* ptc_creature_mode_manager = 0;
    if (!GetParentThing().PeekTC(&ptc_creature_mode_manager))
        return;
    if (ptc_creature_mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_HOVER)
        return;
    water_height = PeekWorldMap().GetWaterSizeZAt(GetParentThing().GetPos());
    if (water_height > g_OnGroundTolerance) {
        float ground_height = PeekWorldMap().GetGroundSizeZAt(GetParentThing().GetPos());
        ripple = water_height - ground_height > 0.1f;
        wade = water_height - GetParentThing().GetPos().Z > 0.7f;
    }
}
#endif

#endif
