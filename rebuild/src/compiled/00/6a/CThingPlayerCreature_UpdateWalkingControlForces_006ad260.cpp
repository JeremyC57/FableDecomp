// CThingPlayerCreature::UpdateWalkingControlForces() -- average every queued relative-movement acceleration component (each summed vector scaled by 1/its count) into RelativeMovementAcceleration, empty the component map, then resolve the movement band, the relative and absolute movement accelerations and the facing direction
// Leaf: ?UpdateWalkingControlForces@CThingPlayerCreature@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AD260, 338 bytes, thiscall, frame 0x18; ends `jmp ResolveFacingDirection` (0x006AB820).
// Identity: Ego_r.pdb ?UpdateWalkingControlForces@CThingPlayerCreature@@IAEXXZ (protected). Two sources: (1) Ego_r donor order puts
//   it directly after ?ResolveMovementBandType@CThingPlayerCreature@@IAEXXZ (0x005495D0 -> 0x00549640), and retail has the landed
//   ResolveMovementBandType at 0x006AD220 directly before this row, with its body expanded here; (2) the body is the walking
//   control update over Ego_r's RelativeMovementAccelerationComponents (map<EGameAction, pair<C3DVector,long> >, retail +0x1A8)
//   and RelativeMovementAcceleration (+0x1B4), calling the PDB's ResolveRelativeMovementAcceleration 0x006A9690 /
//   ResolveMovementAcceleration 0x006AB770 / ResolveFacingDirection 0x006AB820 (all IAEXXZ). The sole caller is ControlledMove
//   0x006AD700 (first call). The other PDB candidate UpdateMovementForces is a 16-byte Ego_r body. The catalog label
//   CThingPlayerCreature_ComputeAcceleration is a descriptive guess.
// Callees: the map's _Rb_tree::_M_erase 0x00527910 (out of line, stl/_tree.c; the bound spelling already names this
//   instantiation), GetMovementBandTypeFromJoytickMagnitude 0x006AB430 (inside the inlined ResolveMovementBandType), the three
//   Resolve* members above.
// Shape: C3DVector arithmetic inline (operator* by a 1/count float, operator+=); the loop's end() is held in a register (no
//   calls in the loop); STLport map::clear -> _Rb_tree::clear (count test, _M_erase, header reset) inline. ResolveMovementBandType
//   is defined in the TU (retail emits it at 0x006AD220, the landed row's text) and VC7.1 expands it. First probe.
// Layout: no engine header for CThingPlayerCreature; Ego_r members at retail offsets (Ego_r +0x1D0 / +0x1E0 / +0x22C less 0x28
//   / 0x2C for the STLport containers before them). STLport 4.6 text (stl/_tree.h, _tree.c, _map.h) reduced to what is used.
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
// ---- tree (stl/_tree.h, stl/_tree.c)
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* __x);
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    }
    else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    void _M_increment() { _M_node = _Rb_global_inst::_M_increment(_M_node); }
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp* pointer;
    typedef _Tp& reference;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    pointer operator->() const { return &(operator*()); }
    _Self& operator++() { _M_increment(); return *this; }
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Node, allocator<_Node> > _M_header;
    ~_Rb_tree_base() { _M_header.deallocate((_Node*)_M_header._M_data, 1); }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    _Base_ptr& _M_root() const { return (_Base_ptr&)this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return (_Base_ptr&)this->_M_header._M_data->_M_right; }
    iterator begin() { return iterator((_Link_type)_M_leftmost()); }
    iterator end() { return iterator((_Link_type)this->_M_header._M_data); }
private:
    void _M_erase(_Link_type __x);                                       // stl/_tree.c (out of line: 0x006C4F80)
public:
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    void clear() { _M_t.clear(); }
};
} // namespace std

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    float GetAccurateMagnitude() const { float x = X, y = Y, z = Z; return (float)sqrt(x * x + y * y + z * z); }
};

enum EGameAction { };
enum EMovementBandType { };

class CThingPlayerCreature {
public:
    unsigned char _pad_0x00[0x1A8];
    std::map<EGameAction, std::pair<C3DVector, long> > RelativeMovementAccelerationComponents;   // +0x1A8
    C3DVector RelativeMovementAcceleration;                                                      // +0x1B4
    unsigned char _pad_0x1c0[0x200 - 0x1C0];
    EMovementBandType MovementBandType;                                                          // +0x200
protected:
    EMovementBandType GetMovementBandTypeFromJoytickMagnitude(float magnitude) const;            // 0x006AB430
    void ResolveMovementBandType();                                                              // 0x006AD220
    void ResolveRelativeMovementAcceleration();                                                  // 0x006A9690
    void ResolveMovementAcceleration();                                                          // 0x006AB770
    void ResolveFacingDirection();                                                               // 0x006AB820
    void UpdateWalkingControlForces();
};

void CThingPlayerCreature::ResolveMovementBandType()
{
    MovementBandType = GetMovementBandTypeFromJoytickMagnitude(RelativeMovementAcceleration.GetAccurateMagnitude());
}

void CThingPlayerCreature::UpdateWalkingControlForces()
{
    C3DVector acceleration(0.0f, 0.0f, 0.0f);
    for (std::map<EGameAction, std::pair<C3DVector, long> >::iterator it = RelativeMovementAccelerationComponents.begin();
         it != RelativeMovementAccelerationComponents.end(); ++it)
        acceleration += it->second.first * (1.0f / (float)it->second.second);
    RelativeMovementAcceleration = acceleration;
    RelativeMovementAccelerationComponents.clear();
    ResolveMovementBandType();
    ResolveRelativeMovementAcceleration();
    ResolveMovementAcceleration();
    ResolveFacingDirection();
}