// Behaviour test for 0x00C104F0 CMemberHandler_00c08940::Handle: the reconstructed body verbatim over a fake container
// (enumerates a fixed item array), a fake array list impl and visitors that collect items whose key matches the event.
#include <cstdio>
#include <cstring>
struct CListItem;
struct CEvent_00c08940;
struct CMemberHandler_00c08940;

struct CListItem {
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual void Slot5(); virtual void Slot6();
    virtual void SetHandler(CMemberHandler_00c08940* handler);   // slot 7 (+0x1c)
    virtual void Notify(long value);                             // slot 8 (+0x20)
};

struct CListImpl {                              // the list's implementation object (+0x8 of the typed list)
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual void Add(void* owner, CListItem* item);        // slot 5 (+0x14)
    virtual bool Remove(void* owner, CListItem* item);     // slot 6 (+0x18)
    virtual void Slot7(); virtual void Slot8(); virtual void Slot9();
    virtual unsigned int GetCount();                       // slot 10 (+0x28)
    virtual CListItem* GetFirst();                         // slot 11 (+0x2c)
    void* m_data;
    CListItem* m_items[16]; int m_count; int m_getCountCalls;
    CListImpl(long mode);                       // 0x00C185B0
    ~CListImpl();                               // 0x00C17B10
};

struct CTypedListBase {                         // vtable 0x012B1AB0
    virtual void Slot0();
    CListImpl* m_pImpl;                         // +0x4
    CTypedListBase(CListImpl* impl) : m_pImpl(impl) { }
    ~CTypedListBase() { }
};

struct CItemList : public CTypedListBase {      // vtable 0x012B1B78
    CListImpl m_impl;                           // +0x8
    virtual void Slot0();
    CItemList(long mode) : CTypedListBase(&m_impl), m_impl(mode) { }
    ~CItemList() { }
    unsigned int GetCount() { return m_pImpl->GetCount(); }
    bool Remove(CListItem* item) { return m_pImpl->Remove(this, item); }
    CListItem* GetFirst() { return m_pImpl->GetFirst(); }
};

struct CVtable_012b1c40 : public CTypedListBase {   // vtable 0x012B1C40 (the second item kind's list)
    CListImpl m_impl;                           // +0x8
    virtual void Slot0();
    CVtable_012b1c40(long mode) : CTypedListBase(&m_impl), m_impl(mode) { }
    ~CVtable_012b1c40() { }
    unsigned int GetCount() { return m_pImpl->GetCount(); }
    bool Remove(CListItem* item) { return m_pImpl->Remove(this, item); }
    CListItem* GetFirst() { return m_pImpl->GetFirst(); }
};

struct CItemVisitor {                           // vtable 0x012B260C
    virtual void Visit(CListItem* item) = 0;    // slot 0
    virtual ~CItemVisitor() { }                 // slot 1
};

struct CCollectFreeMatchesA : public CItemVisitor {   // vtable 0x012B2614
    CItemList* m_pList;                         // +4
    CEvent_00c08940* m_key;                     // +8
    CCollectFreeMatchesA(CItemList* list, CEvent_00c08940* key) : m_pList(list), m_key(key) { }
    virtual void Visit(CListItem* item);        // 0x00C10150
};

struct CCollectFreeMatchesB : public CItemVisitor {   // vtable 0x012B261C
    CVtable_012b1c40* m_pList;                  // +4
    CEvent_00c08940* m_key;                     // +8
    CCollectFreeMatchesB(CVtable_012b1c40* list, CEvent_00c08940* key) : m_pList(list), m_key(key) { }
    virtual void Visit(CListItem* item);        // 0x00C10190
};

struct CItemContainer {                         // typed container at +0x10
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2(); virtual void Slot3(); virtual void Slot4();
    virtual void Slot5(); virtual void Slot6(); virtual void Slot7(); virtual void Slot8(); virtual void Slot9();
    virtual void Enumerate(CItemVisitor* visitor);         // slot 10 (+0x28)
};

struct CMemberHandler_00c08940 {
    unsigned char _pad00[0x10];
    CItemContainer m_items;                     // +0x10
    void Handle(CEvent_00c08940* event);
};

void CMemberHandler_00c08940::Handle(CEvent_00c08940* event)
{
    {
        CItemList list(1);
        CCollectFreeMatchesA visitor(&list, event);
        m_items.Enumerate(&visitor);
        list.GetCount();
        CListItem* item = list.GetFirst();
        while (item) {
            list.Remove(item);
            item->SetHandler(this);
            item->Notify(1);
            item = list.GetFirst();
        }
    }
    {
        CVtable_012b1c40 list(1);
        CCollectFreeMatchesB visitor(&list, event);
        m_items.Enumerate(&visitor);
        list.GetCount();
        CListItem* item = list.GetFirst();
        while (item) {
            list.Remove(item);
            item->SetHandler(this);
            item->Notify(1);
            item = list.GetFirst();
        }
    }
}

