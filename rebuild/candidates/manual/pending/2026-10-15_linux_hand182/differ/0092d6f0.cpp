// CAIStateGroup_GoToBed::IsRunnable() -- re-release a live bed, then runnable when the sleeper was not stamped in
// Grade: DIFFER(ours v retail) -- 196v198: (1) retail loads the sleeper's +0x10 stamp into esi between the GetPWorld call and GFGetCurrentFrame (sub eax,esi), ours subtracts the memory operand after the call; (2) retail schedules mov ecx,edi between FindFreeBed's test al,al and jne, ours after the jne | tried: static frame accessor, inline GetFramesSince(stamp) member (hoists the load above GetPWorld), non-static accessor + positive bed test
// the last 120 frames, no crime or trespass is pending, it is sleep time and a bed (or bandit bed) is free;
// on success the group re-runs OnReplace.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092D6F0, length 198 bytes (+ int3 padding). Vtable slot 11.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (inlined IS_THING_ALIVE on PBed +0x14), OnReplace
// @ 0x0092C740 (qualified, twice), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, 0xd4 = bit 20 of
// +0x38), the brain world accessor @ 0x008FCFC0 (object expression of the static frame accessor),
// GFGetCurrentFrame @ 0x0049D870, IsUnreportedCrimeKnown @ 0x0092D060, IsHeroTrespassingPerceived @ 0x0092CB30,
// IsInSleepTime @ 0x0092C4E0, FindFreeBed(bool) @ 0x0092D1F0, FindFreeBanditBed @ 0x0092C8A0.
// Shape: a chain of early `return false` tests sharing the `xor al,al` exit; the sleeper's frame stamp (+0x10) is
// read into esi before the frame call (`frame - stamp < 120`, signed `jl`).
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
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

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
struct CGameTime {
    bool IsTimeOfDayInRange(long start, long end);   // 0x006BB390
};

struct CWorld {
    unsigned char _pad0[0x1c];
    CGameTime* m_pTime;                         // +0x1c the world clock
    unsigned char _pad[0x40];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
    CWorld* GetPThingWorld();                   // 0x008FCFA0
};

struct CTCSleeper {                             // interface 0xd4
    unsigned char _pad[0x10];
    long m_frameStarted;                        // +0x10
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
struct CWorldFrame {
    long GetCurrentFrame() { return GFGetCurrentFrame(); }
};

struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    CBaseIntelligentPointer PBed;               // +0x14 CIntelligentPointer<CThing>
    long m_sleepTimeOffset;                     // +0x1c
    long m_bedTimeDelay;                        // +0x20
    long m_wakeTimeDelay;                       // +0x24
    CWorldFrame* GetPFrameWorld();              // 0x008FCFC0 (GetPWorld)
    bool IsUnreportedCrimeKnown();              // 0x0092D060
    bool IsHeroTrespassingPerceived();          // 0x0092CB30
    bool IsInSleepTime();                       // 0x0092C4E0
    bool FindFreeBed(bool allowOwned);          // 0x0092D1F0
    bool FindFreeBanditBed();                   // 0x0092C8A0
    virtual void OnReplace();                   // 0x0092C740
    virtual bool IsRunnable();
};

bool CAIStateGroup_GoToBed::IsRunnable()
{
    if (IS_THING_ALIVE(PBed))
        CAIStateGroup_GoToBed::OnReplace();
    CTCSleeper* sleeper;
    if (PBrain->PCreature->GetInterface(&sleeper, 0xd4)
        && GetPFrameWorld()->GetCurrentFrame() - sleeper->m_frameStarted < 120)
        return false;
    if (IsUnreportedCrimeKnown() || IsHeroTrespassingPerceived() || !IsInSleepTime())
        return false;
    if (FindFreeBed(false) || FindFreeBanditBed()) {
        CAIStateGroup_GoToBed::OnReplace();
        return true;
    }
    return false;
}
