#include <cstdio>
#include <stddef.h>
struct CAction { int id; };
struct CCountedActionPointer { CAction* m_pObject; void* m_pCount; };
template<class T> struct CRetailList { struct node { node* next; node* prev; T value; }; node* head;
    struct iterator { node* p; explicit iterator(node* q) : p(q) { } T& operator*() const { return p->value; } iterator& operator++() { p = p->next; return *this; } bool operator!=(const iterator& o) const { return p != o.p; } };
    iterator begin() { return iterator(head->next); } iterator end() { return iterator(head); } };
static int g_executed = 0, g_instances = 0; static const CCountedActionPointer* g_last = 0;
namespace NUISystem { class CManager { public:
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void v05() { } virtual void v06() { } virtual void v07() { } virtual void v08() { } virtual void v09() { } virtual void v10() { } virtual void v11() { } virtual void v12() { } virtual void v13() { }
    virtual void ExecuteAction(const CCountedActionPointer& action) { ++g_executed; g_last = &action; }
    static CManager* __fastcall GetInstance() { static CManager instance; ++g_instances; return &instance; }
}; }
struct CComponent { virtual void v00() { }
    void ExecuteActions(CRetailList<CCountedActionPointer>* actions)
    { if (actions != 0) { for (CRetailList<CCountedActionPointer>::iterator it = actions->begin(); it != actions->end(); ++it) NUISystem::CManager::GetInstance()->ExecuteAction(*it); } } };
int main()
{
    CRetailList<CCountedActionPointer>::node sentinel, n0, n1; sentinel.next = &n0; n0.next = &n1; n1.next = &sentinel; CAction x = { 1 }, y = { 2 }; n0.value.m_pObject = &x; n1.value.m_pObject = &y;
    CRetailList<CCountedActionPointer> list; list.head = &sentinel; CComponent c;
    c.ExecuteActions(&list);
    if (g_executed != 2 || g_instances != 2 || g_last != &n1.value || offsetof(CRetailList<CCountedActionPointer>::node, value) != 8) { std::printf("00536b22_TEST FAIL\n"); return 1; }
    c.ExecuteActions(0); sentinel.next = &sentinel; c.ExecuteActions(&list);
    if (g_executed != 2) { std::printf("00536b22_TEST FAIL\n"); return 1; }
    std::printf("00536b22_TEST PASS\n"); return 0;
}