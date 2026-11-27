#ifdef FABLE_RETAIL_CTCCREATUREHITNOTIFICATION_PASS29
#ifndef FABLE_ENGINE_CTCCREATUREHITNOTIFICATION_H
#define FABLE_ENGINE_CTCCREATUREHITNOTIFICATION_H
#include "engine/CTCBase.h"
#include "engine/CEnginePrimitiveWeaponTrail.h"
#ifdef FABLE_RETAIL_CTCCREATUREHITNOTIFICATION_PASS29
namespace NCreatureHitNotification {
class CFlash {
public:
    std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> FlashGeometry;
    long CreationFrame;
};
}
class CTCCreatureHitNotification : public CTCBase {
public:
    void SetHitNotification(const CThing& attacker);
private:
    std::vector<NCreatureHitNotification::CFlash> Flashes;
};
FABLE_STATIC_ASSERT(sizeof(NCreatureHitNotification::CFlash)==8);
FABLE_STATIC_ASSERT(sizeof(CTCCreatureHitNotification)==24);
#endif
#endif
#endif
