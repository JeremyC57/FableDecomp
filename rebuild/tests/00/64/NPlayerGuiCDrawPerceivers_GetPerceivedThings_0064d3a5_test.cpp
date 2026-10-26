#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>

class CThing { public: int x; };
struct CBaseIntelligentPointer { void* item; void* info; const CThing* GetPItem() { return (const CThing*)item; } };
struct CRetailThingPointerList { struct node { node* next; node* prev; CBaseIntelligentPointer value; }; struct iterator { node* m_pNode; explicit iterator(node* n) : m_pNode(n) {} CBaseIntelligentPointer& operator*() const { return m_pNode->value; } iterator& operator++() { m_pNode = m_pNode->next; return *this; } bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; } };
    node* head; CRetailThingPointerList() { head = new node; head->next = head->prev = head; } iterator begin() { return iterator(head->next); } iterator end() { return iterator(head); }
    void add(CThing* t) { node* n = new node; n->value.item = t; n->prev = head->prev; n->next = head; head->prev->next = n; head->prev = n; } };
struct CRetailThingVector { const CThing* buf[8]; const CThing** f; const CThing** l; const CThing** e; CRetailThingVector() { f = l = buf; e = buf + 8; } void push_back(const CThing* const& v) { *l++ = v; } };
struct CDrawPerceivers { unsigned char p[0x3c]; CRetailThingPointerList Things;
    void GetPerceivedThings(CRetailThingVector* things) { for (CRetailThingPointerList::iterator it = Things.begin(); it != Things.end(); ++it) { const CThing* thing = (*it).GetPItem(); things->push_back(thing); } } };
int main() { CDrawPerceivers d; CThing a, b; d.Things.add(&a); d.Things.add(&b); CRetailThingVector v; d.GetPerceivedThings(&v);
    if (v.l - v.f != 2 || v.buf[0] != &a || v.buf[1] != &b) { std::printf("0064d3a5_TEST FAIL\n"); return 1; }
    std::printf("0064d3a5_TEST PASS\n"); return 0; }