// CTCSpecialAbilities::SetupFireballSpell() -- if the creature is free to cast a will fireball, make sure it carries a CTCFireballSpell component (adding one and setting its weapon state); false when it is busy
// Leaf: ?SetupFireballSpell@CTCSpecialAbilities@@AAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072C930, 143 bytes. Ego_r.pdb ?SetupFireballSpell@CTCSpecialAbilities@@AAE_NXZ (private).
// Callees: CCreatureAction_CastWillFireballSpell(CThingCreatureBase*) 0x008BCD10 and its destructor 0x0062E900 (a frame temporary),
//   CThingCreatureBase::IsFreeToPerformAction(const CCreatureActionBase&) const 0x00662210 (Ego_r.pdb, header overlay),
//   CCharString(const char*, long) 0x0099EBF0, CThing::AddTC 0x004C9D60, ~CCharString 0x0099EAE0, the static __fastcall
//   CTCSpecialAbilities::CalculateWeaponState(CThingCreatureBase&) 0x0072B940 (Ego_r.pdb ?CalculateWeaponState@CTCSpecialAbilities@@SI?AW4EWEAPON_STATE@1@AAVCThingCreatureBase@@@Z),
//   CTCFireballSpell::SetWeaponState(CTCSpecialAbilities::EWEAPON_STATE) 0x007C8660 (Ego_r.pdb).
// Layout: CTCSpecialAbilities (Ego_r.pdb, retail -8 after the STLport list): Creature +0x18.
// Shape: HasTC(TCI_FIREBALL_SPELL = 0x9D) through the retail CThing header (`mov eax,[esi+18h] ; test [eax+30h],...`); the literal
//   passed straight to AddTC (the implicit conversion temporary is built first, then the pushes, then `mov ecx,[esi+18h]`).
// Header debt: engine/CCreatureAction_CastWillFireballSpell.h is a flat generated layout without the CCreatureActionBase chain, so
//   the action class here is the donor-include extension with a conversion to the base reference IsFreeToPerformAction takes.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
class CCreatureActionBase;
#define CCreatureAction_CastWillFireballSpell DonorCCreatureAction_CastWillFireballSpell
#include "engine/CCreatureAction_CastWillFireballSpell.h"
#undef CCreatureAction_CastWillFireballSpell
class CCreatureAction_CastWillFireballSpell : public DonorCCreatureAction_CastWillFireballSpell {
public:
    CCreatureAction_CastWillFireballSpell(CThingCreatureBase* creature);   // 0x008BCD10
    ~CCreatureAction_CastWillFireballSpell();                             // 0x0062E900
    operator const CCreatureActionBase&() const { return *(const CCreatureActionBase*)this; }
};
class CTCFireballSpell;
class CTCSpecialAbilities : public CTCBase {
public:
    enum EWEAPON_STATE { };
    static EWEAPON_STATE __fastcall CalculateWeaponState(CThingCreatureBase& creature);   // 0x0072B940
private:
    bool SetupFireballSpell();
    bool AbilityMutexLock;                      // +0x0c
    bool UsingBed;                              // +0x0d
    long CurrentAbility;                        // +0x10
    unsigned char UndeadFollowers[4];           // +0x14 list<CIntelligentPointer<CThingCreatureBase> >
    CThingCreatureBase* Creature;               // +0x18 (Ego_r.pdb +0x20)
};
class CTCFireballSpell : public CTCBase {
public:
    void SetWeaponState(CTCSpecialAbilities::EWEAPON_STATE state);   // 0x007C8660
};

bool CTCSpecialAbilities::SetupFireballSpell()
{
    if (Creature->IsFreeToPerformAction(CCreatureAction_CastWillFireballSpell(Creature))) {
        if (!Creature->HasTC(TCI_FIREBALL_SPELL)) {
            CTCFireballSpell* spell = (CTCFireballSpell*)Creature->AddTC("CTCFireballSpell", true, 0);
            spell->SetWeaponState(CalculateWeaponState(*Creature));
        }
        return true;
    }
    return false;
}