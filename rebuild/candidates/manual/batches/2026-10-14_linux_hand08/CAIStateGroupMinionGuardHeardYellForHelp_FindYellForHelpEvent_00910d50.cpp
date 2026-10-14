// CAIStateGroup_MinionGuardHeardYellForHelp::FindYellForHelpEvent() -- the most recent "yell for help" message
// (type 11) that carries extra data and concerns this guard's creature.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00910D50, length 69 bytes (+ int3 padding).
// Callees: the state group's second world accessor @ 0x008FCFA0 (PBrain->ThingManager->World; the manifest's
// "CPlayer::GetCurrentMode" is a propagated label) whose message manager sits at +0x60, then
// CMessageEventManager::FindMostRecentMessage<CFilter_Bind3<IsOfType, HasExtraData, OccuredBetween>> @ 0x00910CF0
// taking the bound filter by pointer (`push`, `ret 4`).
// Shape: the filter is the expression chain `IsOfType(11) && HasExtraData() && IsForThing(creature)` passed by
// const reference (see the note on the chain below): 0x18-byte frame holding the three filter temporaries
// (the creature at +8, the type 11 at +4, the empty filter in the high byte of the first slot, `lea edx,
// [esp+3]`) and the Bind3 of three pointers at +0xc..+0x14. The manager is reached through an inline
// accessor so its load follows the filter stores; no `this` save, the result pointer is returned as the
// call leaves it. Base layout local (PDB names); no engine header for this class.
// one-byte filter in the high byte of the first slot, `lea edx,[esp+3]`) and the Bind3 of three pointers at
struct CThing;
struct CThingCreatureBase;
struct CMessageEvent;

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

struct CMessageEventFilter_IsForThing {
    CThing* m_pThing;
    CMessageEventFilter_IsForThing(CThing* thing) : m_pThing(thing) { }
};

typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_HasExtraData, CMessageEventFilter_IsForThing> CYellFilter;

struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CYellFilter& filter);   // 0x00910CF0
};

struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* Creature;                       // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
    CWorld* GetPThingWorld();               // 0x008FCFA0
};

struct CAIStateGroup_MinionGuardHeardYellForHelp : CAIStateGroupBase {
    CMessageEvent* FindYellForHelpEvent();
};

CMessageEvent* CAIStateGroup_MinionGuardHeardYellForHelp::FindYellForHelpEvent()
{
    return GetPThingWorld()->GetPMessageManager()->FindMostRecentMessage(
        CMessageEventFilter_IsOfType(11) && CMessageEventFilter_HasExtraData() && CMessageEventFilter_IsForThing(PBrain->Creature));
}
