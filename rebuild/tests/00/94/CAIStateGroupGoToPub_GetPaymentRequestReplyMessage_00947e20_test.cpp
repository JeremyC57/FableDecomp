// Behaviour test for 00947e20: src/00947e20.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CAIStateGroup_GoToPub::GetPaymentRequestReplyMessage()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_GoToPub::GetPaymentRequestReplyMessage() -- wait for the barman's reply message (type 0x30) aimed at this creature; on arrival remove it and report success
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00947E20, length 132 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PBarman at +0x2c),
// CMessageEventFilter_IsTargettedAtThing(const CThing*) @ 0x006AF290 and
// CMessageEventFilter_IsOriginatedByThing(const CThing&) @ 0x006AF2B0 (byte-identical UID copies; the PDB
// CFilter_Bind3<IsOfType, IsOriginatedByThing, IsTargettedAtThing> fixes which is which),
// CAIStateGroupBase::AIAction_WaitForMessage<Bind3> @ 0x0091A670 (manifest "CCOE_AltarEvil::Main" is a
// propagated label), the state group's world accessor @ 0x008FCFC0 (PBrain->+0x1c->+0x24),
// CMessageEventManager::RemoveMessage @ 0x00493B5B.
// Shape: the creature (edi) and the barman (ebx) are named locals read before the filter is built; the Bind3 is
// built directly from three references (frame 0x24 = type 4 + event 4 + two UID filters 8+8 + Bind3 12). The
// template callee AIAction_WaitForMessage is a same-TU out-of-line stand-in that reads the bound filter (the
// landed 0x00922600 B-17 idiom), which schedules the Bind3.F1 = &IsOfType store right after the first filter
// constructor as in retail. On EAIActionReturn 2 the event is removed through the world's message manager
// (+0x60, inline accessor so its load follows the event push) and `true` returns via `mov al,1`. No engine
// header for the class.
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
long TmplSink_00947e20(const void* self, long seen, CMessageEvent** ppEvent);   // stand-in: the work AIAction_WaitForMessage does
#pragma auto_inline(off)
__declspec(noinline) long CAIStateGroupBase::AIAction_WaitForMessage(long mode, const CReplyFilter& filter, CMessageEvent** ppEvent)
{
    return TmplSink_00947e20(this, mode + *(const long*)&filter, ppEvent);
}
#pragma auto_inline(on)

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

// ---- stubs and driver ----
void CBaseIntelligentPointer::v00() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return (CThing*)m_pItem; }
CAIStateGroupBase::~CAIStateGroupBase() { }
static const CThing* g_orig; static const CThing* g_target;
CMessageEventFilter_IsOriginatedByThing::CMessageEventFilter_IsOriginatedByThing(const CThing& thing) { g_orig = &thing; UID = thing.UID; }
CMessageEventFilter_IsTargettedAtThing::CMessageEventFilter_IsTargettedAtThing(const CThing* thing) { g_target = thing; UID = thing->UID; }
static CMessageEventManager g_mgr; static CWorld g_world;
CWorld* CAIStateGroupBase::GetPWorld() { return &g_world; }
static const CMessageEvent* g_removed; static int g_removes = 0;
void CMessageEventManager::RemoveMessage(const CMessageEvent* event) { ++g_removes; g_removed = event; CHECK(this == &g_mgr); }
static long g_ret = 2; static long g_type = -1; static long g_mode = -1; static int g_waits = 0;
static CMessageEvent* g_ev = (CMessageEvent*)0x1234;
long TmplSink_00947e20(const void* self, long seen, CMessageEvent** ppEvent)
{
    ++g_waits;
    // seen = mode + address of the bound IsOfType filter; mode is 2
    g_type = ((const CMessageEventFilter_IsOfType*)(seen - 2))->m_type;
    if (g_ret == 2) *ppEvent = g_ev;
    return g_ret;
}
int main()
{
    static long storage[0x20];
    CAIStateGroup_GoToPub* group = (CAIStateGroup_GoToPub*)storage;
    static CAIBrain brain; static CThing creature, barman;
    creature.UID = 11; barman.UID = 22;
    brain.PCreature = &creature; group->PBrain = &brain; group->PBarman.m_pItem = &barman;
    g_world.m_pMessageManager = &g_mgr;
    CHECK(group->GetPaymentRequestReplyMessage() == true);
    CHECK(g_waits == 1 && g_type == 0x30 && g_orig == &barman && g_target == &creature && g_removes == 1 && g_removed == g_ev);
    g_ret = 1;
    CHECK(group->GetPaymentRequestReplyMessage() == false && g_waits == 2 && g_removes == 1);
    g_ret = 0;
    CHECK(group->GetPaymentRequestReplyMessage() == false && g_removes == 1);
    if (fails == 0) printf("00947e20_TEST PASS\n");
    return fails;
}
