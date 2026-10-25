// Behaviour test for 00c1ee60: CItemAction::CItemAction(dispatcher, key) -- bind to the dispatcher's item for the key, else create a keyed item for the key and attach it
#include <cstdio>

struct CItemDispatcher;
static int g_attached = 0; static CItemDispatcher* g_attachedTo = 0;
struct CItem {
    virtual ~CItem() { }
    virtual void Attach(CItemDispatcher* dispatcher) { ++g_attached; g_attachedTo = dispatcher; }
};
struct CKeyedItem : CItem {
    long m_key;
    CKeyedItem(long key) : m_key(key) { }
};
struct CItemDispatcher {
    long m_knownKey; CItem* m_known;
    CItem* FindItem(long key) { return key == m_knownKey ? m_known : 0; }
};

struct CItemAction {
    CItemDispatcher* m_pDispatcher;
    CItem* m_pItem;
    CItemAction(CItemDispatcher* dispatcher, long key)
        : m_pDispatcher(dispatcher), m_pItem(dispatcher->FindItem(key))
    {
        if (!m_pItem) {
            m_pItem = new CKeyedItem(key);
            m_pItem->Attach(m_pDispatcher);
        }
    }
};

int main()
{
    CItem known;
    CItemDispatcher d; d.m_knownKey = 42; d.m_known = &known;
    CItemAction a(&d, 42);
    if (a.m_pDispatcher != &d || a.m_pItem != &known || g_attached != 0) { std::printf("00c1ee60_TEST FAIL\n"); return 1; }
    CItemAction b(&d, 7);
    CKeyedItem* k = static_cast<CKeyedItem*>(b.m_pItem);
    if (!k || k == &known || k->m_key != 7 || g_attached != 1 || g_attachedTo != &d) { std::printf("00c1ee60_TEST FAIL\n"); return 1; }
    delete k;
    std::printf("00c1ee60_TEST PASS\n");
    return 0;
}