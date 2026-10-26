// std::__find_if(std::list<CIntelligentPointer<CThingAICreature> >::iterator first, std::list<CIntelligentPointer<CThingAICreature> >::iterator last, HasGuardProperty pred, const std::input_iterator_tag&) -- finds the first creature the out-of-line by-value guard-property predicate accepts
// Leaf: __find_if
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065F9AB, 53 bytes, `ret 0Ch`. Caller: the find_if front end 0x0066000A (passes the tag
// reference in edx: `lea edx,[ebp+13h]`). Callees: CIntelligentPointer copy ctor @ 0x0050F410 (the by-value argument built in its
// slot: `push ecx ; push ecx ; mov ecx,esp`), HasGuardProperty::operator()(CIntelligentPointer<CThingAICreature>) @ 0x0065F152
// (destroys its argument; never reads `this`).
// STLport `__find_if(first, last, pred, const input_iterator_tag&)`; the tag reference is the second `__fastcall` register argument.
// Size TU (`#pragma optimize("s", on)`: unaligned function, `jmp` to the loop test).
#pragma optimize("s", on)
class CThing;
class CThingAICreature;
namespace std {
struct input_iterator_tag {};
}
class CBaseIntelligentPointer {                   // 8 bytes: vptr + item
public:
    virtual void v00();
    CThing* m_pItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer& other);   // 0x0050F410 (out of line)
};
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
template <class InputIter, class Predicate>
InputIter __fastcall __find_if(InputIter first, InputIter last, Predicate pred, const input_iterator_tag&)
{
    while (first != last && !pred(*first))
        ++first;
    return first;
}
}
struct HasGuardProperty {                          // empty functor (its operator() never reads `this`)
public:
    HasGuardProperty() {}
    bool operator()(CIntelligentPointer<CThingAICreature> creature) const;   // 0x0065F152 (out of line)
};
template CListIterator<CIntelligentPointer<CThingAICreature> > __fastcall std::__find_if(CListIterator<CIntelligentPointer<CThingAICreature> >, CListIterator<CIntelligentPointer<CThingAICreature> >, HasGuardProperty, const std::input_iterator_tag&);