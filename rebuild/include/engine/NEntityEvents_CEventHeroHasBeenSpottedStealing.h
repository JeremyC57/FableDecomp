#ifndef FABLE_ENGINE_EVENT_SPOTTED_STEALING_H
#define FABLE_ENGINE_EVENT_SPOTTED_STEALING_H
#ifdef FABLE_RETAIL_DEED_FRAME_PASS29
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/NEntityEvents_EEventType.h"
namespace NEntityEvents {
class CEventHeroHasBeenSpottedPickpocketing;
// Ego_d TPI 0xa8fb / 0xa8fa. Retail vtable 0x012781e0.
class CEventHeroHasBeenSpottedStealing : public CEventBase {
public:
    CEventHeroHasBeenSpottedStealing(CThing*, CThing*);
    virtual ~CEventHeroHasBeenSpottedStealing();
    virtual EEventType GetType() const;
    virtual long GetLifeTime() const;
    static EEventType FABLE_FASTCALL StatGetType() { return EVENT_HERO_HAS_BEEN_SPOTTED_STEALING; }
private:
    CIntelligentPointer<CThing> PTarget;
    CIntelligentPointer<CThing> PSpotter;
};
// Ego_d TPI 0xa906 / 0xa905. FableWin constructor 01f2a770 and predicate 01f2fef0.
class CIsEventOfType {
public:
    explicit CIsEventOfType(EEventType type) : Type(type) {}
    bool operator()(const CEventBase* event) const { return event->GetType()==Type; }
private:
    EEventType Type;
};
}
FABLE_STATIC_ASSERT(sizeof(NEntityEvents::CEventHeroHasBeenSpottedStealing)==24);
FABLE_STATIC_ASSERT(sizeof(NEntityEvents::CIsEventOfType)==4);
#endif
#endif
