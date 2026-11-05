// CAIStateGroup_EatAtTavern::GetBuilding() -- the nearest building with property 0x20 (a tavern) to the creature, in the village its villager component (interface 0x23) belongs to, or null
// Leaf: ?GetBuilding@CAIStateGroup_EatAtTavern@@QAEPAVCThingBuilding@@XZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096D260, length 106 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x23 =
// bitset word 1 bit 3 -> `test byte ptr [eax+24h],8`), GFGetPVillageInterface @ 0x007EE540 (__fastcall on the villager
// component, twice; landed caller 0x0093EDF0), CThing::GetPosition @ 0x004C73D0,
// CTCVillage::AIStateGroup_FindNearestBuildingWithProperty(const C3DVector&, ulong) @ 0x00658E38.
// Shape: the landed GetInterface out-parameter template inside nested ifs with one shared `return 0`; the villager
// component stays in esi; the final call evaluates its arguments right to left (0x20, then the creature position)
// before re-fetching the village as its object.
// Model: local (no engine header for this class).
class CTCBase;
#include "engine/C3DVector.h"

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingBuilding;

class CTCVillage { public:
    CThingBuilding* AIStateGroup_FindNearestBuildingWithProperty(const C3DVector& position, unsigned long property);   // 0x00658E38
};
struct CTCVillager;
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_EatAtTavern : CAIStateGroupBase {
    CThingBuilding* GetBuilding();
};

CThingBuilding* CAIStateGroup_EatAtTavern::GetBuilding()
{
    CTCVillager* villager;
    if (PBrain->Creature->GetTC(TCI_VILLAGE_MEMBER, &villager)) {
        if (((CTCVillageMember*)villager)->GetPVillage() != 0)
            return ((CTCVillageMember*)villager)->GetPVillage()->AIStateGroup_FindNearestBuildingWithProperty(PBrain->Creature->GetPos(), 0x20);
    }
    return 0;
}