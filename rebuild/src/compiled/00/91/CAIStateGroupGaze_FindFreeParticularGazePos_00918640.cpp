// CAIStateGroup_Gaze::FindFreeParticularGazePos(long, bool)
// Collects the gaze markers of type gazeType that are alive and not exclusively used -- from the owning building's
// CTCThingOwner object list (optionally only those inside the building), else from the creature's village
// (CTCVillageMember -> CTCVillage::VillageMarkers) -- into a local vector and returns a random one (0 if none).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00918640, 570 bytes, __fastcall, ret 8. No pragma / flags.
// Callees: virtual GetOwner (slot 14, +0x38), CVectorMap::LowerBound 0x0040F020 (GetTC expansion, TCI_THING_OWNER 0x21 /
//   TCI_VILLAGE_MEMBER 0x23), CBaseIntelligentPointer::GetPItem 0x00A01B50, CThingFilter_IsNotBeingExclusivelyUsed::operator()
//   0x006A8AF0 (temporary functor), CThing::GetPos 0x004C73D0, CThingBuilding::IsPointInside 0x0082D000,
//   vector<T*>::push_back 0x00436C80 (header body, VC calls it out of line), CTCVillageMember::GetPVillage 0x007EE540,
//   CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890 (GFRandom inline), free 0x00BFEA14.
// Shape (what made it match): the GetTC / GetPVillage failure paths are early `return 0`s (owner: `if (GetTC) {loop} else
//   return 0;`, village: two `if (!...) return 0;`), so c2 keeps the vector's _M_start in ebp only from the tail join
//   (0x0091880F) across GetPWorldSeed + GFROR13; a single-exit spelling makes _M_start one whole-function web (+5 bytes).
// Model: shared engine/CAIStateGroup_Gaze.h via FABLE_RETAIL_CAISTATEGROUP_GAZE (CAIStateGroupBase base, Ego_r members, PDB decls).
// Leaf: ?FindFreeParticularGazePos@CAIStateGroup_Gaze@@IAEPAVCThingGameObject@@J_N@Z
#include <stdlib.h>
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_GAZE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingMarker.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCThingOwner.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"

// No shared header exists for this filter. The std::unary_function base is what every engine/CThingFilter_*.h header gives its
// filter class, and the retail bytes need it here: retail takes the address of the empty filter temporary, a one-byte stack
// slot, straight before each `call 0x006A8AF0` (0x009186EA `lea ecx,[esp+17h]`, 0x009187DA `lea ecx,[esp+34h]`, each after the
// `push eax` of the argument) and never stores to it. Without the base the
// class is a POD and VC7.1 value-initialises the temporary (an extra zero store). The landed 0x006A8AF0 row declares the class
// with no base; that row never constructs one, so the base does not show in its bytes.
class CThingFilter_IsNotBeingExclusivelyUsed : public std::unary_function<CThing, bool> {
public:
    bool operator()(const CThing& thing) const;   // 0x006A8AF0
};

class CAIBrainModel;
#include "engine/GFRandom.h"

#include "engine/CAIStateGroup_Gaze.h"

CThingGameObject* CAIStateGroup_Gaze::FindFreeParticularGazePos(long gazeType, bool inside)
{
    std::vector<CThingGameObject*> found;
    CThingBuilding* building = GetOwner();
    if (building) {
        CTCThingOwner* owner;
        if (building->GetTC(TCI_THING_OWNER, &owner)) {
            for (std::list<CIntelligentPointer<CThing> >::iterator it = owner->GetObjectList().begin();
                 it != owner->GetObjectList().end(); ++it) {
                if (it->Get() != 0 && !it->Get()->IsDead() && it->Get()->GetDefGlobalIndex() == (unsigned long)gazeType
                    && CThingFilter_IsNotBeingExclusivelyUsed()(*it->Get())
                    && (!inside || building->IsPointInside(it->Get()->GetPos())))
                    found.push_back((CThingGameObject*)it->Get());
            }
        } else
            return 0;
    } else {
        CTCVillageMember* member;
        if (!GetCreature().GetTC(TCI_VILLAGE_MEMBER, &member))
            return 0;
        if (!member->GetPVillage())
            return 0;
        const std::list<CIntelligentPointer<CThingMarker> >& markers = member->GetPVillage()->VillageMarkers;
        for (std::list<CIntelligentPointer<CThingMarker> >::const_iterator it = markers.begin(); it != markers.end(); ++it) {
            if (it->Get() != 0 && !it->Get()->IsDead() && it->Get()->GetDefGlobalIndex() == (unsigned long)gazeType
                && CThingFilter_IsNotBeingExclusivelyUsed()(*it->Get()))
                found.push_back(it->Get());
        }
    }
    if (found.size() > 0)
        return found[GFRandom(found.size(), GetPWorldSeed())];
    return 0;
}