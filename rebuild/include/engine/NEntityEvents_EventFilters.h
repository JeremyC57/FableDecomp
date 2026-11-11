#ifndef FABLE_ENTITY_EVENT_FILTERS_H
#define FABLE_ENTITY_EVENT_FILTERS_H
#include "stlport/fable_containers.h"
// Original Ego_r types acfe/ad38/ad79/af43; donor02c4bd00 constructors and argument types.
namespace NEntityEvents {
class CEventBase;class CEventHitBase;class CEventHitBy;class CEventHitBySpecialAbility;
class CIsEventCreatedAfter {
public:explicit CIsEventCreatedAfter(long frame):WorldFrame(frame){} bool operator()(const CEventBase*)const;
private:long WorldFrame;
};
class CIsHitEventHitByShot {public:CIsHitEventHitByShot(){} bool operator()(const CEventHitBase*)const;};
class CIsHitEventHitByMelee {public:CIsHitEventHitByMelee(){} bool operator()(const CEventHitBase*)const;};
#ifdef FABLE_RETAIL_NENTITYEVENTS_CISHITEVENTHITBY
// chunk Q15 (row 0x00738A80): Ego_r.pdb CIsHitEventHitBy (4 bytes: CThing* HitBy +0); operator() inline in retail
// (GetPHitBy 0x00810FC0, then `cmp eax,[HitBy]`). FableWin.pdb name set: CFilter_Bind2/3 instances with CIsHitEventHitBy@2.
}
class CThing;
namespace NEntityEvents {
// Ctor ??0CIsHitEventHitBy@NEntityEvents@@QAE@ABVCThing@@@Z; the PDB layout dump drops const, so HitBy's const is the ctor's.
class CIsHitEventHitBy {public:explicit CIsHitEventHitBy(const CThing& thing):HitBy(&thing){} bool operator()(const CEventHitBase* event)const;
private:const CThing* HitBy;
};
#endif
class CIsHitEventBlocked:public std::unary_function<const CEventHitBase*,bool> {public:CIsHitEventBlocked(){} bool operator()(const CEventHitBase*)const;};
}
#ifdef FABLE_RETAIL_NENTITYEVENTS_CISHITEVENTHITBY
#include "engine/NEntityEvents_CEventHitBase.h"
inline bool NEntityEvents::CIsHitEventHitBy::operator()(const CEventHitBase* event)const{return event->GetPHitBy()==HitBy;}
#endif
#endif
