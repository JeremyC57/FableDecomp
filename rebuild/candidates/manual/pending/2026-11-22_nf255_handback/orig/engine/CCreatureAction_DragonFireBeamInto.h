#ifndef FABLE_ENGINE_CCREATUREACTION_DRAGONFIREBEAMINTO_H
#define FABLE_ENGINE_CCREATUREACTION_DRAGONFIREBEAMINTO_H
#ifdef FABLE_RETAIL_JACKDRAGON_HOVER_PASS27
#include "engine/CCombatActionBase.h"
// FableWin 02BE2090 and Ego_r put Timer after the combat-action base.
class CCreatureAction_DragonFireBeamInto : public CCombatActionBase {
public:
    CCreatureAction_DragonFireBeamInto(CThingCreatureBase& creature,CThing& target,float time);
    virtual CCreatureActionBase* Clone() const;
private:
    float Timer;
};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_DragonFireBeamInto)==0x118);
#endif
#endif
