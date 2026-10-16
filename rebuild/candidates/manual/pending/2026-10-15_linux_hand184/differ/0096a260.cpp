// Grade: DIFFER(227v232) -- last search only: retail puts the shot out-slot at esp+0xc and the frame filter at esp+0x10 (ours swapped) and re-normalises the bool (test al,al ; setne al) | tried: function-scope locals (0x1c frame), block-scoped hit locals, block-scoped shot local (frame matches) | next: return (bool)(GetEvent(...) != 0) / a long-returning overload; the frame filter as a named local | landing retry: named CIsEventCreatedAfter + `!= 0` -- DIFFER(233v232): the shot out-slot and frame filter still swapped (esp+0xc/esp+0x14 v esp+0x10/esp+0x10) and the frame store lands at esp+0x14
// CAIStateGroup_WhisperRetreat::FindHitEvent() -- look in the creature's entity events (interface 0x42) for a hit
// newer than FrameOfLastHit: an unblocked hit of the first kind (sets bIsRangeAttack), else one of the second
// kind, else any shot fired since then.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096A260, length 232 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x004365B0 (inlined HasTC + entry-returning FindEntry, 0x42 = bit 2 of +0x28),
// CTCEntityEvents::GetEvent<CEventHitBy, CFilter_Bind3<...>> @ 0x0096BAC0 and @ 0x0096BB70 (the manifest gives both
// the HitByMelee/unary_negate<Blocked>/CreatedAfter label; one of them is the ranged twin),
// CTCEntityEvents::GetEvent<CEventFiredShot, CIsEventCreatedAfter> @ 0x0096BC20.
// Shape: the empty filters take no stores and sit in the high bytes of the first frame slot; the frame filter and
// the Bind3 pointer triple follow; bIsRangeAttack (+0x24) cleared first and set with `mov al,1` on the first hit;
// the last search's bool is `setne`. PDB names from the engine header (bIsRangeAttack +0x24, FrameOfLastHit +0x2c);
// its layout is padding otherwise, so a local model.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

namespace NEntityEvents {
    struct CEventHitBy;
    struct CEventFiredShot;
    struct CIsHitEventHitByRanged { CIsHitEventHitByRanged() { } };   // empty predicates (one byte, no stores)
    struct CIsHitEventHitByMelee { CIsHitEventHitByMelee() { } };
    struct CIsHitEventBlocked { CIsHitEventBlocked() { } };
    struct CIsEventCreatedAfter {
        long m_frame;
        CIsEventCreatedAfter(long frame) : m_frame(frame) { }
    };
}

template<class F> struct CUnaryNegate {         // std::unary_negate<F>
    F m_functor;
    explicit CUnaryNegate(const F& f) : m_functor(f) { }
};
template<class F> inline CUnaryNegate<F> Not1(const F& f) { return CUnaryNegate<F>(f); }   // std::not1

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

typedef CFilter_Bind3<NEntityEvents::CIsHitEventHitByRanged, CUnaryNegate<NEntityEvents::CIsHitEventBlocked>,
                      NEntityEvents::CIsEventCreatedAfter> CRangedHitFilter;
typedef CFilter_Bind3<NEntityEvents::CIsHitEventHitByMelee, CUnaryNegate<NEntityEvents::CIsHitEventBlocked>,
                      NEntityEvents::CIsEventCreatedAfter> CMeleeHitFilter;

struct CTCEntityEvents {                        // interface 0x42
    bool GetEvent(NEntityEvents::CEventHitBy** ppEvent, const CRangedHitFilter& filter);           // 0x0096BAC0
    bool GetEvent(NEntityEvents::CEventHitBy** ppEvent, const CMeleeHitFilter& filter);            // 0x0096BB70
    bool GetEvent(NEntityEvents::CEventFiredShot** ppEvent, const NEntityEvents::CIsEventCreatedAfter& filter);   // 0x0096BC20
};

struct CAIStateGroup_WhisperRetreat : CAIStateGroupBase {
    unsigned char _pad_0x14[8];
    unsigned char PArenaCentre[8];              // +0x1c CIntelligentPointer<CThing>
    bool bIsRangeAttack;                        // +0x24
    unsigned char _pad_0x25[3];
    long NumHits;                               // +0x28
    long FrameOfLastHit;                        // +0x2c
    bool FindHitEvent();
};

bool CAIStateGroup_WhisperRetreat::FindHitEvent()
{
    using namespace NEntityEvents;
    bIsRangeAttack = false;
    if (PBrain->PCreature->HasTC(0x42)) {
        CTCEntityEvents* events = (CTCEntityEvents*)PBrain->PCreature->FindEntry(0x42)->component;
        {
            CEventHitBy* hit;
            if (events->GetEvent(&hit, CIsHitEventHitByRanged() && Not1(CIsHitEventBlocked()) && CIsEventCreatedAfter(FrameOfLastHit))) {
                bIsRangeAttack = true;
                return true;
            }
        }
        {
            CEventHitBy* hit;
            if (events->GetEvent(&hit, CIsHitEventHitByMelee() && Not1(CIsHitEventBlocked()) && CIsEventCreatedAfter(FrameOfLastHit)))
                return true;
        }
        {
            CEventFiredShot* shot;
            return events->GetEvent(&shot, CIsEventCreatedAfter(FrameOfLastHit));
        }
    }
    return false;
}
