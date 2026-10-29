#include <cstdio>
struct CAction { int id; };
struct CP { CAction* m_pObject; void* m_pCount; };
struct Node { Node* next; Node* prev; CP value; };
struct List { Node* head; unsigned long size; };
static int g_sum = 0, g_calls = 0;
namespace NUISystem { class CManager { public: virtual void ExecuteAction(const CP& a) { ++g_calls; g_sum += a.m_pObject->id; } static CManager* __fastcall GetInstance() { static CManager m; return &m; } }; }
struct CComp { void ExecuteActions(List* l) { if (l) for (Node* n = l->head->next; n != l->head; n = n->next) NUISystem::CManager::GetInstance()->ExecuteAction(n->value); } };
int main()
{
    CAction a = { 1 }, b = { 10 }; Node head, n1, n2; n1.value.m_pObject = &a; n2.value.m_pObject = &b;
    head.next = &n1; n1.next = &n2; n2.next = &head; head.prev = &n2; n1.prev = &head; n2.prev = &n1; List l = { &head, 2 };
    CComp c; c.ExecuteActions(&l);
    if (g_calls != 2 || g_sum != 11) { std::printf("005309b0_TEST FAIL\n"); return 1; }
    c.ExecuteActions(0); head.next = &head; c.ExecuteActions(&l);
    if (g_calls != 2) { std::printf("005309b0_TEST FAIL\n"); return 1; }
    std::printf("005309b0_TEST PASS\n");
    return 0;
}