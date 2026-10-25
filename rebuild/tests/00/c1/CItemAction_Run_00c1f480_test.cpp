// Behaviour test for 00c1f480: CItemAction::Run -- only for an item of kind 3: rebuild it as its target's type | 2 (detach, factory replace, delete, attach)
#include <cstdio>

struct CItemDispatcher { int m_id; };
struct CItemTarget {
    unsigned int m_type;
    virtual unsigned int GetType() { return m_type; }
};
static int g_live = 0, g_creates = 0;
struct CItem {
    unsigned int m_kind; unsigned int m_type; CItemTarget* m_target; CItemDispatcher* m_attachedTo;
    CItem(unsigned int kind, unsigned int type) : m_kind(kind), m_type(type), m_target(0), m_attachedTo(0) { ++g_live; }
    virtual unsigned int GetKind() { return m_kind; }
    virtual CItemTarget* GetTarget() { return m_target; }
    virtual void Attach(CItemDispatcher* d) { m_attachedTo = d; }
    virtual void Detach(CItemDispatcher*) { m_attachedTo = 0; }
    virtual ~CItem() { --g_live; }
};
struct CFactory {
    static CItem* Create(unsigned int type, CItem*) { ++g_creates; return new CItem(type & 0xFF, type); }
};

struct CItemAction {
    CItemDispatcher* m_pDispatcher;
    CItem* m_pItem;
    void ReplaceItem(unsigned int type)
    {
        m_pItem->Detach(m_pDispatcher);
        CItem* item = CFactory::Create(type, m_pItem);
        delete m_pItem;
        m_pItem = item;
        m_pItem->Attach(m_pDispatcher);
    }
    void Run()
    {
        if (m_pItem->GetKind() == 3) {
            unsigned int type = m_pItem->GetTarget()->GetType() | 2;
            ReplaceItem(type);
        }
    }
};

int main()
{
    CItemDispatcher d = { 1 };
    CItemTarget target; target.m_type = 0x00010101u;
    CItemAction a; a.m_pDispatcher = &d; a.m_pItem = new CItem(1, 0x00010101u); a.m_pItem->m_target = &target;
    CItem* first = a.m_pItem;
    a.Run();                                   // kind 1: untouched
    if (a.m_pItem != first || g_creates != 0) { std::printf("00c1f480_TEST FAIL\n"); return 1; }
    first->m_kind = 3;
    a.Run();
    if (a.m_pItem == first || a.m_pItem->m_type != 0x00010103u || a.m_pItem->m_attachedTo != &d || g_creates != 1 || g_live != 1) {
        std::printf("00c1f480_TEST FAIL\n"); return 1;
    }
    delete a.m_pItem;
    std::printf("00c1f480_TEST PASS\n");
    return 0;
}