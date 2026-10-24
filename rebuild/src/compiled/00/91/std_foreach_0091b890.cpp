// std::for_each(std::list<CTCPerceiveThing*>::const_iterator first, std::list<CTCPerceiveThing*>::const_iterator last, std::mem_fun_t<void, CTCPerceiveThing> f) -- calls CTCPerceiveThing::SetAsIndirectlyAwareOfPerceivedThing on every pointer of a list
// Leaf: for_each
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091B890, 60 bytes, `ret 0Ch`. Caller: CAIStateGroup_SeekPerceivedThing::Update 0x0091B6B7
// (pushes the member pointer 0x007449E0). The catalog label `_Find_if<..., CIsIntelligentPointerToThing>` is propagated.
// SGI/STLport `for_each` over a VC6-era Dinkumware list (node {next, prev, value}); the iterator is a one-pointer class with a user copy ctor, so `__fastcall` passes it on the stack and only the hidden functor pointer is in ecx.
// Shape: `mov ecx,[esi+8] ; call ebx` (node value is the pointer), `mov esi,[esi]` (++it).
template <class T> struct SListNode {             // VC6-era Dinkumware list node
    SListNode* next;                              // +0x00
    SListNode* prev;                              // +0x04
    T value;                                      // +0x08
};
template <class T> class CListIterator {          // list<T>::const_iterator: one node pointer with a user copy ctor
public:
    SListNode<T>* m_node;
    CListIterator(SListNode<T>* node) : m_node(node) {}
    CListIterator(const CListIterator& o) : m_node(o.m_node) {}
    T& operator*() const { return m_node->value; }
    CListIterator& operator++() { m_node = m_node->next; return *this; }
    bool operator!=(const CListIterator& o) const { return m_node != o.m_node; }
};
namespace std {
template <class Result, class T> class mem_fun_t {
    Result (T::*m_pf)();
public:
    explicit mem_fun_t(Result (T::*pf)()) : m_pf(pf) {}
    Result operator()(T* p) const { return (p->*m_pf)(); }
};
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
class CTCPerceiveThing {
public:
    void SetAsIndirectlyAwareOfPerceivedThing();  // 0x007449E0 (the bound member pointer)
};
template std::mem_fun_t<void, CTCPerceiveThing> __fastcall std::for_each(CListIterator<CTCPerceiveThing*>, CListIterator<CTCPerceiveThing*>, std::mem_fun_t<void, CTCPerceiveThing>);