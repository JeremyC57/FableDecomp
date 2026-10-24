// std::count_if(list<CIntelligentPointer<CThingAICreature> >::const_iterator first, last, HasBuildingAsWork pred) -- count the living village people whose workplace is the predicate's building
// Leaf: count_if
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C1C70, length 106 bytes, `ret 0Ch`. Caller: CTCBuyableHouse::
// IsHouseBeingUsed (0x006BFB63, landed: it declares this `long __stdcall` taking first, last and the 4-byte predicate by
// value). The manifest label names the instantiation (Dinkumware-style spelling).
// Layout: Ego_r.pdb HasBuildingAsWork = {CThing* Building}; the STLport list node {_M_next +0, _M_prev +4, _M_data +8};
// the creature's dead bit at +0x91 bit 0 (as the landed CActionEventCameraShake::Dispatch 0x00871250 / CPlayer::
// InitModeBoastUI 0x00633F10) and its work-building intelligent pointer at +0x170 (the out-of-line
// HasBuildingAsHomeOrWork 0x0065FBE1 compares the pointers at +0x168 and +0x170).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x4 per element).
// Shape: STLport count_if with the predicate inlined: the element address stays in esi for the three GetPItem calls,
// Building is loaded once into ebp, the counter is a frame slot (register pressure), the empty-range test is hoisted
// (speed TU: loop head aligned with `lea ecx,[ecx]`). The work building is read through an inline accessor on the
// creature (`lea ecx,[eax+170h]`); chaining `->WorkBuilding.GetPItem()` directly gives `add eax,170h ; mov ecx,eax`.
struct CBaseObject;
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

class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char m_isDead : 1;                              // +0x91 bit 0
};
class CThingAICreature : public CThing {
public:
    unsigned char               _pad92[0x168 - 0x92];
    CIntelligentPointer<CThing> HomeBuilding;                // +0x168
    CIntelligentPointer<CThing> WorkBuilding;                // +0x170
    CThing* GetWorkBuilding() const { return WorkBuilding.GetPItem(); }
};

struct HasBuildingAsWork {
    CThing* Building;                                        // +0x00
    bool operator()(const CIntelligentPointer<CThingAICreature>& creature) const
    {
        return creature.GetPItem() && !creature.GetPItem()->m_isDead &&
               creature.GetPItem()->GetWorkBuilding() == Building;
    }
};

template long __stdcall std::count_if(std::_List_iterator<CIntelligentPointer<CThingAICreature> >,
                                      std::_List_iterator<CIntelligentPointer<CThingAICreature> >, HasBuildingAsWork);