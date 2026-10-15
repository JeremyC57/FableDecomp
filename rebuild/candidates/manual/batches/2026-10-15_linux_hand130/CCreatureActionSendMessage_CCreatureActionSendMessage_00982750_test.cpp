// Behaviour test for 0x00982750 CCreatureAction_SendMessage copy constructor: def add-ref, position, receiver, extra.
#include <cstdio>
struct CBaseObject { int id; };
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CMessageEventDef { int m_refCount; void AddRef() { ++m_refCount; } };
struct CDefPointer {
    CMessageEventDef* m_pDef;
    CDefPointer(CMessageEventDef* d) : m_pDef(d) { }
    CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) m_pDef->AddRef(); }
};
struct CCreatureActionBase { int m_base; CCreatureActionBase() : m_base(7) { } CCreatureActionBase(const CCreatureActionBase& o) : m_base(o.m_base) { } };
struct CCreatureAction_SendMessage : CCreatureActionBase {
    CDefPointer MessageDef;
    float PositionX, PositionY, PositionZ;
    CIntelligentPointer PReceiver;
    int Extra;
    CCreatureAction_SendMessage(CMessageEventDef* d) : MessageDef(d), PositionX(0), PositionY(0), PositionZ(0), Extra(0) { }
    CCreatureAction_SendMessage(const CCreatureAction_SendMessage& other)
        : CCreatureActionBase(other), MessageDef(other.MessageDef), PositionX(other.PositionX), PositionY(other.PositionY),
          PositionZ(other.PositionZ), PReceiver(other.PReceiver), Extra(other.Extra) { }
};
int main()
{
    CMessageEventDef def; def.m_refCount = 1;
    CBaseObject receiver; receiver.id = 3;
    CCreatureAction_SendMessage a(&def);
    a.PositionX = 1.5f; a.PositionY = -2.0f; a.PositionZ = 4.0f; a.PReceiver.SetPItem(&receiver); a.Extra = 99;
    CCreatureAction_SendMessage b(a);
    if (def.m_refCount != 2 || b.MessageDef.m_pDef != &def || b.PositionX != 1.5f || b.PositionY != -2.0f || b.PositionZ != 4.0f
        || b.PReceiver.GetPItem() != &receiver || b.Extra != 99 || b.m_base != 7) { std::printf("00982750_TEST FAIL\n"); return 1; }
    CCreatureAction_SendMessage c(0);
    CCreatureAction_SendMessage d(c);   // null def: no add-ref
    if (d.MessageDef.m_pDef != 0 || def.m_refCount != 2) { std::printf("00982750_TEST FAIL\n"); return 1; }
    std::printf("00982750_TEST PASS\n");
    return 0;
}
