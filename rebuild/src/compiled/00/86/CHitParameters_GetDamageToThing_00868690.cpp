// CHitParameters::GetDamageToThing(const CThingGameObject&) const -- the hit's health change: zero minus every damage amount in the damage map (the thing is not consulted)
// Leaf: ?GetDamageToThing@CHitParameters@@QBEMABVCThingGameObject@@@Z
// Retail 0x00868690, 0x4E bytes, thiscall `ret 4` (the thing argument unused), float in st0. FableWin/Ego_r name:
// ?GetDamageToThing@CHitParameters@@QBEMABVCThingGameObject@@@Z. Callers 0x006AB2D5, 0x00734D91 (CTCCombat::SetStruck
// family), 0x0083A07A -- each passes the struck thing and feeds the result to a health change.
// Layout (Ego_r, no shift before it): CHitParameters DamageMap +0x30 = STLport map<EDamageAttribute, float> {_M_header
// node pointer +0, _M_node_count +4, empty _M_key_compare +8}; node {color, parent +4, left +8, right +0x0C, value +0x10:
// key +0x10, float +0x14}. Constant 0.0f @ 0x0122DEDC.
// Shape: `float damage = 0.0f; for (it = begin; it != end; ++it) damage -= it->second; return damage;` with STLport's
// _Rb_global<bool>::_M_increment inlined on the iterator's node (right subtree's leftmost, else climb while a right
// child).
// Manifest label "Health_ComputeWithEffects" is a descriptive auto label.
class CThingGameObject;

enum EDamageAttribute { DAMAGE_ATTRIBUTE_NONE = 0 };

namespace _STL {
template <class _T1, class _T2> struct pair
{
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { };

struct _Rb_tree_node_base
{
    bool                _M_color;                    // +0x00
    _Rb_tree_node_base* _M_parent;                   // +0x04
    _Rb_tree_node_base* _M_left;                     // +0x08
    _Rb_tree_node_base* _M_right;                    // +0x0C
};
template <class _Value> struct _Rb_tree_node : _Rb_tree_node_base
{
    _Value _M_value_field;                           // +0x10
};
template <class _Dummy> struct _Rb_global
{
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* __x)
    {
        if (__x->_M_right != 0) {
            __x = __x->_M_right;
            while (__x->_M_left != 0)
                __x = __x->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = __x->_M_parent;
            while (__x == __y->_M_right) {
                __x = __y;
                __y = __y->_M_parent;
            }
            if (__x->_M_right != __y)
                __x = __y;
        }
        return __x;
    }
};
template <class _Value> struct _Rb_tree_iterator
{
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    const _Value* operator->() const { return &((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __x) const { return _M_node != __x._M_node; }
};
template <class _Key, class _Tp, class _Compare> class map
{
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;                   // +0x00
    unsigned long       _M_node_count;               // +0x04
    _Compare            _M_key_compare;              // +0x08
    const_iterator begin() const { return const_iterator(_M_header->_M_left); }
    const_iterator end() const { return const_iterator(_M_header); }
};
}

class CHitParameters
{
public:
    unsigned char _pad0[0x30];                       // Localised .. ShotBuildupTime
    _STL::map<EDamageAttribute, float, _STL::less<EDamageAttribute> > DamageMap;   // +0x30

    float GetDamageToThing(const CThingGameObject& thing) const;
};

float CHitParameters::GetDamageToThing(const CThingGameObject& thing) const
{
    float damage = 0.0f;
    for (_STL::map<EDamageAttribute, float, _STL::less<EDamageAttribute> >::const_iterator it = DamageMap.begin();
         it != DamageMap.end(); ++it)
        damage -= it->second;
    return damage;
}