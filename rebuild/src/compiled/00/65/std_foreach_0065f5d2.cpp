// std::for_each(std::list<CIntelligentPointer<CThingBuilding> >::iterator first, std::list<CIntelligentPointer<CThingBuilding> >::iterator last, CNearestBuildingWithProperty f) -- feeds every building of a list to the nearest-building functor and returns a copy of it
// Leaf: for_each
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065F5D2, 50 bytes, `ret 20h`. Caller: 0x00658E66 (AI find-nearest-building). Callees:
// CNearestBuildingWithProperty::operator()(CIntelligentPointer<CThingBuilding>*) @ 0x0065F056 and its copy ctor @ 0x0065D750 (manifest).
// SGI/STLport `for_each` over a VC6-era Dinkumware list (node {next, prev, value}); the iterator is a one-pointer class with a user copy ctor, so `__fastcall` passes it on the stack and only the hidden functor pointer is in ecx.
// Size TU (`#pragma optimize("s", on)`, ebp frame chosen by VC, `jmp` to the loop test); the result is copy-constructed
// out of line from the argument slot.
#pragma optimize("s", on)
class CThing;
class CThingBuilding;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
template <class T> struct SListNode {             // VC6-era Dinkumware list node
    SListNode* next;                              // +0x00
    SListNode* prev;                              // +0x04
    T value;                                      // +0x08
};
template <class T> class CListIterator {          // list<T>::iterator: one node pointer with a user copy ctor
public:
    SListNode<T>* m_node;
    CListIterator(SListNode<T>* node) : m_node(node) {}
    CListIterator(const CListIterator& o) : m_node(o.m_node) {}
    T& operator*() const { return m_node->value; }
    CListIterator& operator++() { m_node = m_node->next; return *this; }
    bool operator!=(const CListIterator& o) const { return m_node != o.m_node; }
};
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
class CNearestBuildingWithProperty {             // 0x18 bytes
public:
    long m_words[6];
    CNearestBuildingWithProperty(const CNearestBuildingWithProperty& other);   // 0x0065D750 (out of line)
    void operator()(const CIntelligentPointer<CThingBuilding>& building);       // 0x0065F056 (out of line)
};
template CNearestBuildingWithProperty __fastcall std::for_each(CListIterator<CIntelligentPointer<CThingBuilding> >, CListIterator<CIntelligentPointer<CThingBuilding> >, CNearestBuildingWithProperty);