// CThingGameObject::GetWeaponTrailGraphicIndex() const -- the weapon-trail graphic for this weapon: from its CWeaponDef sub-def,
// with the knockdown variant when the wielder (the carrier, if carried) is doing a knockdown / vital / decapitating strike (a
// creature's current action) or, for a non-creature, when its trap component is the current knockdown trap; a berserking wielder
// uses CTCBerserk's trail; a wielder with a valid melee flourish not already in progress uses the def's FlourishSpecialWeaponTrail
// when it is set (> 0); otherwise the first WeaponTrails entry (walked backwards) whose augmentation type the weapon carries, else
// the type-0 entry; -1 when there is no weapon def or no entry.
// Leaf: ?GetWeaponTrailGraphicIndex@CThingGameObject@@QBEJXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00839100, 795 bytes, __fastcall, plain `ret`, long in eax (speed TU, no frame pointer).
// Callees: CDefClassBase::GetSubDef<CWeaponDef> 0x0042B1A6 (template COMDAT; its body visible noinline through
//   engine/CDefClassBase_GetSubDef.h, which keeps the def in edi), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const
//   0x004365B0 (CThing::PeekTC, GETTC_EXPANDED), CTCCarryable::GetPCarriedByThing 0x00705170, CTCTrapBase::
//   IsCurrentTrapToCauseKnockdown 0x00787C18, CCreatureActionBase virtuals IsKnockdownStrike (+0x4c) / IsVitalStrike (+0x48) /
//   IsDecapitatingAttack (+0xd8), CTCBerserk::GetWeaponTrailKnockDown 0x0078BEE0 / GetWeaponTrailAttack 0x0078BED0,
//   CTCCombat::IsMeleeFlourishValid 0x00733EE0, CTCObjectAugmentations::HasCombinationOfAugmentations 0x00766050,
//   and the WeaponTrails map's STLport members, both out of line in retail and kept so here from the real fable_map.h bodies:
//   _Rb_tree_iterator<pair<const EObjectAugmentationType,CWeaponTrailGraphicSet>,_Const_traits<..> >::operator-- 0x0083AEA0
//   (FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE; same bytes as the landed row CRetailTreeIterator_Decrement_0083aea0.cpp, grades MATCH
//   at 0x0083AEA0 from this TU) and _Rb_tree<..>::find<EObjectAugmentationType> const 0x0083B170 (FABLE_RETAIL_STLP_TREE_CONSTFIND_
//   TEMPLATE_BODY + FABLE_RETAIL_AUGMENTATION_PARTICLES, _M_find inlined; same bytes as the landed row stdRbtreeCUnprovenKey0083b170
//   ..._find_0083b170.cpp, grades MATCH at 0x0083B170). NAME_FIXES.tsv hands back these real spellings for both landed rows.
//   The CDefPointer release is the CDefPointer dtor.
// Shape: one `if (GetSubDef)` around the body so the no-def exit and the find-miss exit share the null-tested release + `or eax,-1`;
//   the reverse walk copies a named rbegin() into the loop iterator (retail `mov esi,eax` and the aligned loop head);
//   CWeaponDef.WeaponTrails is the real std::map<EObjectAugmentationType,CWeaponTrailGraphicSet> (struct_layouts_egor.tsv) through
//   the opt-in FABLE_RETAIL_CWEAPONDEF_WEAPONTRAILS; rbegin/rend const via FABLE_RETAIL_STLP_MAP_REVERSE (+ STLport reverse_iterator).
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_WEAPONTRAIL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTCTRAPBASE
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_WEAPONTRAIL
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBERSERK_HIT_EFFECT
#define FABLE_RETAIL_CTCBERSERK_WEAPONTRAIL
#define FABLE_RETAIL_AUGMENTATION_PARTICLES
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_REVERSE
#define FABLE_RETAIL_STLP_MAP_REVERSE
#define FABLE_RETAIL_CWEAPONDEF_WEAPONTRAILS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefPointer.h"
#include "engine/CWeaponDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCTrapBase.h"
#include "engine/CTCBerserk.h"
#include "engine/CTCCombat.h"
#include "engine/CTCObjectAugmentations.h"
long CThingGameObject::GetWeaponTrailGraphicIndex() const
{
    CDefPointer<const CWeaponDef> weapon_def;
    if (PBaseDef->GetSubDef(&weapon_def)) {
    const CThing* wielder = this;
    const CTCCarryable* carryable;
    if (PeekTC(TCI_CARRYABLE, &carryable) && carryable->PCarryingTC)
        wielder = carryable->GetPCarriedByThing();
    bool special = false;
    if (wielder->GetTypeFlags() & 8) {
        const CCreatureActionBase* action = ((const CThingCreatureBase*)wielder)->PeekPCurrentAction();
        if (action && (action->IsKnockdownStrike() || action->IsVitalStrike() || action->IsDecapitatingAttack()))
            special = true;
    } else {
        const CTCTrapBase* trap;
        if (PeekTC(TCI_TRAP, &trap) && trap->IsCurrentTrapToCauseKnockdown())
            special = true;
    }
    const CTCBerserk* berserk;
    if (wielder->PeekTC(TCI_BERSERK, &berserk)) {
        if (special)
            return berserk->GetWeaponTrailKnockDown();
        return berserk->GetWeaponTrailAttack();
    }
    const CTCCombat* combat;
    if (wielder->PeekTC(TCI_COMBAT, &combat) && combat->IsMeleeFlourishValid() && !combat->GetIsInMiddleOfFlourishAction()) {
        long graphic = special ? weapon_def->FlourishSpecialWeaponTrail.Knockdown : weapon_def->FlourishSpecialWeaponTrail.Attack;
        if (graphic > 0)
            return graphic;
    }
    const CTCObjectAugmentations* augmentations;
    if (PeekTC(TCI_OBJECT_AUGMENTATIONS, &augmentations)) {
        std::map<EObjectAugmentationType, CWeaponTrailGraphicSet>::const_reverse_iterator rb = weapon_def->WeaponTrails.rbegin();
        std::map<EObjectAugmentationType, CWeaponTrailGraphicSet>::const_reverse_iterator end = weapon_def->WeaponTrails.rend();
        for (std::map<EObjectAugmentationType, CWeaponTrailGraphicSet>::const_reverse_iterator it = rb; it != end; ++it) {
            EObjectAugmentationType key = (*it).first;
            if (key && augmentations->HasCombinationOfAugmentations(key)) {
                if (special)
                    return (*it).second.Knockdown;
                return (*it).second.Attack;
            }
        }
    }
    EObjectAugmentationType none = (EObjectAugmentationType)0;
    std::map<EObjectAugmentationType, CWeaponTrailGraphicSet>::const_iterator entry = weapon_def->WeaponTrails.find(none);
    if (entry != weapon_def->WeaponTrails.end()) {
        if (special)
            return (*entry).second.Knockdown;
        return (*entry).second.Attack;
    }
    }
    return -1;
}