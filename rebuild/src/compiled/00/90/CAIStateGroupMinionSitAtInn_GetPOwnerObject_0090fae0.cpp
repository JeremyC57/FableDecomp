// CAIStateGroup_MinionSitAtInn::GetPOwnerObject(CThing** ppOwner) -- the nearest building of the villager's village
// with property 0x20 (the inn), measured from the creature's position.
// Leaf: ?GetPOwnerObject@CAIStateGroup_MinionSitAtInn@@MAE_NPAPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090FAE0, length 115 bytes (+ int3 padding), `ret 4`. Vtable slot 16.
// Callees: const CVectorMap::LowerBound @ 0x004365B0 (the inline PeekTC<const CTCVillageMember>, TCI_VILLAGE_MEMBER 0x23, bit 3 of +0x24),
// CTCVillageMember::GetPVillage @ 0x007EE540, CThing::GetPos @ 0x004C73D0 on the creature,
// CTCVillage::GetNearestBuildingWithProperty(const C3DVector&, ESimBuildingProperty) @ 0x00658E38.
// Shape: nested ifs sharing one `xor al,al` exit; on success the building is stored through the out pointer and `mov al,1`.
// NF67 (round 7): ported onto the shared headers. The local CThing / CThingB / CThingPos / CInterfaceMap(B) / CBaseIntelligentPointer /
//   CAIBrain / CAIStateGroupBase / CTCVillage / CTCVillageMember models are gone; the class chain comes from overlay opt-ins
//   FABLE_RETAIL_CAISTATEGROUP_MINIONSITATINN (-> _MINIONSITONSEATBASE -> _USESSEAT, new overlay header engine/CAIStateGroup_UsesSeat.h).
//   0x00658E38 now binds ?GetNearestBuildingWithProperty@CTCVillage@@QAEPAVCThingBuilding@@ABVC3DVector@@W4ESimBuildingProperty@@@Z (nf41:e93).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_TAVERN
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CAISTATEGROUP_MINIONSITATINN
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_MinionSitAtInn.h"

bool CAIStateGroup_MinionSitAtInn::GetPOwnerObject(CThing** ppOwner)
{
    const CTCVillageMember* villager;
    if (GetCreature().PeekTC(&villager)) {
        CTCVillage* village = villager->GetPVillage();
        if (village != 0) {
            CThingBuilding* inn = village->GetNearestBuildingWithProperty(GetCreature().GetPos(), (ESimBuildingProperty)0x20);
            if (inn != 0) {
                *ppOwner = inn;
                return true;
            }
        }
    }
    return false;
}
