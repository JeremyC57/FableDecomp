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

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

namespace NEntityEvents {
    struct CIsEventOfTypeCreatedAfter {
        long Type;                              // EEntityEventType
        long Frame;
        CIsEventOfTypeCreatedAfter(long type, long frame) : Type(type), Frame(frame) { }
    };
}

struct CTCEntityEvents {                        // interface type 0x42
    bool HasEvent(const NEntityEvents::CIsEventOfTypeCreatedAfter& predicate);   // 0x00956650
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
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