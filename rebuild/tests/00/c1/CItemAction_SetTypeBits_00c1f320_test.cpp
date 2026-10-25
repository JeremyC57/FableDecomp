// Behaviour test for 00c1f320: CItemAction::SetTypeBits -- rebuild the item as type (kind byte | new bits 8..31): detach the old item, replace it through the factory, delete it, attach the new one
#include <cstdio>

struct CItemDispatcher { int m_id; };
static int g_seq[8]; static int g_step = 0; static int g_live = 0;
struct CItem {
    unsigned int m_type; CItemDispatcher* m_attachedTo; CItem* m_source;
    CItem(unsigned int type) : m_type(type), m_attachedTo(0), m_source(0) { ++g_live; }
    virtual unsigned int GetKind() { return m_type & 0xFF; }
    virtual void Attach(CItemDispatcher* d) { m_attachedTo = d; g_seq[g_step++] = 4; }
    virtual void Detach(CItemDispatcher* d) { if (m_attachedTo == d) m_attachedTo = 0; g_seq[g_step++] = 1; }
    virtual ~CItem() { --g_live; g_seq[g_step++] = 3; }
};
struct CFactory {
    static CItem* Create(unsigned int type, CItem* source) { CItem* i = new CItem(type); i->m_source = source; g_seq[g_step++] = 2; return i; }
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
    void SetTypeBits(unsigned int typeBits)
    {
        unsigned int type = m_pItem->GetKind() | (typeBits & 0xFFFFFF00u);
        ReplaceItem(type);
    }
};

int main()
{
    CItemDispatcher d = { 1 };
    CItemAction a; a.m_pDispatcher = &d; a.m_pItem = new CItem(0x00010103u); a.m_pItem->m_attachedTo = &d;
    CItem* old = a.m_pItem;
    a.SetTypeBits(0x00020277u);
    if (a.m_pItem == old || a.m_pItem->m_type != 0x00020203u || a.m_pItem->m_source != old || a.m_pItem->m_attachedTo != &d) {
        std::printf("00c1f320_TEST FAIL\n"); return 1;
    }
    int want[4] = { 1, 2, 3, 4 };
    for (int i = 0; i < 4; ++i) if (g_seq[i] != want[i]) { std::printf("00c1f320_TEST FAIL\n"); return 1; }
    if (g_live != 1) { std::printf("00c1f320_TEST FAIL\n"); return 1; }
    delete a.m_pItem;
    std::printf("00c1f320_TEST PASS\n");
    return 0;
}