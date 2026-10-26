// std::find_if(first, last, CIsHeldThing pred) -- STLport linear search of a list of intelligent thing pointers for the node whose resolved object is the predicate's thing
// Leaf: find_if
// Retail: Fable.exe @ 0x0076D810, 64 bytes, `ret 0Ch`: __fastcall with the hidden iterator result in ecx, first / last /
// a 4-byte predicate on the stack; edx is neither read here nor set by the only call site 0x0076CD0D in
// CTCEnemy::IsEnemyOf(CThing*) (0x0076CCE0), which pushes the thing pointer as the third stack word.
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50.
// Shape: the compared thing sits in the THIRD STACK SLOT while edx stays unused, so it is not the `const T&` value of
// __find (that would arrive in edx, as in the twin 0x0069CAF0): it is a by-value class argument -- a one-pointer
// predicate -- and there is no tag argument at all (an out-of-line __find_if would take the tag reference in edx, and
// a probe caller then sets edx with `lea`): STLport find_if() with the input-iterator __find_if inlined into it, like
// the find() at 0x006C4FC0 (`ret 8`, no tag slot). The predicate word is loaded once after the
// empty-range test (`mov ebp,[esp+1Ch]`) and compared with each node's resolved object (`cmp eax,ebp`).
// List node: _M_next +0, _M_prev +4, data (the intelligent pointer) +8.
class CBaseObject;
class CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                       // +0x04
    CBaseObject* GetPItem() const;              // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
};
typedef CIntelligentPointer<const CThing> CThingPointer;

// "does this intelligent pointer currently resolve to my thing?"
struct CIsHeldThing {
    const CBaseObject* m_pThing;                // +0x00
    bool operator()(const CThingPointer& p) const { return p.GetPItem() == m_pThing; }
};

namespace std {
template <class T> struct _Nonconst_traits {};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node == y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node != y._M_node; }
};
template <class T, class Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) {}
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) {}
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
};
template <class InputIter, class Predicate>
InputIter __fastcall find_if(InputIter first, InputIter last, Predicate pred)
{
    while (first != last && !pred(*first))
        ++first;
    return first;
}
typedef _List_iterator<CThingPointer, _Nonconst_traits<CThingPointer> > _ThingListIter;
template _ThingListIter __fastcall find_if(_ThingListIter, _ThingListIter, CIsHeldThing);
}