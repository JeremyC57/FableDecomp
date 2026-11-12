#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTSHOTSTRUCK_H
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTSHOTSTRUCK_H
// Layout and names: both original PDBs; donor026fd4f0 and native007c1000.
#include "engine/NEntityEvents_CEventHitBase.h"
namespace NEntityEvents {class CEventShotStruck:public CEventBase {public:
 CEventShotStruck(C3DVector& pos):StrikePos(pos){}
 static EEventType FABLE_FASTCALL StatGetType(){return EVENT_SHOT_STRUCK;}
 virtual EEventType GetType()const{return StatGetType();}
private:C3DVector StrikePos;
};}
FABLE_STATIC_ASSERT(sizeof(NEntityEvents::CEventShotStruck)==20);
#endif
