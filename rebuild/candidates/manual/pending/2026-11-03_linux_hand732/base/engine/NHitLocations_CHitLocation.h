#ifndef FABLE_ENGINE_NHITLOCATIONS_CHITLOCATION_H
#define FABLE_ENGINE_NHITLOCATIONS_CHITLOCATION_H
#include "engine/EActionRegisteredType.h"
class CThing;
// Original TPI0x25311; native815290/2a0/2b0 access PDef and enum fields.
namespace NHitLocations {
class CHitLocation {
    CThing* POwnerThing;
    unsigned char PDef[4],PArmourDef[4];
    EActionRegisteredType HitResponseAction,BlockResponseAction;
    bool Enabled;
public:
    EActionRegisteredType GetHitResponseActionType() const;
    EActionRegisteredType GetHitKnockdownActionType() const;
    EActionRegisteredType GetHitKnockdownDeathActionType() const;
};
}
FABLE_STATIC_ASSERT(sizeof(NHitLocations::CHitLocation)==0x18);
#endif
