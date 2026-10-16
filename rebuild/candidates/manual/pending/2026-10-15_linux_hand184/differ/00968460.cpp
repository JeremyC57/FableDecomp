// Grade: DIFFER(172v172) -- register allocation: retail keeps &PPayee in edi and the creature filter's this in ebx (ebx pushed inside the if); ours uses ebx for &PPayee and edi for the creature | tried: left-grouped chain, named creature filter, right-grouped a && (b && c)
// CAIStateGroup_TavernKeeper::GetPaymentMessage() -- while the payee is alive, wait for its message of type 0x30 aimed at this
// creature; on a hit withdraw that message from the world's message manager and report success.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0000968460, length 172 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PPayee +0x2c, three times), the two byte-identical UID
// filter ctors @ 0x006AF290 (pointer: the creature) and 0x006AF2B0 (reference: the payee),
// WaitForMessage<CFilter_Bind3<...>>(mode 2, filter, &event) @ 0x0091A670 (returns 2 on a hit; the manifest's
// "CCOE_AltarEvil::Main" label is propagated), CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (manifest
// "CPlayer::GetCurrentMode" propagated), CMessageEventManager::RemoveMessage @ 0x00493B5B.
// Shape: the event out-slot preset to null; inline IS_THING_ALIVE on PPayee; the creature read into ebx before the
// payee fetch (named local); the filter chain `IsOfType(0x30) && From(*payee) && (From(*payee) && From(creature))` -- the
// right-grouped pair is evaluated first (right operand first), so the creature filter is built first; the Bind3 holds the three
// operand pointers. Engine header is padding only (PDB names PPayee +0x2c), so a local model is used.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CThing {
    unsigned char _pad0[0x18];
    unsigned __int64 UID;                       // +0x18
    unsigned char _pad1[0x6c - 0x20];
    unsigned long m_flags6c;                    // +0x6c low five bits all set: a creature-derived thing
    unsigned char _pad2[0x91 - 0x70];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};

struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingAICreature>, vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { SetPItem(other.GetPItem()); return *this; }
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

// CMessageEvent (PDB): Type +8, OriginatorUID +0x20, ExtraData +0x3c, MessageID +0x40, POriginator +0x48
struct CMessageEvent {
    unsigned char _pad0[0x3c];
    long ExtraData;                             // +0x3c
    long MessageID;                             // +0x40
    unsigned char _pad1[4];
    CBaseIntelligentPointer POriginator;        // +0x48 CIntelligentPointer<CThing>
    // inline typed originator: null unless the thing's low five flag bits (+0x6c) are all set
    CThing* GetPOriginatorCreature()
    {
        CThing* result = 0;
        if (POriginator.GetPItem()) {
            if (!(~POriginator.GetPItem()->m_flags6c & 0x1f))
                result = POriginator.GetPItem();
        }
        return result;
    }
};

struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

struct CMessageEventFilter_IsOriginatedByThing {
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF290 (pointer) -- twin 0x006AF2B0 takes a reference
    CMessageEventFilter_IsOriginatedByThing(const CThing& thing);    // 0x006AF2B0
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
// right-grouped form: `a && (b && c)`
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const A& a, const CFilter_Bind2<B, C>& bc)
{
    CFilter_Bind3<A, B, C> r(CFilter_Bind2<A, B>(a, *bc.m_pFirst), *bc.m_pSecond);
    return r;
}

typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing> CTypeFromFilter;
typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing, CMessageEventFilter_IsOriginatedByThing> CTypeFromToFilter;

struct CMessageEventManager {
    void RemoveMessage(const CMessageEvent* event);   // 0x00493B5B
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing, CMessageEventFilter_IsOriginatedByThing> CPaymentFilter;

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIStateGroup_TavernKeeper : CAIStateGroupBase {
    CBaseIntelligentPointer PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PPub;               // +0x24 CIntelligentPointer<CThingBuilding>
    CIntelligentPointerCreature PPayee;         // +0x2c CIntelligentPointer<CThingAICreature>
    CWorld* GetPWorld();                        // 0x008FCFC0
    long WaitForMessage(long mode, const CPaymentFilter& filter, CMessageEvent** ppEvent);   // 0x0091A670
    bool GetPaymentMessage();
};

bool CAIStateGroup_TavernKeeper::GetPaymentMessage()
{
    CMessageEvent* event = 0;
    if (IS_THING_ALIVE(PPayee)) {
        CThing* creature = PBrain->PCreature;
        CThing* payee = PPayee.GetPItem();
        if (WaitForMessage(2, CMessageEventFilter_IsOfType(0x30)
                              && (CMessageEventFilter_IsOriginatedByThing(*payee) && CMessageEventFilter_IsOriginatedByThing(creature)),
                           &event) == 2) {
            GetPWorld()->GetPMessageManager()->RemoveMessage(event);
            return true;
        }
    }
    return false;
}
