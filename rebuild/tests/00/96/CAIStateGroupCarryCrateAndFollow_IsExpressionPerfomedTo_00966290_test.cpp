// Behaviour test for 00966290: body copied from src/00966290.cpp (CAIStateGroup_CarryCrateAndFollow::IsExpressionPerfomedTo), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
};
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
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
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x45];
    unsigned char m_flags91;
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
};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
struct CAIBrain { unsigned char _pad[0x18]; CThing* PCreature; };
static CWorld* g_world = 0; static int g_getPWorld = 0;
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {}
    virtual void Cleanup() {}
    virtual void OnReplace() {}
    virtual bool IsRunnable() { return false; }
    virtual void Update() {}
    virtual void v13() {}
    virtual long GetTimeToInitiate() { return 0; }
    virtual void v15() {} virtual void v16() {} virtual void v17() {}
    CAIBrain* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    unsigned char Terminated;
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CWorld* GetPWorld() { ++g_getPWorld; return g_world; }
};
template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
static void MakeThing(CThing& t, CInterfaceMap::SEntry* entries, long type, CTCBase* component)
{
    memset(&t, 0, sizeof(t));
    entries[0].type = type; entries[0].component = component; entries[1].type = 0x100;
    t.m_interfaces.first = entries; t.m_interfaces.last = entries + 2;
    t.m_presentBits[type >> 5] |= 1UL << (type & 31);
}
struct CWorld { int dummy; };
struct CCharString { long index; };
struct CDefString {
    long m_index;
    bool operator==(const CCharString& other) const { return m_index == other.index; }
};
static int g_destroyed = 0;
struct CExpressionPayload {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_count;
    virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {}
    virtual void v07() {} virtual void v08() {} virtual void v09() {}
    virtual CDefString GetExpression() const { CDefString s; s.m_index = m_expr; return s; }
    long m_expr;
};
template <class T> struct CIVCountedPointer {
    T* Data;
    CIVCountedPointer(T* p) : Data(p) { if (Data) ++Data->m_count; }
    CIVCountedPointer(const CIVCountedPointer& o) : Data(o.Data) { if (Data) ++Data->m_count; }
    ~CIVCountedPointer() { if (--Data->m_count == 0) Data->Destroy(); }
    T* operator->() const { return Data; }
};
static CThing* g_setItem = 0; static int g_setCalls = 0;
struct CBaseIntelligentPointerS {
    virtual void v00() {}
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
    CBaseIntelligentPointerS() : m_pItem(0) {}
    ~CBaseIntelligentPointerS() {}
    void SetPItem(CThing* item) { ++g_setCalls; g_setItem = item; m_pItem = item; }
};
struct CMessage {
    virtual void v00() {}
    virtual long GetType() const { return m_type; }
    long _04;
    CBaseIntelligentPointer Sender;
    CIVCountedPointer<CExpressionPayload> Payload;
    long m_type;
    CMessage(CExpressionPayload* p, long type, CThing* sender) : Payload(p), m_type(type) { Sender.m_pItem = sender; }
};
struct CMessageList {
    struct SNode { SNode* _M_next; SNode* _M_prev; CMessage* _M_data; };
    SNode* _M_node;
};
static CMessage* g_removed = 0; static int g_removeCalls = 0;
struct CTCMessageReceiver {
    unsigned char _pad[0x0c];
    CMessageList Messages;
    void RemoveMessage(CMessage* message) { ++g_removeCalls; g_removed = message; }
};
struct CIntelligentPointerThingC : CBaseIntelligentPointerS {
    virtual void v00() {}
    CIntelligentPointerThingC() {}
    CIntelligentPointerThingC(CThing* item) : CBaseIntelligentPointerS() { SetPItem(item); }
    CIntelligentPointerThingC& operator=(const CIntelligentPointerThingC& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CAIStateGroup_CarryCrateAndFollow : CAIStateGroupBase {
    bool IsExpressionPerfomedTo(const CCharString& expression, CIntelligentPointerThingC& performer) const;
};
bool CAIStateGroup_CarryCrateAndFollow::IsExpressionPerfomedTo(const CCharString& expression, CIntelligentPointerThingC& performer) const
{
    CTCMessageReceiver* receiver;
    if (PBrain->PCreature->GetInterface(&receiver, 0x42)) {
        for (CMessageList::SNode* node = receiver->Messages._M_node->_M_next; node != receiver->Messages._M_node; node = node->_M_next) {
            CMessage* message = node->_M_data;
            if (message->GetType() == 3) {
                bool performed;
                {
                    CIVCountedPointer<CExpressionPayload> payload = message->Payload;
                    performed = payload->GetExpression() == expression;
                }
                if (performed) {
                    performer = CIntelligentPointerThingC(message->Sender.GetPItem());
                    receiver->RemoveMessage(message);
                    return true;
                }
            }
        }
    }
    return false;
}
int main()
{
    static CThing creature, senderA, senderB; static CInterfaceMap::SEntry entries[2]; static CTCMessageReceiver receiver; static CAIBrain brain;
    MakeThing(creature, entries, 0x42, (CTCBase*)&receiver); brain.PCreature = &creature;
    static CExpressionPayload pA, pB, pC; pA.m_count = 0; pA.m_expr = 11; pB.m_count = 0; pB.m_expr = 22; pC.m_count = 0; pC.m_expr = 22;
    CMessage mA(&pA, 3, &senderA), mB(&pB, 5, &senderB), mC(&pC, 3, &senderB);
    static CMessageList::SNode head, n[3]; head._M_next = &n[0]; n[0]._M_next = &n[1]; n[1]._M_next = &n[2]; n[2]._M_next = &head;
    n[0]._M_data = &mA; n[1]._M_data = &mB; n[2]._M_data = &mC; receiver.Messages._M_node = &head;
    CAIStateGroup_CarryCrateAndFollow sg; sg.PBrain = &brain;
    CIntelligentPointerThingC performer; CCharString expr; expr.index = 22;
    // 1: the third message (type 3, expression 22) matches; the type-5 one with the same expression is skipped
    g_setCalls = 0; g_removeCalls = 0;
    CHECK(sg.IsExpressionPerfomedTo(expr, performer)); CHECK(performer.m_pItem == &senderB); CHECK(g_removeCalls == 1 && g_removed == &mC);
    CHECK(g_setCalls >= 1 && g_setItem == &senderB); CHECK(pA.m_count == 1 && pB.m_count == 1 && pC.m_count == 1); CHECK(g_destroyed == 0);
    // 2: expression 11 matches the first message
    g_removeCalls = 0; expr.index = 11; performer.m_pItem = 0;
    CHECK(sg.IsExpressionPerfomedTo(expr, performer)); CHECK(performer.m_pItem == &senderA); CHECK(g_removed == &mA);
    // 3: no match
    g_removeCalls = 0; g_setCalls = 0; expr.index = 99; performer.m_pItem = 0;
    CHECK(!sg.IsExpressionPerfomedTo(expr, performer)); CHECK(g_removeCalls == 0 && g_setCalls == 0 && performer.m_pItem == 0);
    // 4: no receiver component
    creature.m_presentBits[0x42 >> 5] = 0; expr.index = 11;
    CHECK(!sg.IsExpressionPerfomedTo(expr, performer)); CHECK(g_removeCalls == 0);
    if (fails == 0) printf("00966290_TEST PASS\n"); else printf("00966290_TEST FAIL\n");
    return fails;
}