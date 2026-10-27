// QuadricOptimiserInternals::COptimisedVertex::InterpolateGroups(const COptimisedVertex& other, float t) -- blend the vertex's bone-group weights towards another vertex's: scale every own weight by (1 - t), then add t times each of the other's weights (creating missing groups at 0)
// Leaf: ?InterpolateGroups@COptimisedVertex@QuadricOptimiserInternals@@QAEXABV12@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC8720, 251 bytes, thiscall `ret 8`.
// Identity: Ego_r.pdb `?InterpolateGroups@COptimisedVertex@QuadricOptimiserInternals@@QAEXABV12@M@Z`; the walked map at
//   +0x9C is COptimisedVertex::Groups (map<unsigned long, float>, Ego_r +0xA0 less the 4 bytes CIndexList FaceList
//   loses as a 12-byte STLport vector), and the inserter it calls is the landed map<unsigned long, float>
//   _Rb_tree::insert_unique(const value_type&) 0x00AD3EF0. The catalog's "QuadricOptimiser_UpdateQuadricCosts" is
//   descriptive.
// Callees: _Rb_tree<unsigned long, pair<const unsigned long, float>, ...>::insert_unique(const value_type&) @ 0x00AD3EF0
//   ({iterator, bool} through the hidden pointer); the iterator increments are STLport's _Rb_global::_M_increment inline.
// Layout: local model (no engine header for COptimisedVertex): Position/Normal/TextureCoord +0/+0xC/+0x18,
//   PositionError (CQuadric) +0x28, Cost +0x80, FaceList +0x84 (12-byte vector), Exists +0x90, UniqueIndex +0x94,
//   OutputIndex +0x98, Groups +0x9C, PairContractions +0xA8.
// Shape: `w = other weight * t; Groups.insert(value_type(key, 0)).first->second += w` -- the weight is a named local
//   computed before the call; the value_type temporary's 0.0f store is hoisted out of the loop only when VC7.1 can see
//   that neither insert_unique nor _M_insert writes the value: the TU carries the landed insert_unique body and a
//   _M_insert stand-in (both noinline, not graded) -- 4 grades.
namespace std {

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _M_decrement(_Base_ptr __x);   // inlined into operator-- 0x00AD1850
    static _Base_ptr _M_increment(_Base_ptr __x)
    {
        if (__x->_M_right != 0) {
            __x = __x->_M_right;
            while (__x->_M_left != 0)
                __x = __x->_M_left;
        }
        else {
            _Base_ptr __y = __x->_M_parent;
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
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment() { _M_node = _Rb_global_inst::_M_increment(_M_node); }
};
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }
    reference operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    pointer operator->() const { return &(operator*()); }
    _Self& operator++() { _M_increment(); return *this; }
    _Self& operator--();
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;
    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08
    iterator begin() { return iterator(_M_header._M_data->_M_left); }
    iterator end() { return iterator(_M_header._M_data); }
    const_iterator begin() const { return const_iterator(_M_header._M_data->_M_left); }
    const_iterator end() const { return const_iterator(_M_header._M_data); }
    typedef _Rb_tree_node_base* _Base_ptr;
    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }
private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);   // 0x00AD3440
public:
    pair<iterator, bool> insert_unique(const value_type& __v);     // 0x00AD3EF0 (same-TU body below)
};

// The TU holds the real bodies of the iterator decrement (0x00AD1850) and insert_unique (0x00AD3EF0, landed); with them
// visible VC7.1 sees that insert_unique never writes its value argument and hoists the temporary's 0.0f store out of
// the loop, as retail does. Both stay out of line (auto_inline off + noinline); neither is graded here.
#pragma auto_inline(off)
template <class _Value, class _Traits> __declspec(noinline)
_Rb_tree_iterator<_Value, _Traits>& _Rb_tree_iterator<_Value, _Traits>::operator--()
{
    _M_node = _Rb_global_inst::_M_decrement(_M_node);
    return *this;
}
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> __declspec(noinline)
pair<typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator, bool>
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(const _Value& __v)
{
    _Link_type __y = this->_M_header._M_data;
    _Link_type __x = _M_root();
    bool __comp = true;
    while (__x != 0) {
        __y = __x;
        __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
        __x = __comp ? _S_left(__x) : _S_right(__x);
    }
    iterator __j = iterator(__y);
    if (__comp)
        if (__j == begin())
            return pair<iterator, bool>(_M_insert(__y, __y, __v), true);
        else
            --__j;
    if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
        return pair<iterator, bool>(_M_insert(__x, __y, __v), true);
    return pair<iterator, bool>(__j, false);
}
// _M_insert 0x00AD3440 (private) as a same-TU stand-in that reads the value and never writes it (its real body
// copies v into the new node); not graded here.
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> __declspec(noinline)
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w)
{
    _M_node_count += (unsigned int)__v.first + (unsigned int)__v.second + (unsigned int)__x + (unsigned int)__w;
    return iterator(__y);
}
#pragma auto_inline(on)

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    const_iterator begin() const { return _M_t.begin(); }
    const_iterator end() const { return _M_t.end(); }
    pair<iterator, bool> insert(const value_type& __x) { return _M_t.insert_unique(__x); }
};

}   // namespace std

class C3DVector { public: float X, Y, Z; };

namespace QuadricOptimiserInternals {

class CContractionReference;

class COptimisedVertex {
public:
    C3DVector     Position;                                 // +0x00
    C3DVector     Normal;                                   // +0x0C
    C3DVector     TextureCoord;                             // +0x18
    unsigned char _pad24[4];                                // +0x24 (CQuadric is 8-aligned)
    unsigned char PositionError[0x58];                      // +0x28 CQuadric
    float         Cost;                                     // +0x80
    unsigned char FaceList[0xC];                            // +0x84 CIndexList (one 12-byte vector)
    bool          Exists;                                   // +0x90
    unsigned long UniqueIndex;                              // +0x94
    unsigned long OutputIndex;                              // +0x98
    std::map<unsigned long, float> Groups;                  // +0x9C
    void InterpolateGroups(const COptimisedVertex& other, float t);
};

void COptimisedVertex::InterpolateGroups(const COptimisedVertex& other, float t)
{
    std::map<unsigned long, float>::iterator it;
    for (it = Groups.begin(); it != Groups.end(); ++it)
        it->second *= 1.0f - t;
    for (std::map<unsigned long, float>::const_iterator o = other.Groups.begin(); o != other.Groups.end(); ++o) {
        float weight = o->second * t;
        Groups.insert(std::map<unsigned long, float>::value_type(o->first, 0.0f)).first->second += weight;
    }
}

}   // namespace QuadricOptimiserInternals