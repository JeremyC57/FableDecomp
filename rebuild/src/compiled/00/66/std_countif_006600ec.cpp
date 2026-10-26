// std::count_if(list<CIntelligentPointer<CThingAICreature> >::const_iterator first, last, HasBuildingAsHomeOrWork pred) -- count the village people whose home or workplace is the predicate's building
// Leaf: count_if
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006600EC, length 43 bytes, `ret 0Ch`. Callers: CTCVillage::UpdateCurfew
// (0x0065AE13), CTCBuyableHouse::IsHouseBeingUsed (0x006BFB6A, landed: it declares this `long __stdcall` taking first, last
// and the 4-byte predicate by value), 0x006C144B. The manifest label names the instantiation (Dinkumware-style spelling).
// Layout: Ego_r.pdb HasBuildingAsHomeOrWork = {CThing* Building}; the STLport list node is {_M_next +0, _M_prev +4,
// _M_data +8}, the iterator its node pointer.
// Callees: HasBuildingAsHomeOrWork::operator()(const CIntelligentPointer<CThingAICreature>&) const @ 0x0065FBE1 (out of line).
// Shape: STLport count_if; the counter lives in edi, the predicate is called on its own argument slot
// (`lea ecx,[esp+18h]`), `last` is compared from its slot. Opt: an unaligned, size-optimised TU -- the loop is entered
// through a `jmp` to the bottom test (`#pragma optimize("s", on)`).
#pragma optimize("s", on)
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual void v000();
    CBaseObject* PItem;                                      // +0x04
    CBaseObject* GetPItem() const;                           // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
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

class HasBuildingAsHomeOrWork { public:
    CThing* Building;                                        // +0x00
    bool operator()(const CIntelligentPointer<CThingAICreature>& creature) const;   // 0x0065FBE1
};

template long __stdcall std::count_if(std::_List_iterator<CIntelligentPointer<CThingAICreature> >,
                                      std::_List_iterator<CIntelligentPointer<CThingAICreature> >, HasBuildingAsHomeOrWork);