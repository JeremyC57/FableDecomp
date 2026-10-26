#include <cstdio>
#include <cstdlib>
static long g_owner = 0;
struct CRetailList_node0xc { struct node { node* next; node* prev; long value; }; node* head;
    CRetailList_node0xc() : head(0) { node* s = (node*)std::malloc(sizeof(node)); s->next = s; s->prev = s; head = s; } };
namespace NUISystem {
struct IComponentA { virtual void va00() { } unsigned char _state[0x10]; }; struct IComponentB { virtual void vb00() { } }; struct CComponentRoot { virtual void v00() { } };
class CComponent : public CComponentRoot, public IComponentA, public IComponentB { public: unsigned char _body[0x13c - 0x1c]; CComponent(long owner) { g_owner = owner; } };
class CChangingStateComponent : public CComponent { public: CRetailList_node0xc m_stateList; CChangingStateComponent(long owner) : CComponent(owner), m_stateList() { } }; }
int main() { NUISystem::CChangingStateComponent c(42); if (g_owner != 42 || c.m_stateList.head == 0 || c.m_stateList.head->next != c.m_stateList.head || c.m_stateList.head->prev != c.m_stateList.head) { std::printf("0052cc50_TEST FAIL\n"); return 1; } std::printf("0052cc50_TEST PASS\n"); return 0; }