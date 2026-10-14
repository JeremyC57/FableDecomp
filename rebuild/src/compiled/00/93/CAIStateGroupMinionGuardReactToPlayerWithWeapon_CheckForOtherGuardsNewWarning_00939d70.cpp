// CAIStateGroup_MinionGuardReactToPlayerWithWeapon::CheckForOtherGuardsNewWarning() -- look for a newer
// "guard warning" message (type 0x1b) that this creature could see or hear since the one already recorded; on
// a hit adopt its message id (+0x40 -> m_lastWarningId at +0x1c) and its source (+0x3c -> m_pWarningSource +0x14).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00939D70, length 89 bytes (+ int3 padding).
// Callees: the state group's second world accessor @ 0x008FCFA0 (message manager at +0x60),
// CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, CanBeSeenOrHeard>> @ 0x00939CB0 taking
// the bound filter by pointer.
// Shape: the filter is the expression chain `IsOfType(0x1b) && CanBeSeenOrHeard(m_lastWarningId)` passed by
// const reference: 0x10-byte frame with the type filter at +4, the "newer than" filter at +8 and the Bind2
// pair of pointers at +0xc..+0x10; the manager is reached through an inline accessor so its load follows
// the filter stores. `if (ev) { copy +0x40 then +0x3c; return true; } return false;` with `mov al,1` /
// shared `xor al,al`. Base layout local (PDB names); member names descriptive (no engine
// header for this class).
struct CMessageEvent {
    unsigned char _pad[0x3c];
    void* m_pSource;                        // +0x3c
    long m_id;                              // +0x40
};


// Retail builds message filters as an expression chain: `IsOfType(n) && HasExtraData() && IsForThing(t)`.
// Each `operator&&` returns a CFilter_Bind2/CFilter_Bind3 holding POINTERS to its operands (the filter
// temporaries live in the caller's frame until the call returns; the empty one-byte filter has a user
// constructor so it takes no store and sits in the high byte of a slot), and FindMostRecentMessage takes the
// bound filter by const reference (one `lea ; push`).
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

struct CMessageEventFilter_HasExtraData {
    CMessageEventFilter_HasExtraData() { }
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

struct CMessageEventFilter_CanBeSeenOrHeard {      // "newer than the recorded id" per the bytes
    long m_sinceId;
    CMessageEventFilter_CanBeSeenOrHeard(long sinceId) : m_sinceId(sinceId) { }
};

typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_CanBeSeenOrHeard> CWarningFilter;

struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CWarningFilter& filter);   // 0x00939CB0
};

struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];               // +0x04..+0x13
    CWorld* GetPThingWorld();               // 0x008FCFA0
};

struct CAIStateGroup_MinionGuardReactToPlayerWithWeapon : CAIStateGroupBase {
    void* m_pWarningSource;                 // +0x14
    long _pad18;                            // +0x18
    long m_lastWarningId;                   // +0x1c
    bool CheckForOtherGuardsNewWarning();
};

bool CAIStateGroup_MinionGuardReactToPlayerWithWeapon::CheckForOtherGuardsNewWarning()
{
    CMessageEvent* event = GetPThingWorld()->GetPMessageManager()->FindMostRecentMessage(
        CMessageEventFilter_IsOfType(0x1b) && CMessageEventFilter_CanBeSeenOrHeard(m_lastWarningId));
    if (event) {
        m_lastWarningId = event->m_id;
        m_pWarningSource = event->m_pSource;
        return true;
    }
    return false;
}