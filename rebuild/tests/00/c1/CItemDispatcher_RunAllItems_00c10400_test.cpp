// behaviour test for 0x00C10400 CItemDispatcher::RunAllItems: collect every container item into a local list, then unlink and run each one
#include <cstdio>

struct CListItem { int m_id; int m_runs; };

struct CListImpl {                              // test implementation: array
    CListItem* m_items[8];
    int m_count;
    CListImpl(long) : m_count(0) { }
    virtual ~CListImpl() { }
    virtual void Add(void*, CListItem* item) { m_items[m_count++] = item; }
    virtual bool Remove(void*, CListItem* item)
    {
        for (int i = 0; i < m_count; ++i)
            if (m_items[i] == item) { for (int j = i + 1; j < m_count; ++j) m_items[j - 1] = m_items[j]; --m_count; return true; }
        return false;
    }
    virtual CListItem* GetFirst() { return m_count ? m_items[0] : 0; }
};

struct CTypedListBase {
    CListImpl* m_pImpl;
    CTypedListBase(CListImpl* impl) : m_pImpl(impl) { }
    virtual ~CTypedListBase() { }
};

struct CItemList : public CTypedListBase {
    CListImpl m_impl;
    CItemList(long mode) : CTypedListBase(&m_impl), m_impl(mode) { }
    ~CItemList() { }
    void Add(CListItem* item) { m_pImpl->Add(this, item); }
    bool Remove(CListItem* item) { return m_pImpl->Remove(this, item); }
    CListItem* GetFirst() { return m_pImpl->GetFirst(); }
};

struct CItemVisitor {
    virtual void Visit(CListItem* item) = 0;
    virtual ~CItemVisitor() { }
};

struct CCollectAllItems : public CItemVisitor {
    CItemList* m_pList;
    CCollectAllItems(CItemList* list) : m_pList(list) { }
    virtual void Visit(CListItem* item) { m_pList->Add(item); }   // 0x00C10720
};

struct CItemContainer {
    CListItem* m_items[4];
    int m_count;
    virtual void Enumerate(CItemVisitor* visitor) { for (int i = 0; i < m_count; ++i) visitor->Visit(m_items[i]); }
};

struct CItemDispatcher;
static CItemDispatcher* g_lastDispatcher = 0;
static int g_order[8], g_runs = 0;

struct CItemAction {
    CItemDispatcher* m_pDispatcher;
    CListItem* m_pItem;
    CItemAction(CItemDispatcher* dispatcher, CListItem* item) : m_pDispatcher(dispatcher), m_pItem(item) { }   // 0x00C1EE00
    void Run() { g_lastDispatcher = m_pDispatcher; ++m_pItem->m_runs; g_order[g_runs++] = m_pItem->m_id; }      // 0x00C1F480
};

struct CItemDispatcher {
    CItemContainer m_items;
    void RunAllItems();
};

void CItemDispatcher::RunAllItems()
{
    CItemList list(1);
    CCollectAllItems visitor(&list);
    m_items.Enumerate(&visitor);
    CListItem* item = list.GetFirst();
    while (item) {
        list.Remove(item);
        CItemAction action(this, item);
        action.Run();
        item = list.GetFirst();
    }
}

int main()
{
    CListItem items[3] = { { 10, 0 }, { 20, 0 }, { 30, 0 } };
    CItemDispatcher d;
    d.m_items.m_count = 0;
    d.RunAllItems();                                              // empty container: nothing runs
    if (g_runs != 0) { std::printf("00c10400_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) d.m_items.m_items[i] = &items[i];
    d.m_items.m_count = 3;
    d.RunAllItems();
    if (g_runs != 3 || g_order[0] != 10 || g_order[1] != 20 || g_order[2] != 30 || g_lastDispatcher != &d) { std::printf("00c10400_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (items[i].m_runs != 1) { std::printf("00c10400_TEST FAIL\n"); return 1; }
    std::printf("00c10400_TEST PASS\n");
    return 0;
}