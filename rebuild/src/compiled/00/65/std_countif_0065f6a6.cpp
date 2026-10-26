// std::count_if(list<CIntelligentPointer<CThingAICreature> >::iterator first, last, HasGuardProperty pred) -- count the village people that are guards (the predicate takes each as a CIntelligentPointer<const CThing> by value)
// Leaf: count_if
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065F6A6, length 52 bytes, `ret 0Ch`. Caller: CTCVillage::
// CalculateSecuritySweepTasks (0x00659823). The manifest label names the instantiation (Dinkumware-style spelling).
// Callees: the converting constructor CIntelligentPointer<const CThing>::CIntelligentPointer(const CIntelligentPointer
// <CThingAICreature>&) @ 0x0050F410 (out-of-line template member), HasGuardProperty::operator()(CIntelligentPointer<const
// CThing>) const @ 0x0065F152 (`ret 8`: it owns and destroys its by-value argument).
// Shape: STLport count_if (__stdcall, as its siblings 0x006600EC / 0x006C1C70); the element converts implicitly to the
// predicate's by-value parameter, which is built in place in the reserved argument slot (`push ecx ; push ecx ;
// mov ecx,esp`). Opt: an unaligned size-optimised TU (`jmp` to the bottom loop test) -- `#pragma optimize("s", on)`.
#pragma optimize("s", on)
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual void v000();
    CBaseObject* PItem;                                      // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    template <class U> CIntelligentPointer(const CIntelligentPointer<U>& other);   // 0x0050F410 for T = const CThing
};

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                                // +0x00
    _List_node_base* _M_prev;                                // +0x04
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;                                               // +0x08
};
template <class T> struct _List_iterator {                   // the 4-byte STLport node iterator
    _List_node_base* _M_node;
    T& operator*() const { return static_cast<_List_node<T>*>(_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};
template <class _InputIter, class _Predicate>
long __stdcall count_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    long __n = 0;
    for ( ; __first != __last; ++__first)
        if (__pred(*__first))
            ++__n;
    return __n;
}
}

class CThing;
class CThingAICreature;

struct HasGuardProperty {
    bool operator()(CIntelligentPointer<const CThing> thing) const;   // 0x0065F152
};

template long __stdcall std::count_if(std::_List_iterator<CIntelligentPointer<CThingAICreature> >,
                                      std::_List_iterator<CIntelligentPointer<CThingAICreature> >, HasGuardProperty);