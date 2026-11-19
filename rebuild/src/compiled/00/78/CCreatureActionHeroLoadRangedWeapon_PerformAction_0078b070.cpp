// CCreatureAction_HeroLoadRangedWeapon::PerformAction() -- when the ranged weapon lives and is analogue-loading, create one
// shot of the weapon's ammo (its created-shot def's object def) at the hero for the hero's player, keep it in PAmmo, and put a
// live carryable ammo thing into the weapon's missile-load carry slot
// Leaf: ?PerformAction@CCreatureAction_HeroLoadRangedWeapon@@UAEXXZ
// Flags: none
// Inline-budget: FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_FORCEINLINE (engine/CIntelligentPointer.h per-TU opt-in) -- retail expands the
//   `PAmmo = Create(...)` assignment in place (0x0078B1AA..0x0078B1BC: GetPItem on the temporary, SetPItem); the plain inline ASSIGN body is
//   called out of line under VC7.1's caller-size budget here (graded: DIFFER 437v442).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078B070, 442 bytes, __thiscall virtual (vtable 0x0126B9D4 slot 3), plain `ret`.
// Name: Ego_r.pdb / FableWin.pdb ?PerformAction@CCreatureAction_HeroLoadRangedWeapon@@UAEXXZ + vtable slot 3 (rtti-vtables.tsv).
// Callees: IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340 / <CIntelligentPointer<CThingObject> > 0x0078B2A0 (out of line);
//   CBaseIntelligentPointer::GetPItem 0x00A01B50; CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound 0x0040F020 (the expanded
//   CThing::GetTC<CTCWeapon> / <CTCCarrying>); CTCWeapon::IsAnalogueLoading 0x005DA8F0, GetMissileLoadCarrySlot 0x005DA920,
//   GetCreatedShotDefGlobalIndex 0x005DA750 (all const); CCreatureActionBase::PeekDefinitionManager 0x00692910;
//   CDefinitionManager::GetDef<CThingShotDef> 0x0078B230; CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E) / ~CCharString
//   0x0099EAE0; CThing::GetPos 0x004C73D0; CThingObject::Create 0x00703210 (static __fastcall); CIntelligentPointer<CThingObject>
//   (CThingObject*) 0x0065E133, SetPItem 0x00A01B90, ~CBaseIntelligentPointer 0x00A01C10 (the expanded assignment);
//   CTCCarrying::PutThingInCarrySlot 0x006A1F80; the def pointee's virtual delete (slot 1) at the end of the CDefPointer.
// Port (NF101, 2026-11-19, q11260): moved off its 14-class local model onto the shared headers (0x008D5300 / 0x008F3630 define set);
//   Create now binds the PDB ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z (was the free stand-in
//   CreateThingObject), 0x005DA920 the PDB GetMissileLoadCarrySlot (was the made-up GetCategoryIdentifier), 0x0078B230 the PDB
//   GetDef<CThingShotDef> (was a CDefinitionManager::GetDef(CDefPointer<const CShotDef>*) stand-in).
// Shape: GetDef<T> is called with only its declaration visible (the header body, FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, lets
//   VC7.1 cache the def pointer and drop the release's null test; retail reloads it, 0x0078B204); the object def is read into a named local
//   before the Create call (retail reads +0x7C before building the "" name); the def release reads the count through a reference
//   (FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF: `mov esi,[eax+4] ; mov ecx,eax ; add eax,4 ; dec esi`, 0x0078B20C).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCREATUREACTION_HEROLOADRANGEDWEAPON_CLASS
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_LOADING
#define FABLE_RETAIL_CTCWEAPON_GETCREATEDSHOTDEFGLOBALINDEX
#define FABLE_RETAIL_CTCWEAPON_GETMISSILELOADCARRYSLOT
#define FABLE_RETAIL_CTCCARRYING_PUTTHING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingObject.h"
#include "engine/CThingShotDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/ThingAlive.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCCarrying.h"
#include "engine/CCreatureAction_HeroLoadRangedWeapon.h"

void CCreatureAction_HeroLoadRangedWeapon::PerformAction()
{
    CTCWeapon* ptc_weapon;
    if (IS_THING_ALIVE(PRangedWeapon) && PRangedWeapon->GetTC(&ptc_weapon)) {
        CTCCarrying* ptc_carry;
        if (PCreature->GetTC(&ptc_carry) && ptc_weapon->IsAnalogueLoading()) {
            long slot = ptc_weapon->GetMissileLoadCarrySlot();
            long shot_def_index = ptc_weapon->GetCreatedShotDefGlobalIndex();
            if (slot >= 0 && shot_def_index >= 0) {
                CDefPointer<const CThingShotDef> pshot_def;
                PeekDefinitionManager().GetDef(shot_def_index, &pshot_def);
                long object_def = pshot_def->ObjectDef;
                PAmmo = CThingObject::Create(object_def, PCreature->GetPos(), PCreature->GetPlayerNumber(), 0, 0, "");
                if (IS_THING_ALIVE(PAmmo) && PAmmo->HasTC(TCI_CARRYABLE))
                    ptc_carry->PutThingInCarrySlot(*PAmmo.Get(), slot, true);
            }
        }
    }
}
