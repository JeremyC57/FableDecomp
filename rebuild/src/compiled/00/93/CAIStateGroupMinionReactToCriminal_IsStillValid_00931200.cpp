// CAIStateGroup_MinionReactToCriminal::IsStillValid() const -- while the home building is alive, refresh the three "inside the
//   house" flags (the minion itself, the live hero, any live village guard) and then defer to IsValid().
// Leaf: ?IsStillValid@CAIStateGroup_MinionReactToCriminal@@UBE_NXZ
// Grade: RELOCATION_MATCH (VC7.1 RTM 3077, default flags) -- ported onto the shared engine headers.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00931200, length 387 bytes (+ int3 padding). Vtable 0x0128B1F4 slot 5 (override of
//   CAIStateGroupBase::IsStillValid() const 0x008FD276). Ego_r.pdb 0x009544F0 / FableWin.pdb 0x02B5A2F0, same leaf.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (every PHome / PHero / village person read; IS_THING_ALIVE calls it
//   twice), CThing::GetPos @ 0x004C73D0, CThingBuilding::IsPointInside @ 0x0082D000, CVectorMap::LowerBound @ 0x0040F020 (the
//   inlined CThing::GetTC(TCI_VILLAGE_MEMBER, &p): bit 3 of UsingComponents +0x24, Components +0x44),
//   CTCVillageMember::GetPVillage @ 0x007EE540 (twice: the test, then the list), IsValid @ 0x00930EE0 (private const, same TU,
//   out of line; both exits call it).
// Inlined: IS_THING_ALIVE<CIntelligentPointer<T>> (engine/ThingAlive.h; `!= 0` then IsDead(), +0x91 bit 0), CAIStateGroupBase::
//   GetCreature() -> CAIBrain::GetCreature() (PBrain +0x04, Creature +0x18), CThing::HasTC(TCI_GUARD) as the in-place dword test
//   (`test dword ptr [eax+34h],100000h`), CTCVillage::PeekVillagePeople() (VillagePeople +0x2c) and the STLport list iterator.
// Shape: the creature is a named reference taken before the first PHome read (esi loaded before the GetPItem call); the hero flag
//   is the int-valued `&&` (mov eax,1 / xor eax,eax) stored as a byte; the people list is held by reference, so end() is re-read
//   through the list pointer each iteration (`cmp edi,[ebx]`); the guard hit stores GuardInsideHouse and breaks. `this` lives in
//   ebp, which the guard test reuses for the home building, hence the entry spill of `this` and its reload.
// The function is const but writes HeroInsideHouse / MeInsideHouse / GuardInsideHouse: the header opt-in declares them mutable
//   (neither PDB records mutability; a const_cast would give the same bytes).
// Headers: engine/CAIStateGroup_MinionReactToCriminal.h under FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIMINAL (new opt-in: the
//   polymorphic class over CAIStateGroupBase). CAIStateGroupBase.h must precede CAIBrain.h so that the inline GetCreature body is seen.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_PEEKVILLAGEPEOPLE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIMINAL
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/ThingAlive.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_MinionReactToCriminal.h"

bool CAIStateGroup_MinionReactToCriminal::IsStillValid() const
{
    if (IS_THING_ALIVE(PHome)) {
        CThingAICreature& creature = GetCreature();
        MeInsideHouse = PHome->IsPointInside(creature.GetPos());
        HeroInsideHouse = IS_THING_ALIVE(PHero) && PHome->IsPointInside(PHero->GetPos());
        GuardInsideHouse = false;
        CTCVillageMember* village_member;
        if (GetCreature().GetTC(TCI_VILLAGE_MEMBER, &village_member) && village_member->GetPVillage()) {
            const std::list<CIntelligentPointer<CThingAICreature> >& people = village_member->GetPVillage()->PeekVillagePeople();
            for (std::list<CIntelligentPointer<CThingAICreature> >::const_iterator it = people.begin(); it != people.end(); ++it) {
                const CIntelligentPointer<CThingAICreature>& person = *it;
                if (IS_THING_ALIVE(person) && person->HasTC(TCI_GUARD) && PHome->IsPointInside(person->GetPos())) {
                    GuardInsideHouse = true;
                    break;
                }
            }
        }
    }
    return IsValid();
}