struct CEvent_00c08940 { int id; };
void CListItem::Slot0() {} void CListItem::Slot1() {} void CListItem::Slot2() {} void CListItem::Slot3() {}
void CListItem::Slot4() {} void CListItem::Slot5() {} void CListItem::Slot6() {}
void CListItem::SetHandler(CMemberHandler_00c08940*) {} void CListItem::Notify(long) {}

void CListImpl::Slot0() {} void CListImpl::Slot1() {} void CListImpl::Slot2() {} void CListImpl::Slot3() {}
void CListImpl::Slot4() {} void CListImpl::Slot7() {} void CListImpl::Slot8() {} void CListImpl::Slot9() {}
CListImpl::CListImpl(long) : m_data(0), m_count(0), m_getCountCalls(0) {}
static int g_implDtors = 0;
CListImpl::~CListImpl() { g_implDtors++; }
void CListImpl::Add(void*, CListItem* item) { m_items[m_count++] = item; }
bool CListImpl::Remove(void*, CListItem* item)
{
    for (int i = 0; i < m_count; i++)
        if (m_items[i] == item) { for (int j = i; j + 1 < m_count; j++) m_items[j] = m_items[j + 1]; m_count--; return true; }
    return false;
}
unsigned int CListImpl::GetCount() { m_getCountCalls++; return m_count; }
CListItem* CListImpl::GetFirst() { return m_count ? m_items[0] : 0; }
void CTypedListBase::Slot0() {}
void CItemList::Slot0() {}
void CVtable_012b1c40::Slot0() {}

struct CTestItem : CListItem {
    CEvent_00c08940* key; int kind; int claimed; CMemberHandler_00c08940* handler; long notified; int order;
    void Slot0() {} void Slot1() {} void Slot2() {} void Slot3() {} void Slot4() {} void Slot5() {} void Slot6() {}
    void SetHandler(CMemberHandler_00c08940* h) { handler = h; }
    void Notify(long v);
};
static int g_seq = 0;
void CTestItem::Notify(long v) { notified = v; order = ++g_seq; }

void CCollectFreeMatchesA::Visit(CListItem* item)
{
    CTestItem* t = (CTestItem*)item;
    if (t->kind == 0 && t->key == m_key && !t->claimed) m_pList->m_impl.Add(m_pList, item);
}
void CCollectFreeMatchesB::Visit(CListItem* item)
{
    CTestItem* t = (CTestItem*)item;
    if (t->kind == 1 && t->key == m_key && !t->claimed) m_pList->m_impl.Add(m_pList, item);
}

static CTestItem g_items[6];
void CItemContainer::Slot0() {} void CItemContainer::Slot1() {} void CItemContainer::Slot2() {} void CItemContainer::Slot3() {}
void CItemContainer::Slot4() {} void CItemContainer::Slot5() {} void CItemContainer::Slot6() {} void CItemContainer::Slot7() {}
void CItemContainer::Slot8() {} void CItemContainer::Slot9() {}
void CItemContainer::Enumerate(CItemVisitor* visitor) { for (int i = 0; i < 6; i++) visitor->Visit(&g_items[i]); }

static int fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed line %d: %s\n", __LINE__, #c); fail++; } } while (0)

int main()
{
    CEvent_00c08940 ev, other;
    int kinds[6] = { 0, 1, 0, 1, 0, 1 };
    for (int i = 0; i < 6; i++) {
        g_items[i].key = &ev; g_items[i].kind = kinds[i]; g_items[i].claimed = 0;
        g_items[i].handler = 0; g_items[i].notified = 0; g_items[i].order = 0;
    }
    g_items[2].claimed = 1;                 // claimed: skipped
    g_items[5].key = &other;                // other event: skipped
    CMemberHandler_00c08940 h;
    h.Handle(&ev);
    CHECK(g_items[0].handler == &h && g_items[0].notified == 1);
    CHECK(g_items[4].handler == &h && g_items[4].notified == 1);
    CHECK(g_items[1].handler == &h && g_items[3].handler == &h);
    CHECK(g_items[2].handler == 0 && g_items[5].handler == 0);
    // kind 0 list drained first, in collection order; then kind 1
    CHECK(g_items[0].order == 1 && g_items[4].order == 2 && g_items[1].order == 3 && g_items[3].order == 4);
    CHECK(g_implDtors == 2);
    if (fail) { std::printf("00c104f0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00c104f0_TEST PASS\n");
    return 0;
}