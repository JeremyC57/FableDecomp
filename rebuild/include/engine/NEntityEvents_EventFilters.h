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
class CIsHitEventBlocked:public std::unary_function<const CEventHitBase*,bool> {public:CIsHitEventBlocked(){} bool operator()(const CEventHitBase*)const;};
}
#endif
