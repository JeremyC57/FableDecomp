// CAIStateGroup_MazeTutorialBattleRespondToLightning::FindHitEvent() -- true when the creature's entity-event component holds an event of type 0x1b created after FrameOfLastHit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00954C60, length 105 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (the const instantiation of the inlined interface lookup, type
// 0x42 = CTCEntityEvents, `test byte ptr [eax+28h],4`), CTCEntityEvents::HasEvent<NEntityEvents::
// CIsEventOfTypeCreatedAfter>(const CIsEventOfTypeCreatedAfter&) @ 0x00956650 (manifest name).
//
// Shape: `if (GetInterface(&events, 0x42)) { if (events->HasEvent(CIsEventOfTypeCreatedAfter(0x1b,
// FrameOfLastHit))) return true; } return false;` -- the predicate temporary {type, frame} is built in the
// lookup key's frame slot (8-byte frame: key and predicate share it), the nested form keeps `mov al,1` before
// the shared `xor al,al` tail. PDB: FrameOfLastHit +0x20. The engine header for the class is flattened
// data-only: local model.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;     // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

namespace NEntityEvents {
    class CIsEventOfTypeCreatedAfter { public:
        long Type;                              // EEntityEventType
        long Frame;
        CIsEventOfTypeCreatedAfter(long type, long frame) : Type(type), Frame(frame) { }
    };
}

class CTCEntityEvents { public:                        // interface type 0x42
    bool HasEvent(const NEntityEvents::CIsEventOfTypeCreatedAfter& predicate);   // 0x00956650
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
};

struct CAIStateGroup_MazeTutorialBattleRespondToLightning : CAIStateGroupBase {
    unsigned char _base14[0x0c];                // +0x14..+0x1f (base state)
    long FrameOfLastHit;                        // +0x20
    bool FindHitEvent();
};

bool CAIStateGroup_MazeTutorialBattleRespondToLightning::FindHitEvent()
{
    CTCEntityEvents* events;
    if (PBrain->PCreature->GetInterface(&events, 0x42)) {
        if (events->HasEvent(NEntityEvents::CIsEventOfTypeCreatedAfter(0x1b, FrameOfLastHit)))
            return true;
    }
    return false;
}