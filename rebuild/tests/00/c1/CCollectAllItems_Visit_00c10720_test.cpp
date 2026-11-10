// Behaviour test for CCollectAllItems::Visit @ 0x00C10720: every visited item is handed to the list's Add, in visit order.
#include <cstdio>
struct CListItem { int id; };

struct CItemList {
    CListItem* added[8]; int count;
    CItemList() : count(0) { }
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { }
    virtual void v05() { } virtual void v06() { } virtual void v07() { } virtual void v08() { }
    virtual void Add(CListItem* item) { added[count++] = item; }   // slot 9
};

struct CItemVisitor {
    virtual void Visit(CListItem* item) = 0;
    virtual ~CItemVisitor() { }
};

struct CCollectAllItems : public CItemVisitor {
    CItemList* m_pList;
    CCollectAllItems(CItemList* list) : m_pList(list) { }
    virtual void Visit(CListItem* item);
};

void CCollectAllItems::Visit(CListItem* item)
{
    m_pList->Add(item);
}

int main()
{
    CItemList list;
    CCollectAllItems visitor(&list);
    CListItem a = { 1 }, b = { 2 }, c = { 3 };
    CItemVisitor* v = &visitor;
    v->Visit(&a); v->Visit(&b); v->Visit(&c);
    if (list.count != 3 || list.added[0] != &a || list.added[1] != &b || list.added[2] != &c) {
        std::printf("00c10720_TEST FAIL\n");
        return 1;
    }
    std::printf("00c10720_TEST PASS\n");
    return 0;
}