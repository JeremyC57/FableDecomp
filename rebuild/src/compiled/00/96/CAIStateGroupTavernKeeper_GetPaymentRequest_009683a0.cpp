// CAIStateGroup_TavernKeeper::GetPaymentRequest() -- wait for a "payment request" message (type 0x30) sent by
// this creature; on success adopt its creature originator as PPayee and its extra data as JugSlot.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009683A0, length 192 bytes (+ int3 padding).
// Callees: CMessageEventFilter_IsOriginatedByThing ctor @ 0x006AF290, the state group's message wait
// WaitForMessage<CFilter_Bind2<IsOfType, IsOriginatedByThing>>(mode 2, filter, &PaymentRequestMessage) @ 0x00969760
// (returns 2 on a hit; the manifest's "CCOE_AltarEvil::Main" label is propagated), CBaseIntelligentPointer
// GetPItem @ 0x00A01B50 / ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10, CIntelligentPointer
// <CThingAICreature> vtable 0x0123E7E0.
// Shape: the filter chain `IsOfType(0x30) && IsOriginatedByThing(creature)` passed by const reference (frame: type,
// unused slot, Bind2 pair, 8-byte UID); the event's typed originator is an inline accessor (null unless the
// thing's +0x6c low five bits are all set) assigned through a converting temporary. As in the landed
// CAIStateGroup_Drunk::IsRunnable 0x00922600: the Bind2 has a constructor storing both addresses, the creature is a
// named local, and the template callee is a same-TU out-of-line stand-in that reads the bound filter (so the &type
// store is scheduled ahead of the IsOriginatedByThing constructor call, as in retail). Engine header is padding only
// (PDB names PPayee +0x2c, JugSlot +0x5c, PaymentRequestMessage +0x64), so a local model is used.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CThing { public:
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

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
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

// CMessageEvent (PDB): Type +8, OriginatorUID +0x20, ExtraData +0x3c, MessageID +0x40, POriginator +0x48
class CMessageEvent { public:
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

class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

class CMessageEventFilter_IsOriginatedByThing { public:
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF290 (pointer) -- twin 0x006AF2B0 takes a reference
    CMessageEventFilter_IsOriginatedByThing(const CThing& thing);    // 0x006AF2B0
};

template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> class CFilter_Bind3 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing> CTypeFromFilter;
typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing, CMessageEventFilter_IsOriginatedByThing> CTypeFromToFilter;

struct CAIStateGroup_TavernKeeper : CAIStateGroupBase {
    CBaseIntelligentPointer PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PPub;               // +0x24 CIntelligentPointer<CThingBuilding>
    CIntelligentPointerCreature PPayee;         // +0x2c CIntelligentPointer<CThingAICreature>
    long JugActionPointIndex;                   // +0x34
    float JugPos_X, JugPos_Y, JugPos_Z;         // +0x38
    unsigned char JugRHSet[0x18];               // +0x44 CRightHandedSet
    long JugSlot;                               // +0x5c
    float AverageWalkSpeed;                     // +0x60
    CMessageEvent* PaymentRequestMessage;       // +0x64
    long WaitForMessage(long mode, const CTypeFromFilter& filter, CMessageEvent** ppEvent);   // 0x00969760
    bool GetPaymentRequest();
};
long TmplSink_009683a0(const void* self, long seen, CMessageEvent** ppEvent);   // stand-in: the work WaitForMessage does (outside the bytes graded here)
#pragma auto_inline(off)
__declspec(noinline) long CAIStateGroup_TavernKeeper::WaitForMessage(long mode, const CTypeFromFilter& filter, CMessageEvent** ppEvent)
{
    return TmplSink_009683a0(this, mode + *(const long*)&filter, ppEvent);
}
#pragma auto_inline(on)

bool CAIStateGroup_TavernKeeper::GetPaymentRequest()
{
    CThing* creature = PBrain->PCreature;
    if (WaitForMessage(2, CMessageEventFilter_IsOfType(0x30) && CMessageEventFilter_IsOriginatedByThing(creature),
                       &PaymentRequestMessage) == 2) {
        PPayee = PaymentRequestMessage->GetPOriginatorCreature();
        JugSlot = PaymentRequestMessage->ExtraData;
        return true;
    }
    return false;
}