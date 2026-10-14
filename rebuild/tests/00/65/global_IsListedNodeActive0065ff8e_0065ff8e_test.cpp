#include <cstdio>
struct CActiveObject { bool m_active; virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual bool IsActive() { return m_active; } };
struct CListNode { CListNode* next; CListNode* prev; CActiveObject* m_pObject; };
struct CListIterator { CListNode* p; CListIterator(CListNode* node) : p(node) { } CListIterator(const CListIterator& other) : p(other.p) { } };
struct CNodeList { CListNode* head; CListIterator begin() const { return CListIterator(head->next); } CListIterator end() const { return CListIterator(head); } };
struct CNodeRef { CListNode* p; CNodeRef& Find(CListIterator first, CListIterator last, CListNode* value); };
CNodeRef& CNodeRef::Find(CListIterator first, CListIterator last, CListNode* value)
{
    CListNode* n = first.p; while (n != last.p && n != value) n = n->next; p = n; return *this;
}
bool __stdcall IsListedNodeActive_0065ff8e(CNodeRef ref, CNodeList* list)
{
    ref.Find(list->begin(), list->end(), ref.p);
    if (ref.p == list->head) return false;
    return ref.p->m_pObject->IsActive();
}
int main()
{
    CActiveObject a, b; a.m_active = true; b.m_active = false;
    CListNode head, n1, n2, stray; head.next = &n1; n1.next = &n2; n2.next = &head; n1.m_pObject = &a; n2.m_pObject = &b; head.prev = &n2; n1.prev = &head; n2.prev = &n1;
    CNodeList list; list.head = &head; CNodeRef r1 = { &n1 }, r2 = { &n2 }, r3 = { &stray };
    if (!IsListedNodeActive_0065ff8e(r1, &list) || IsListedNodeActive_0065ff8e(r2, &list) || IsListedNodeActive_0065ff8e(r3, &list)) { std::printf("0065ff8e_TEST FAIL\n"); return 1; }
    std::printf("0065ff8e_TEST PASS\n");
    return 0;
}