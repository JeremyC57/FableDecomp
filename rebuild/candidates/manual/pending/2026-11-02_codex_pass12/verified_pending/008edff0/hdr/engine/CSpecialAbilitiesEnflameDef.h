#ifdef FABLE_RETAIL_WILL_BUTTON_TIMER
#ifndef FABLE_ENGINE_CSPECIALABILITIESENFLAMEDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESENFLAMEDEF_H
#include "engine/CParentDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
// Original complete 0x1c16b in Ego_r; native timing loads confirm retail vector reductions.
class CSpecialAbilitiesEnflameDef : public CParentDefClassBase {
public:
    float CamDist[4];
    float CamHeight[4];
    float CamFOV[4];
    float CamSwitchTime[4];
    float LevelUpTimings[3];
    float Damage[4];
    std::vector<float,std::allocator<float> > RadiusOfEffect;
    float EnflameCombatMultiplierFactor;
};
#endif

#else

#endif
