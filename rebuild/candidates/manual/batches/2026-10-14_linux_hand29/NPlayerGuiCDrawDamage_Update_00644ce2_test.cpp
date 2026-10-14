#include <cstdio>
namespace NPlayerGui {
struct CRetailDamageList { struct node { node* next; node* prev; long m_countdown; float m_value; unsigned char m_flag; };
    struct iterator { node* m_pNode; iterator(node* n) : m_pNode(n) { } iterator(const iterator& o) : m_pNode(o.m_pNode) { } };
    node* head; int erased;
    iterator erase(iterator w) { node* n = w.m_pNode; n->prev->next = n->next; n->next->prev = n->prev; ++erased; return iterator(n->next); } };
struct CDrawDamage { unsigned char _pad[0x28]; CRetailDamageList DamageValues;
    void Update() { CRetailDamageList::node* n = DamageValues.head->next; while (n != DamageValues.head) { if (--n->m_countdown < 0) n = DamageValues.erase(CRetailDamageList::iterator(n)).m_pNode; else n = n->next; } } }; }
int main() { using namespace NPlayerGui; CRetailDamageList::node s, a, b, c; s.next = &a; a.prev = &s; a.next = &b; b.prev = &a; b.next = &c; c.prev = &b; c.next = &s; s.prev = &c;
    a.m_countdown = 0; b.m_countdown = 3; c.m_countdown = 0; CDrawDamage d; d.DamageValues.head = &s; d.DamageValues.erased = 0; d.Update();
    if (d.DamageValues.erased != 2 || s.next != &b || b.next != &s || b.m_countdown != 2) { std::printf("00644ce2_TEST FAIL\n"); return 1; } std::printf("00644ce2_TEST PASS\n"); return 0; }
