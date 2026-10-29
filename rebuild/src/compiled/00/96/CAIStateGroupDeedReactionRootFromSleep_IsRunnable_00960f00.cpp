// CAIStateGroup_DeedReactionRootFromSleep::IsRunnable() -- never runnable itself: when the sleeping creature would react to a violent/alarming deed it just clears the sleep component's flag (waking it) so the awake reaction root takes over
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00960F00, length 118 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0xD4 =
// bitset word 6 bit 20 -> `test dword ptr [eax+38h],100000h`), CAIStateGroup_DeedReactionRoot::IsRunnable @ 0x009609B0
// (the base implementation, called non-virtually).
// Shape: the landed GetInterface out-parameter template; the sleep flag is copied into a bool local that starts
// false and is tested by its own `if (!asleep) return false;` -- the join on that flag is what keeps `push esi` in
// the prologue (an early `return` straight off the GetInterface result lets VC7.1 shrink-wrap the push past the
// interface-bit test); the component is kept in esi for the final byte store; the deed-type test `(type != 0 && type < 10) || type == 0x35 || type == 10 || type == 11`
// (short-circuit order kept: `test ; je ; cmp 0Ah ; jl` then three equality tests); every path returns false
// (one `xor al,al` tail).
// Model: local (no engine header for this class; CurrentDeedType +0x1c from struct_layouts_egor.tsv).
class CTCBase;
class CWorld;
class CThingAICreature;

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

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCSleep { public:                               // interface 0xD4
    unsigned char _pad[0xc];
    bool m_bAsleep;                             // +0x0c
};

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

struct CAIStateGroup_DeedReactionRoot : CAIStateGroupBase {
    unsigned char ThingReactingTo[8];           // +0x14 CIntelligentPointer<const CThing>
    long CurrentDeedType;                       // +0x1c EOpinionDeedType
    virtual bool IsRunnable();                  // 0x009609B0
};

struct CAIStateGroup_DeedReactionRootFromSleep : CAIStateGroup_DeedReactionRoot {
    virtual bool IsRunnable();
};

bool CAIStateGroup_DeedReactionRootFromSleep::IsRunnable()
{
    CTCSleep* sleep;
    bool asleep = false;
    if (PBrain->Creature->GetInterface(&sleep, 0xD4))
        asleep = sleep->m_bAsleep;
    if (!asleep)
        return false;
    if (CAIStateGroup_DeedReactionRoot::IsRunnable()) {
        long type = CurrentDeedType;
        if ((type != 0 && type < 10) || type == 0x35 || type == 10 || type == 11)
            sleep->m_bAsleep = false;
    }
    return false;
}