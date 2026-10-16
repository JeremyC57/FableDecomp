// CCombatActionBase::GetWeaponTrailUsedInStrikeAction(long* strikeIndex) -- the active strike event at the current frame (for_each over the strike-event list), then for a live weapon thing: store the event's index and return its interface-type-0x4c component (else 0)
// Leaf: GetWeaponTrailUsedInStrikeAction
// Retail: Fable.exe (TLC, VC7.1) @ 0x00857D10, 176 bytes incl. padding (ret 4).
// Callees: std::for_each<list<CCountedPointer<CStrikeEvent>>::const_iterator, CGetActiveStrikeEventThatContainsFrame>
//          @ 0x0085A960 (LTCG custom convention: the returned functor's address in ecx, begin/end/functor on the stack,
//          callee-clean -- modelled as a __thiscall member of the result functor), CBaseIntelligentPointer::GetPItem
//          @ 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the inline GetInterface).
// Layout (landed CCombatActionBase ctor 0x00857E40 model): current frame +0x1C (CCreatureActionBase), StrikeEvents
//          +0xBC (STLport list: header-node pointer, node {next +0, prev +4, value +8}); CStrikeEvent: index +8,
//          PWeapon CIntelligentPointer<CThing> +0x14 (item +0x18); CThing: interface bitset +0x20, map +0x44, dead bit +0x91.
// Shape: the functor is built in its by-value argument slot (`mov ebx,esp`); the out-parameter GetInterface
//          (landed ScorpionUnblockable 0x008C4430 idiom) with the type local homed in the dead argument slot.
struct CTCBase;
struct CInterfaceMap {                          // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CStrikeEvent {
    unsigned char           _pad0[8];
    long                    Index;              // +0x08
    unsigned char           _pad0c[8];
    CBaseIntelligentPointer PWeapon;            // +0x14
};

struct CStrikeEventNode {                       // STLport list node of CCountedPointer<CStrikeEvent>
    CStrikeEventNode* next;                     // +0
    CStrikeEventNode* prev;                     // +4
    CStrikeEvent*     event;                    // +8
    void*             count;                    // +0xC
};
struct CStrikeEventIterator { CStrikeEventNode* node; };

struct CGetActiveStrikeEventThatContainsFrame {
    long          Frame;                        // +0
    CStrikeEvent* PFound;                       // +4
    long          BestDistance;                 // +8
    CGetActiveStrikeEventThatContainsFrame() { }
    CGetActiveStrikeEventThatContainsFrame(long frame) : Frame(frame), PFound(0), BestDistance(1000000) { }
    CGetActiveStrikeEventThatContainsFrame& ForEach(CStrikeEventIterator first, CStrikeEventIterator last,
                                                    CGetActiveStrikeEventThatContainsFrame f);   // 0x0085A960
};

struct CStrikeEventList {
    CStrikeEventNode* head;
    CStrikeEventIterator begin() const { CStrikeEventIterator i = { head->next }; return i; }
    CStrikeEventIterator end() const { CStrikeEventIterator i = { head }; return i; }
};

struct CTCWeaponTrail;

class CCombatActionBase {
public:
    void*            __vftable;                 // +0x00
    unsigned char    _pad04[0x18];
    long             CurrentFrame;              // +0x1C
    unsigned char    _pad20[0xbc - 0x20];
    CStrikeEventList StrikeEvents;              // +0xBC
    CTCWeaponTrail* GetWeaponTrailUsedInStrikeAction(long* strikeIndex);
};

CTCWeaponTrail* CCombatActionBase::GetWeaponTrailUsedInStrikeAction(long* strikeIndex)
{
    CGetActiveStrikeEventThatContainsFrame result;
    CStrikeEvent* event = result.ForEach(StrikeEvents.begin(), StrikeEvents.end(),
                                         CGetActiveStrikeEventThatContainsFrame(CurrentFrame)).PFound;
    if (event != 0) {
        CThing* weapon = event->PWeapon.GetPItem();
        if (weapon != 0 && !(weapon->m_flags91 & 1)) {
            *strikeIndex = event->Index;
            CTCWeaponTrail* trail;
            if (!(weapon->m_flags91 & 1) && weapon->GetInterface(&trail, 0x4c))
                return trail;
        }
    }
    return 0;
}
