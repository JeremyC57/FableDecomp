// CAIStateGroup_EatAtTavern::GetBuilding() -- the nearest building with property 0x20 (a tavern) to the creature, in the village its villager component (interface 0x23) belongs to, or null
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
class C3DVector { public: float X, Y, Z; };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing { public:
    const C3DVector& GetPos() const;    // 0x004C73D0
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CThingBuilding;

class CTCVillage { public:
    CThingBuilding* AIStateGroup_FindNearestBuildingWithProperty(const C3DVector& position, unsigned long property);   // 0x00658E38
};
struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

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
    if (PBrain->Creature->GetInterface(&villager, 0x23)) {
        if (GFGetPVillageInterface(villager) != 0)
            return GFGetPVillageInterface(villager)->AIStateGroup_FindNearestBuildingWithProperty(PBrain->Creature->GetPos(), 0x20);
    }
    return 0;
}