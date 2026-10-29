// CAIStateGroup_WorkAtSchool::OnReplace() -- clear the school village's "teacher present" flag when this group is replaced
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00942C10, length 82 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (PBrain->Creature's
// interface map at +0x44, key by address), GFGetPVillageInterface @ 0x007EE540 (__fastcall on the
// creature's interface-0x23 villager component: its village thing's (+0x10) interface 0x22 or
// null; "IS_THING_ALIVE" is the propagated label), called twice (test, then the store).
// Shape: the inlined `creature->GetInterface(0x23, &villager)` (presence bitset at +0x20 as a
// branch: byte word 1 bit 3 -> `test byte ptr [eax+24h],8`; key in the `push ecx` slot; end-or-larger
// -> the end entry; out-pointer form); the component stays in esi for both helper calls and the
// village's byte at +0xbc is cleared.
struct CTCVillager;

class CTCVillage { public:
    unsigned char _pad[0xbc];
    bool m_teacherPresent;                      // +0xbc
};

CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

struct CTCVillager;

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingAICreature { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    __forceinline bool GetInterface(long type, CTCVillager** out)
    {
        if (m_presentBits[type / 32] & (1 << (type % 32))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = ((CTCVillager*)it->second);
            return true;
        }
        return false;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
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

struct CAIStateGroup_WorkAtSchool : CAIStateGroupBase {
    virtual void OnReplace();
};

void CAIStateGroup_WorkAtSchool::OnReplace()
{
    CTCVillager* villager;
    if (PBrain->Creature->GetInterface(0x23, &villager)) {
        if (GFGetPVillageInterface(villager) != 0)
            GFGetPVillageInterface(villager)->m_teacherPresent = false;
    }
}