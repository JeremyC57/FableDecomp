// CAIStateGroup_WanderAroundShops::InitStateGroup(CAIBrain&, CThingAICreature&) -- reset the buy timer and register with the idle scheduler
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00946E10, length 82 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (on the creature
// argument's interface map at +0x44, key by address), CTCIdleScheduler::RegisterIdleStateGroup
// (CAIStateGroupBase*, EIdleStateGroup 10) @ 0x00801920.
// Shape: NextFrameBuy (+0x24, engine header CAIStateGroup_WanderAroundShops.h) is zeroed, then the
// creature's inlined `GetInterface(0xd3, &scheduler)` (presence bitset at +0x20 tested as a branch:
// dword word 6 bit 19 -> `test dword ptr [eax+38h],80000h`; the key in a named local; end-or-larger
// -> the end entry; out-pointer form) and, when present, the registration. The landed CleanHouse
// InitStateGroup @ 0x00917AF0 shape with idle type 10.
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
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingAICreature { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
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
    unsigned char _pad[0x10];                   // PBrain, ScriptAIPriority, Name, flags (+0x4..+0x13)
};

struct CAIStateGroup_WanderAroundShops : CAIStateGroupBase {
    unsigned char Shop[8];                      // +0x14 CIntelligentPointer<CThing>
    unsigned char PrettyThing[8];               // +0x1c CIntelligentPointer<CThing>
    long NextFrameBuy;                          // +0x24
    virtual void InitStateGroup(CAIBrain& brain, CThingAICreature& creature);
};

void CAIStateGroup_WanderAroundShops::InitStateGroup(CAIBrain& brain, CThingAICreature& creature)
{
    NextFrameBuy = 0;
    CTCIdleScheduler* scheduler;
    if (creature.GetInterface(0xd3, &scheduler))
        scheduler->RegisterIdleStateGroup(this, 10);
}