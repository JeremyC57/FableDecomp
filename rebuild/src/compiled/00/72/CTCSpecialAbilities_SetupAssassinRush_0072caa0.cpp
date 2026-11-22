// CTCSpecialAbilities::SetupAssassinRush() -- start the assassin-rush cast action on the hero (not while interfaces 0xA8/0xA9 are present): aimed at a live creature target when the targeting component has one (with the player's Z-targeting flag for a player-controlled hero), otherwise untargeted; returns whether the action was set
// Leaf: ?SetupAssassinRush@CTCSpecialAbilities@@AAE_NXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Name fix (NF93 q11451, 2026-11-19): ~CCreatureAction_CastAssassinRush 0x0072D990 is virtual (??1CCreatureAction_CastAssassinRush@@UAE@XZ: the class is
//   polymorphic, vtable 0x01284254, engine/CCreatureAction_CastAssassinRush.h on CCombatActionBase; 0x0096AE50 binds UAE); the landed local model
//   gains its vptr in place of the first 4 pad bytes (size unchanged, 0x138). Was the QAE spelling.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072CAA0, 252 bytes (plain `ret`).
// Callees: the const interface LowerBound @ 0x004365B0 (interface 8), the targeting component's slot 33 (+0x84, target thing),
//   CThing::PeekPlayer() @ 0x004C7A10, CCreatureAction_CastAssassinRush(CThingCreatureBase&, CThing&, bool, long,
//   CCountedPointer<CCreatureActionBase>) @ 0x008CE040 and (CThingCreatureBase&, long, CCountedPointer<CCreatureActionBase>)
//   @ 0x008CE100, CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0, ~CCreatureAction_CastAssassinRush
//   @ 0x0072D990 (manifest "_Dest_val<...>" is propagated).
// Layout: CTCSpecialAbilities Creature +0x18 (retail 4-byte list); CThing::UsingComponents bitset<274> +0x20 (8 = word +0x20 bit 8,
//   0xA8/0xA9 = word +0x34 bits 8/9), Components map +0x44, ThingTypeFlags +0x6C (8 creature, 0x20 player-controlled), Dead bitfield +0x91;
//   CPlayer::ZTargeting +0x20A.
// Shape: HasTC is the real STLport bitset::test (range check + `!= 0`), which keeps the three presence tests as separate word
//   loads (a hand-folded mask form lets VC merge 0xA9/0xA8 into one `test ch,3`); both actions are TEMPORARIES bound to
//   SetCurrentAction's const reference (the ctor's eax is pushed and the two set+destroy tails cross-jump); the empty counted
//   pointer is built in its argument slot (user copy constructor).
// Port (NF268, 2026-11-22, q12770; nf261 NOTES entry 4): the row-local CThing / CPlayer / CThingCreatureBase / CTCTargeter /
//   CTCSpecialAbilities / CCreatureAction_CastAssassinRush models (and the latter's conversion operator) replaced by the shared headers:
//   engine/CTCSpecialAbilities.h FULL + USE (the 0x0072CF40 UseSpecialAbility define set: Creature is the PDB reference +0x18, and the
//   own leaf becomes the private PDB spelling ?SetupAssassinRush@CTCSpecialAbilities@@AAE_NXZ, Ego_r 0x00744040 + FableWin 0x0254D9F0,
//   which 0x0072CF40 already binds; was the local model's public QAE); engine/CThing.h (HasTC on TCI_ASSASSIN_RUSH / TCI_BATTLE_CHARGE,
//   the expanded const PeekTC, IsDead, GetTypeFlags, PeekPlayer), engine/CPlayer.h (FableWin inline IsZTargeting), engine/CTCTargetingBase.h
//   (interface 8, slot 33 GetPTargetedThing; was the descriptive CTCTargeter::GetPTarget), engine/CThingCreatureBase.h (SetCurrentAction),
//   engine/CCreatureAction_CastAssassinRush.h (retail class on CCombatActionBase, its implicit destructor = 0x0072D990's pick-any COMDAT;
//   NF259 TARGETCTOR for 0x008CE040, the NF268 add-only UNTARGETEDCTOR for 0x008CE100, and NF266 INITIALISE so that any table this TU
//   emits binds slot 12 to 0x008CD470). The SetupEnflameSpell 0x0072C790 recipe.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSPECIALABILITIES_USE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH
#define FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH_TARGETCTOR
#define FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH_UNTARGETEDCTOR
#define FABLE_RETAIL_CCREATUREACTION_CASTASSASSINRUSH_INITIALISE
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CPlayer.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_CastAssassinRush.h"

bool CTCSpecialAbilities::SetupAssassinRush()
{
    if (!Creature.HasTC(TCI_ASSASSIN_RUSH) && !Creature.HasTC(TCI_BATTLE_CHARGE)) {
        const CTCTargetingBase* targeting;
        if (Creature.PeekTC(&targeting)) {
            CThing* target = targeting->GetPTargetedThing();
            if (target && !target->IsDead() && (target->GetTypeFlags() & 8)) {
                bool flag = false;
                if (Creature.GetTypeFlags() & 0x20)
                    flag = Creature.PeekPlayer().IsZTargeting();
                return Creature.SetCurrentAction(CCreatureAction_CastAssassinRush(Creature, *target, flag, 0, CCountedPointer<CCreatureActionBase>()));
            }
        }
        return Creature.SetCurrentAction(CCreatureAction_CastAssassinRush(Creature, 0, CCountedPointer<CCreatureActionBase>()));
    }
    return false;
}
