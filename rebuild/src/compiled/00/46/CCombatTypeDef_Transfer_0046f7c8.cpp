// CCombatTypeDef::Transfer(CPersistContext&) -- serialize combat-def fields in retail order
// Leaf: ?Transfer@CCombatTypeDef@@UAEXAAVCPersistContext@@@Z
// Retail 0x0046F7C8, 274 bytes, virtual `ret 4` (slot 18).
// Port (round 4, NF40): the NativeVector32 / NativeMap32 views and the local CPersistContext model (stand-in TransferLongVector /
//   TransferStringVector / TransferMap / TransferCombatType / TransferRecoil members, non-virtual QAE leaf) are replaced by
//   engine/CCombatTypeDef.h (FABLE_RETAIL_CCOMBATTYPEDEF layout; the FABLE_RETAIL_CCOMBATTYPEDEF_TRANSFER opt-in declares the
//   override ?Transfer@CCombatTypeDef@@UAEXAAVCPersistContext@@@Z, Ego_r.pdb / FableWin.pdb) and engine/CPersistContext.h:
//   TransferVector<long> 0x00466A47, TransferVector<CCharString> 0x004331F9, TransferMap<long,ECombatCreatureType> 0x0046FA22,
//   Transfer<ECombatCreatureType> 0x0046F8DA, Transfer<ECombatStrikeRecoilStyle> 0x0046F97E.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_CCOMBATTYPEDEF_TRANSFER
#include "fable_string.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CPersistContext.h"

void CCombatTypeDef::Transfer(CPersistContext& context)
{
    context.TransferVector(DefenderCombatRingOccupationNumbers);
    context.Transfer(DefenderWheelOuterRingStartIndex);
    context.Transfer(DefenderWheelAllowAttackerStrafing);
    context.TransferVector(CombatSequence);
    context.TransferVector(CombatSequenceDef);
    context.Transfer(PreferOuterRing);
    context.Transfer(FrontFarLimit);
    context.Transfer(FrontMiddleLimit);
    context.Transfer(FrontNearLimit);
    context.Transfer(SideLimit);
    context.Transfer(RearLimit);
    context.Transfer(PreferredCombatDistance);
    context.TransferMap(AttackPatterns);
    context.Transfer(CombatCreatureType);
    context.Transfer(AlwaysAttackIfNearest);
    context.Transfer(KnockdownCausesStun);
    context.Transfer(RecoilStyle);
    context.Transfer(CanBeKnockedDown);
    context.Transfer(ComboContinueSpeed);
    context.Transfer(CombatFOV);
    context.Transfer(StartsAsReadyForMelee);
}
