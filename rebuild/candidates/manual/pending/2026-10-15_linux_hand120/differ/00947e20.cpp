// Grade: DIFFER(ours v retail) -- 132v132: only the Bind3.F1 = &IsOfType store is scheduled late (lane: right before the WaitForMessage pushes; retail: right after the first filter constructor)
// CAIStateGroup_GoToPub::GetPaymentRequestReplyMessage() -- wait for the barman's reply message (type 0x30) aimed at this creature; on arrival remove it and report success
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00947E20, length 132 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PBarman at +0x2c),
// CMessageEventFilter_IsTargettedAtThing(const CThing*) @ 0x006AF290 and
// CMessageEventFilter_IsOriginatedByThing(const CThing&) @ 0x006AF2B0 (byte-identical UID copies; the PDB
// CFilter_Bind3<IsOfType, IsOriginatedByThing, IsTargettedAtThing> fixes which is which),
// CAIStateGroupBase::AIAction_WaitForMessage<Bind3> @ 0x0091A670 (manifest "CCOE_AltarEvil::Main" is a
// propagated label), the state group's world accessor @ 0x008FCFC0 (PBrain->+0x1c->+0x24),
// CMessageEventManager::RemoveMessage @ 0x00493B5B.
//
// Tried: left-assoc // Shape: the creature (edi)// Shape: the creature (edi) chain (barman filter built first), right-nested A && (B && C) (type store late), this
// direct three-reference Bind3 (best), by-value Bind2 operand (extra ebp).
// Shape: the creature (edi) and the barman (ebx) are named locals read before the filter is built; the
// filter is (in retail most likely) the landed expression chain `IsOfType(0x30) && IsOriginatedByThing(*barman) &&
// IsTargettedAtThing(creature)` (operands built right to left, the Bind2 pair folded into the Bind3 frame
// record of three pointers at +0x24); this source builds the Bind3 directly from three references. Frame 0x24 = type 4 + event 4 + two UID filters 8+8 + Bind3 12.
// On EAIActionReturn 2 the event is removed through the world's message manager (+0x60, inline accessor so
// its load follows the event push) and `true` returns via `mov al,1`. No engine header for the class.
struct CThing {
    unsigned char _pad[0x18];
    unsigned __int64 UID;                       // +0x18
};
class CMessageEvent;

struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

struct CMessageEventFilter_IsOriginatedByThing {
    unsigned __int64 UID;
    CMessageEventFilter_IsOriginatedByThing(const CThing& thing);   // 0x006AF2B0
};

struct CMessageEventFilter_IsTargettedAtThing {
    unsigned __int64 UID;
    CMessageEventFilter_IsTargettedAtThing(const CThing* thing);    // 0x006AF290
};

template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> struct CFilter_Bind3 {
    const A* F1;
    const B* F2;
    const C* F3;
    CFilter_Bind3(const A& a, const B& b, const C& c) : F1(&a), F2(&b), F3(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }


typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing,
    CMessageEventFilter_IsTargettedAtThing> CReplyFilter;

struct CMessageEventManager {
    void RemoveMessage(const CMessageEvent* event);     // 0x00493B5B
};

struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
    CWorld* GetPWorld();                        // 0x008FCFC0
    long AIAction_WaitForMessage(long mode, const CReplyFilter& filter, CMessageEvent** ppEvent);   // 0x0091A670
};

struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CBaseIntelligentPointer PPub;               // +0x14 CIntelligentPointer<CThingBuilding>
    CBaseIntelligentPointer PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PTable;             // +0x24 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PBarman;            // +0x2c CIntelligentPointer<CThingAICreature>
    bool GetPaymentRequestReplyMessage();
};

bool CAIStateGroup_GoToPub::GetPaymentRequestReplyMessage()
{
    CThing* creature = PBrain->PCreature;
    CThing* barman = PBarman.GetPItem();
    CMessageEvent* event;
    if (AIAction_WaitForMessage(2, CReplyFilter(CMessageEventFilter_IsOfType(0x30), CMessageEventFilter_IsOriginatedByThing(*barman),
            CMessageEventFilter_IsTargettedAtThing(creature)), &event) == 2) {
        GetPWorld()->GetPMessageManager()->RemoveMessage(event);
        return true;
    }
    return false;
}
