// CTCSpecialAbilities::SetupDrainLifeSpell() -- when the creature targets a live creature it may drain (an enemy, or any while it belongs to a player), make sure it carries a CTCDrainLife component and report success
// Leaf: ?SetupDrainLifeSpell@CTCSpecialAbilities@@AAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072B260, 198 bytes (plain `ret`). Ego_r.pdb ?SetupDrainLifeSpell@CTCSpecialAbilities@@AAE_NXZ (private).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (inside CThing::PeekTC<const CTCTargetingBase>),
//   CTCTargetingBase::GetPTargetedThing() const (virtual slot 33, +0x84), CTCEnemy::AreThingsEnemies(const CThing&, const CThing&)
//   0x0076D2F0 (static __fastcall, FableWin.pdb), CThing::GetPlayer 0x004C7A30, CTCWillResponse::IsThingLifeDrainable(const CThing*)
//   0x007E44E0 (static __fastcall, FableWin.pdb), CCharString(const char*, long) 0x0099EBF0, CThing::AddTC 0x004C9D60,
//   ~CCharString 0x0099EAE0.
// Layout: CTCSpecialAbilities (Ego_r.pdb, retail -8 after the STLport list): Creature +0x18; CPlayer::IsZTargeting() reads +0x20A.
// Shape: PeekTC<const CTCTargetingBase> (the const LowerBound); the dead test is the Dead bit (`!IsDead()`; IsAlive() compiles
//   as `not al ; test al,1`); the creature test is the +0x6C type flag bit 3; the component name is a named local.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_PLAYER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CPlayer.h"
class CTCEnemy {
public:
    static bool __fastcall AreThingsEnemies(const CThing& a, const CThing& b);   // 0x0076D2F0
};
class CTCWillResponse {
public:
    static bool __fastcall IsThingLifeDrainable(const CThing* thing);            // 0x007E44E0
};
class CTCSpecialAbilities : public CTCBase {
private:
    bool SetupDrainLifeSpell();
    bool AbilityMutexLock;                      // +0x0c
    bool UsingBed;                              // +0x0d
    long CurrentAbility;                        // +0x10
    unsigned char UndeadFollowers[4];           // +0x14 list<CIntelligentPointer<CThingCreatureBase> >
    CThingCreatureBase* Creature;               // +0x18 (Ego_r.pdb +0x20)
};

bool CTCSpecialAbilities::SetupDrainLifeSpell()
{
    const CTCTargetingBase* targeting;
    if (Creature->PeekTC(&targeting)) {
        CThing* target = targeting->GetPTargetedThing();
        if (target && !target->IsDead() && (target->GetTypeFlags() & 8)) {
            if (CTCEnemy::AreThingsEnemies(*Creature, *target) || Creature->GetPlayer().IsZTargeting()) {
                if (CTCWillResponse::IsThingLifeDrainable(target)) {
                    if (!Creature->HasTC(TCI_DRAIN_LIFE)) {
                        CCharString name("CTCDrainLife");
                        Creature->AddTC(name, true, 0);
                    }
                    return true;
                }
            }
        }
    }
    return false;
}