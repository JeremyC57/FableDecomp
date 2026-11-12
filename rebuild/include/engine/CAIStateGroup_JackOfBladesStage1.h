#ifndef FABLE_ENGINE_CAISTATEGROUP_JACKOFBLADESSTAGE1_H
#define FABLE_ENGINE_CAISTATEGROUP_JACKOFBLADESSTAGE1_H
#include "engine/CAIStateGroup_JackOfBladesBase.h"
class C3DVector;
// Original complete Ego_r type, native base size4c and remaining original members.
class CAIStateGroup_JackOfBladesStage1:public CAIStateGroup_JackOfBladesBase {
#ifdef FABLE_RETAIL_JACK_STAGE1_UPDATE
public:virtual void Update();
protected:
 void PowerUpSword();
 EAIActionReturn MeleeCombat();
#endif
protected:
 bool HasAttacked;
 long ZigZagCounter;
 CIntelligentPointer<CThing> PObstruction,PShieldThing;
 bool GetRushToPosForLunge(C3DVector& rush_to_pos);
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_JackOfBladesStage1)==0x64);
#endif
