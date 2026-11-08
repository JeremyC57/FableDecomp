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
#ifdef FABLE_RETAIL_CHITLOCATION_STATREGISTERCLASS
    // Retail opt-in (row 0x004EE23F CTCBase::RegisterTCs): Ego_r.pdb / FableWin.pdb ?StatRegisterClass@CHitLocation@NHitLocations@@SIXXZ
    // (public static __fastcall), retail 0x008162F0 (between the NHitLocations primitives and CHitLocation::SetEnabled 0x00816390, as Ego_r
    // 0x0080ED20 follows CTCTargetingPlayer::StatRegisterClass); RegisterTCs calls it with the other classes' static setup. Declaration only.
    static void __fastcall StatRegisterClass();                          // 0x008162F0
#endif
};
}
FABLE_STATIC_ASSERT(sizeof(NHitLocations::CHitLocation)==0x18);
#endif
