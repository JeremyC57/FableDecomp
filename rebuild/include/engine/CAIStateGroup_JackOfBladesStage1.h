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
#ifdef FABLE_RETAIL_JACK_STAGE1_HASHITSCAUSEDRUSH
 // Opt-in (row 0x00972CC0 MeleeCombat): Ego_r.pdb / FableWin.pdb ?HasHitsCausedRush@CAIStateGroup_JackOfBladesStage1@@IAE_NXZ
 // (protected; retail 0x00972340, landed), declaration only.
protected:
 bool HasHitsCausedRush();   // 0x00972340
#endif
#ifdef FABLE_RETAIL_JACK_STAGE1_ONACTIVATE
 // Opt-in (wf20, row 0x00972990): Ego_r.pdb / FableWin.pdb ?OnActivate@CAIStateGroup_JackOfBladesStage1@@UAEXXZ (vtable 0x0128D6B4 slot 2).
public:
 virtual void OnActivate();
#endif
protected:
 bool HasAttacked;
 long ZigZagCounter;
 CIntelligentPointer<CThing> PObstruction,PShieldThing;
 bool GetRushToPosForLunge(C3DVector& rush_to_pos);
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_JackOfBladesStage1)==0x64);
#endif
