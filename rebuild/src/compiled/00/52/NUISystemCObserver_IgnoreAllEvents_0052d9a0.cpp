// NUISystem::CObserver::IgnoreAllEvents() -- empty the member tree (+0x4)
// Leaf: ?IgnoreAllEvents@CObserver@NUISystem@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052D9A0, 52 bytes, thiscall, plain `ret`.
// Identity (probable): slot 5 of NUISystem::CObserver's vtable 0x01246020 (RTTI .?AVCObserver@NUISystem@@; inherited unchanged by CSprite, CYesNo, COk, CControllerDisconnect, CChangingStateComponent), between ObserveEvent (slot 3) / IgnoreEvent and ObserveAllEvents (slot 6); Ego_r.pdb ?IgnoreAllEvents@CObserver@NUISystem@@UAEXXZ is the method that empties EventsToObserve (set<EEvent>, +4).
// Callees: the member tree's _Rb_tree::_M_erase(_Rb_tree_node*) 0x0052DCA0.
// Shape (family M119, representative 0x0052D9A0): STLport 4.6 map/set `clear()` inline -- `if (_M_node_count != 0) {
//   _M_erase(_M_root()); _M_leftmost() = _M_header; _M_root() = 0; _M_rightmost() = _M_header; _M_node_count = 0; }`.
// Header debt: the owning class is modelled locally (no shared header declares its methods / the container member).
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Pair> struct _Select1st { };
template <class _Tp> struct _Identity { };
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> > class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Base_ptr& _M_root() const { return _M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return _M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return _M_header._M_data->_M_right; }
    void _M_erase(_Link_type __x);
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = _M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = _M_header._M_data;
            _M_node_count = 0;
        }
    }
    _STLP_alloc_proxy<_Base_ptr, allocator<_Rb_tree_node<_Value> > > _M_header;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map {
public:
    void clear() { _M_t.clear(); }
    _Rb_tree<_Key, pair<const _Key, _Tp>, _Select1st<pair<const _Key, _Tp> >, _Compare, _Alloc> _M_t;
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    void clear() { _M_t.clear(); }
    _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _M_t;
};
}
namespace NUISystem {
enum EEvent { EVENT_NONE = 0 };
class CObserver {                                         // Ego_r.pdb; vtable 0x01246020
public:
    virtual ~CObserver();
    virtual void IgnoreAllEvents();
    std::set<EEvent> EventsToObserve;                     // +0x04
    bool PreventObservation;                              // +0x10
};
void CObserver::IgnoreAllEvents()
{
    EventsToObserve.clear();
}
}