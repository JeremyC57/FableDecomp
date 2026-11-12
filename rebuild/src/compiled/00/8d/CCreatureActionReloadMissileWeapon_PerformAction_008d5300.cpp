// CCreatureAction_ReloadMissileWeapon::PerformAction() -- when the creature's carrying component holds a weapon thing (primary slot) with a weapon component whose CWeaponDef names an ammunition def (Ammo) and a load carry slot (MissileLoadCarrySlot), create the ammunition object at the creature's position for the creature's player, keep it in PAmmo and, if it is alive, evict and kill whatever living thing sits in the load slot and put the ammunition there
// Leaf: ?PerformAction@CCreatureAction_ReloadMissileWeapon@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D5300, 383 bytes (+ int3 padding), __fastcall virtual (vtable 0x012656A4 slot 3), plain `ret`.
// Name: rtti-vtables.tsv (slot 3 of the class vtable, FinishAction 0x008D5480 is slot 4) and Ego_r.pdb / FableWin.pdb
//   ?PerformAction@CCreatureAction_ReloadMissileWeapon@@UAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound 0x0040F020 (the
//   expanded CThing::GetTC<CTCCarrying>); CTCCarrying::GetThingInPrimarySlot 0x006A1440; CDefClassBase::GetSubDef<CWeaponDef>
//   0x0042B1A6 (out of line COMDAT, body visible); CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 (the ""
//   temporary of the scriptName argument, literal 0x0122D70E); CThing::GetPos 0x004C73D0; CThingObject::Create 0x00703210 (static
//   __fastcall, six arguments); CIntelligentPointer<CThingObject>(CThingObject*) 0x0065E133, operator= 0x007B82C0 and
//   ~CBaseIntelligentPointer 0x00A01C10 (the temporary of `PAmmo = Create(...)`); IS_THING_ALIVE<CIntelligentPointer<CThingObject> >
//   0x0078B2A0 and IS_THING_ALIVE<CThing*> 0x0040DCE0 (both out of line here); CTCCarrying::GetThingInCarrySlot 0x006A11F0,
//   RemoveThing 0x006A19B0, PutThingInCarrySlot 0x006A1F80; CThing::Kill 0x004C9B80; the def pointee's virtual Delete (slot 1) on
//   the last reference (the inline ~CDefPointer<const CWeaponDef>).
// Layout: retail CCreatureActionBase is 0xA8 bytes (PCreature CIntelligentPointer<CThingCreatureBase> +0x08); the class's one own
//   member PAmmo (Ego_r.pdb CIntelligentPointer<CThingObject>) follows at +0xA8. No shared header exists for this class, so it is
//   declared here over the shared engine/CCreatureActionBase.h. CThing UsingComponents +0x20 (TCI_CARRYING 70 = bit 6 of +0x28,
//   TCI_WEAPON 72 = bit 8 of +0x28), Components +0x44, PBaseDef +0x70, PlayerNumber +0x90; CWeaponDef MissileLoadCarrySlot +0x94,
//   Ammo +0xA4.
// Shape: `this` stays spilled in the frame and PCreature (this+8) is re-formed for each `PCreature->` access (three GetPItem calls:
//   GetTC, GetPlayerNumber, GetPos -- the Create arguments are evaluated right to left, so the CCharString("") temporary is built
//   first and destroyed last, after the CIntelligentPointer temporary of the assignment). The carrying component lives in ebx, Ammo
//   in edi, the slot in ebp; `ammo > 0 && slot > 0` tests Ammo first although the slot is loaded between the test and the branch.
//   `existing` is a frame word because IS_THING_ALIVE<CThing*> takes it by reference out of line.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CTCCARRYING_PUTTHING
#define FABLE_RETAIL_CTCCARRYING_REMOVETHING
#define FABLE_RETAIL_CTCCARRYING_CARRYSLOT
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_string.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingObject.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CWeaponDef.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/ThingAlive.h"
#include "engine/CTCCarrying.h"

class CCreatureAction_ReloadMissileWeapon : public CCreatureActionBase {
public:
    virtual void PerformAction();
protected:
    CIntelligentPointer<CThingObject> PAmmo;                 // +0xA8
};

void CCreatureAction_ReloadMissileWeapon::PerformAction()
{
    CTCCarrying* ptc_carry;
    if (PCreature->GetTC(&ptc_carry)) {
        CThing* pweapon = ptc_carry->GetThingInPrimarySlot();
        if (pweapon && pweapon->HasTC(TCI_WEAPON)) {
            CDefPointer<const CWeaponDef> weapon_def;
            if (pweapon->PeekBaseDef().GetSubDef(&weapon_def)) {
                long ammo = weapon_def->Ammo;
                long slot = weapon_def->MissileLoadCarrySlot;
                if (ammo > 0 && slot > 0) {
                    PAmmo = CThingObject::Create(ammo, PCreature->GetPos(), PCreature->GetPlayerNumber(), 0, 0, "");
                    if (IS_THING_ALIVE(PAmmo)) {
                        CThing* existing = ptc_carry->GetThingInCarrySlot(slot);
                        if (IS_THING_ALIVE(existing)) {
                            ptc_carry->RemoveThing(*existing);
                            existing->Kill(true);
                        }
                        ptc_carry->PutThingInCarrySlot(*PAmmo.Get(), slot, true);
                    }
                }
            }
        }
    }
}