// CAIStateGroup_HoverHornetQueenTakeDamage::FindHitEvent() -- look in the queen's entity-event component for a
// ranged hit (flagging bIsRangeAttack) or else a melee hit created after the last handled hit frame + 2.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095A7D0, length 188 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x004365B0 (inlined GetInterface, 0x42 = CTCEntityEvents, bit 2 of +0x28),
// CTCEntityEvents::GetEvent<const CEventHitBy, CFilter_Bind2<CIsHitEventHitByRanged, CIsEventCreatedAfter>>
// @ 0x0095C680 and the melee instantiation @ 0x0095C720 (the manifest labels both with the melee filter).
// Shape: bIsRangeAttack (+0x1d, PDB) cleared first; each query builds the filter chain `HitBy() &&
// CreatedAfter(FrameOfLastHit + 2)` in the frame (the empty hit-kind filter in the high byte of a slot, the
// frame filter at +0xc, the Bind2 pair at +0x14) and shares one out-pointer slot; `return bIsRangeAttack = true`.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
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
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
};


// Retail builds message filters as an expression chain: `IsOfType(n) && HasExtraData() && IsForThing(t)`.
// Each `operator&&` returns a CFilter_Bind2/CFilter_Bind3 holding POINTERS to its operands (the filter
// temporaries live in the caller's frame until the call returns; the empty one-byte filter has a user
// constructor so it takes no store and sits in the high byte of a slot), and FindMostRecentMessage takes the
// bound filter by const reference (one `lea ; push`) -- the landed 0x00910D50 / 0x00939D70 model.
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

struct CMessageEventFilter_HasExtraData {
    CMessageEventFilter_HasExtraData() { }
};

struct CMessageEventFilter_IsForThing {
    CThing* m_pThing;
    CMessageEventFilter_IsForThing(CThing* thing) : m_pThing(thing) { }
};

struct CMessageEventFilter_IsOriginatedByThing {
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF2B0 (0x006AF2B0 / 0x006AF290 twins, out of line, `ret 4`)
};

template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> struct CFilter_Bind3 {
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

struct CMessageEvent;
struct CMessageEventManager;
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
    CWorld* GetPThingWorld();                   // 0x008FCFA0
};

namespace NEntityEvents {
    struct CEventHitBy;
    struct CIsHitEventHitByRanged { CIsHitEventHitByRanged() { } };
    struct CIsHitEventHitByMelee { CIsHitEventHitByMelee() { } };
    struct CIsEventCreatedAfter {
        long m_frame;
        CIsEventCreatedAfter(long frame) : m_frame(frame) { }
    };
}
using namespace NEntityEvents;

typedef CFilter_Bind2<CIsHitEventHitByRanged, CIsEventCreatedAfter> CRangedHitFilter;
typedef CFilter_Bind2<CIsHitEventHitByMelee, CIsEventCreatedAfter> CMeleeHitFilter;

struct CTCEntityEvents {                        // interface 0x42
    bool GetEvent(const CEventHitBy** ppEvent, const CRangedHitFilter& filter);   // 0x0095C680
    bool GetEvent(const CEventHitBy** ppEvent, const CMeleeHitFilter& filter);    // 0x0095C720
};

struct CAIStateGroup_HoverHornetQueenTakeDamage : CAIStateGroupBase {
    unsigned char _pad14[8];
    bool LashOut;                               // +0x1c
    bool bIsRangeAttack;                        // +0x1d
    unsigned char _pad1e[2];
    long RangeAttackCount;                      // +0x20
    long FrameOfLastHit;                        // +0x24
    bool FindHitEvent();
};

bool CAIStateGroup_HoverHornetQueenTakeDamage::FindHitEvent()
{
    bIsRangeAttack = false;
    CTCEntityEvents* events;
    if (PBrain->PCreature->GetInterface(&events, 0x42)) {
        const CEventHitBy* hit;
        if (events->GetEvent(&hit, CIsHitEventHitByRanged() && CIsEventCreatedAfter(FrameOfLastHit + 2)))
            return bIsRangeAttack = true;
        if (events->GetEvent(&hit, CIsHitEventHitByMelee() && CIsEventCreatedAfter(FrameOfLastHit + 2)))
            return true;
    }
    return false;
}