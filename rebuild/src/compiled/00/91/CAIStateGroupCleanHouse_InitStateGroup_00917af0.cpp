// CAIStateGroup_CleanHouse::InitStateGroup(CAIBrain&, CThingAICreature&) -- reset, then register with the idle scheduler
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00917AF0, length 87 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (on the creature's
// interface map at +0x44, key by address), CTCIdleScheduler::RegisterIdleStateGroup
// (CAIStateGroupBase*, EIdleStateGroup 2) @ 0x00801920.
// The four members are zeroed first (+0x24 byte, +0x20 long, +0x14, +0x15 bytes: source order).
// Then the creature's component lookup for interface type 0xd3 (CTCIdleScheduler) is inlined
// (`GetInterface(type, &out)`, __forceinline): the presence bitset at +0x20 tested as a branch
// (dword words: `test dword ptr [eax+38h],80000h` is bit 0xd3; a `!= 0` bool value would come
// out as shr/and) and, when set, the sorted vector map's lower bound with the key copied into a
// named local (the `mov [esp+14h],0D3h` slot; comparing against the constant parameter keeps the
// immediate compare) -- end or a larger key -> the end entry. The out pointer is what leaves the
// component in eax before the pushes. Same shape as the landed GetInterface rows.
class CAIBrain;
class CAIStateGroupBase;

class CTCIdleScheduler { public:
    void RegisterIdleStateGroup(CAIStateGroupBase* group, long idleType);   // 0x00801920
};

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingAICreature { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    __forceinline bool GetInterface(long type, CTCIdleScheduler** out)
    {
        if (m_presentBits[type / 32] & (1 << (type % 32))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = ((CTCIdleScheduler*)it->second);
            return true;
        }
        return false;
    }
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];               // +0x4..+0x13
};

struct CAIStateGroup_CleanHouse : CAIStateGroupBase {
    bool m_startedCleaning;                 // +0x14
    bool m_finishedCleaning;                // +0x15
    unsigned char _pad2[0xa];               // +0x16..+0x1f
    long m_cleanCount;                      // +0x20
    bool m_registered;                      // +0x24
    virtual void InitStateGroup(CAIBrain& brain, CThingAICreature& creature);
};

void CAIStateGroup_CleanHouse::InitStateGroup(CAIBrain& brain, CThingAICreature& creature)
{
    m_registered = false;
    m_cleanCount = 0;
    m_startedCleaning = false;
    m_finishedCleaning = false;
    CTCIdleScheduler* scheduler;
    if (creature.GetInterface(0xd3, &scheduler))
        scheduler->RegisterIdleStateGroup(this, 2);
}