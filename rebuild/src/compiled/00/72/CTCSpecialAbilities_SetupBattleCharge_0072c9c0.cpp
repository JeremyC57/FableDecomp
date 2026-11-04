// CTCSpecialAbilities::SetupBattleCharge() -- unless the creature is already assassin-rushing or battle-charging, make it take a CCreatureAction_CastBattleCharge at its live, targetable targeted thing (z-targeting for a player's creature) or, with no such target, an untargeted one; returns whether the creature took the action
// Leaf: ?SetupBattleCharge@CTCSpecialAbilities@@AAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072C9C0, 224 bytes, plain `ret`. Ego_r.pdb / FableWin.pdb ?SetupBattleCharge@CTCSpecialAbilities@@AAE_NXZ (private).
// Callees: CVectorMap::LowerBound 0x004365B0 (the inline GetTC<CTCTargetingBase>, TCI_TARGETING = 8), CTCTargetingBase::GetPTargetedThing
//   const (virtual slot 33), CThing::PeekPlayer const 0x004C7A10 (header overlay) + the inline CPlayer::IsZTargeting const,
//   CCreatureAction_CastBattleCharge(CThingCreatureBase&, CThing&, bool, long) 0x008CE290 / (CThingCreatureBase&, long) 0x008CE2F0
//   (Ego_r.pdb / FableWin.pdb), CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) 0x006644F0, the action's
//   destructor 0x0062E900 (one merged temporary slot).
// Shape: HasTC(TCI_ASSASSIN_RUSH) || HasTC(TCI_BATTLE_CHARGE) through the retail CThing header (two STLport bitset tests: the second
//   word reloaded, 0x100 kept in ecx for the GetTC<CTCTargetingBase> presence test); the thing-type flags (+0x6C) bit 3 / bit 5 read
//   through the inline GetTypeFlags.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_PLAYER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CPlayer.h"
#include "engine/CTCTargetingBase.h"
class CCreatureActionBase;
#define CCreatureAction_CastBattleCharge DonorCCreatureAction_CastBattleCharge
#include "engine/CCreatureAction_CastBattleCharge.h"
#undef CCreatureAction_CastBattleCharge
class CCreatureAction_CastBattleCharge : public DonorCCreatureAction_CastBattleCharge {
public:
    CCreatureAction_CastBattleCharge(CThingCreatureBase& creature, CThing& target, bool zTargeting, long level);   // 0x008CE290
    CCreatureAction_CastBattleCharge(CThingCreatureBase& creature, long level);                                   // 0x008CE2F0
    ~CCreatureAction_CastBattleCharge();                                                                          // 0x0062E900
    operator const CCreatureActionBase&() const { return *(const CCreatureActionBase*)this; }
};
class CTCSpecialAbilities : public CTCBase {
private:
    bool SetupBattleCharge();
    bool AbilityMutexLock;                      // +0x0c
    bool UsingBed;                              // +0x0d
    long CurrentAbility;                        // +0x10
    unsigned char UndeadFollowers[4];           // +0x14 list<CIntelligentPointer<CThingCreatureBase> >
    CThingCreatureBase* Creature;               // +0x18 (Ego_r.pdb +0x20)
};

bool CTCSpecialAbilities::SetupBattleCharge()
{
    if (Creature->HasTC(TCI_ASSASSIN_RUSH) || Creature->HasTC(TCI_BATTLE_CHARGE))
        return false;
    CTCTargetingBase* targeting;
    if (Creature->GetTC(&targeting)) {
        CThing* target = targeting->GetPTargetedThing();
        if (target && !target->IsDead() && (target->GetTypeFlags() & 8)) {
            bool zTargeting = false;
            if (Creature->GetTypeFlags() & 0x20)
                zTargeting = Creature->PeekPlayer().IsZTargeting();
            return Creature->SetCurrentAction(CCreatureAction_CastBattleCharge(*Creature, *target, zTargeting, 0));
        }
    }
    return Creature->SetCurrentAction(CCreatureAction_CastBattleCharge(*Creature, 0));
}