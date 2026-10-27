// CHitParameters::ClearAllDamage() -- empty the member tree (+0x30)
// Leaf: ?ClearAllDamage@CHitParameters@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00868A40, 52 bytes, thiscall, plain `ret`.
// Identity (proven): its tree's _M_erase is 0x00450903, bound ?_M_erase@?$map@W4EDamageAttribute@@M...; it sits among CHitParameters' damage methods (HasDamageAttribute 0x008688C0, AddDamageWithAttribute 0x00868CA0) and CTCCombat::SetStruck(..., CHitParameters&) 0x00734C60 calls it on its hit parameters; Ego_r.pdb CHitParameters::DamageMap is map<EDamageAttribute, float> at +0x30 and ?ClearAllDamage@CHitParameters@@QAEXXZ empties it.
// Callees: the member tree's _Rb_tree::_M_erase(_Rb_tree_node*) 0x00450903.
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
enum EDamageAttribute { DAMAGE_ATTRIBUTE_NONE = 0 };
class CHitParameters {                                    // Ego_r.pdb (retail offsets equal up to +0x30)
public:
    void ClearAllDamage();
    unsigned char _pad00[0x30];                           // Localised .. ShotBuildupTime
    std::map<EDamageAttribute, float> DamageMap;          // +0x30
};
void CHitParameters::ClearAllDamage()
{
    DamageMap.clear();
}