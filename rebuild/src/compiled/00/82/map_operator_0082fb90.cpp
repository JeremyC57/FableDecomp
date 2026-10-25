// map::operator[](const unsigned __int64& k) -- STLport map<unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation> subscript: find the key's slot, inserting a default-constructed movement when it is missing
// Leaf: ??A?$map@_KVCThingPosAndOrientation@CScriptedMapBrush@@
// Grade: RELOCATION_MATCH -- chunk-422 recipe: insert_unique(hint, v) 0x0082F930 receives the pair temporary's address; a same-TU noinline stand-in that READS v but never keeps its address lets VC7.1 schedule the default value's stores as retail (the parked "pair zero-store scheduling" residue). A stand-in that ignores v lets VC drop the pair stores (101 bytes).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082FB90, 192 bytes (+2 int3), thiscall `ret 4`, 0x50-byte frame.
// Identity: the brush's ThingsToMove map (callers 0x0082EA5B / 0x0082EA7B in SetThingToMove); the catalog label
//   "CThing_CalculateMovementTiming" is an autoname.
// Callees: _Rb_tree::_M_lower_bound(const key&) 0x0082F0D0 (STLport 4.6: the inline lower_bound wraps it, the node in
//   eax), __default_constructed((CThingPosAndOrientation*)0) 0x0082F5C0 (STLport's _STLP_DEFAULT_CONSTRUCTED: link-time
//   __fastcall, the null type pointer in ecx, the value through the hidden pointer), map::insert(iterator, const
//   value_type&) 0x0082F930 (the iterator through the hidden pointer).
// Shape: STLport 4.x verbatim -- `iterator i = lower_bound(k); if (i == end() || key_comp()(k, (*i).first)) i =
//   insert(i, value_type(k, _STLP_DEFAULT_CONSTRUCTED(_Tp))); return (*i).second;` -- the u64 compare high dword first,
//   the 32-byte pair built in the frame (key, the C3DVector's memberwise copy, the angles), the hint iterator copied
//   into its argument slot, the result iterator on the dead `k` slot; `.second` is node + 0x18.
namespace std {
template <class _Tp> _Tp __fastcall __default_constructed(_Tp* __p);   // 0x0082F5C0 (`return _Tp();`)

template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };
template <class A, class B> struct pair {
    typedef A first_type;
    typedef B second_type;
    A first;
    B second;
    pair(const A& a, const B& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& x) const { return x.first; } };
template <class T> class allocator { };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                 // +0x00
    _Base_ptr _M_parent;                                // +0x04
    _Base_ptr _M_left;                                  // +0x08
    _Base_ptr _M_right;                                 // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& o) const { return _M_node == o._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    _Link_type   _M_header;                             // +0x00
    unsigned int _M_node_count;                         // +0x04
    _Compare     _M_key_compare;                        // +0x08
    _Compare key_comp() const { return _M_key_compare; }
    iterator end() { return iterator(_M_header); }
    _Rb_tree_node_base* _M_lower_bound(const _Key& k) const;   // 0x0082F0D0
    iterator lower_bound(const _Key& k) { return iterator(_M_lower_bound(k)); }
    iterator insert_unique(iterator position, const _Value& v);   // 0x0082F930
};
#pragma auto_inline(off)
// Same-TU stand-in for insert_unique(hint, v) 0x0082F930 (reads v, never keeps its address).
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
__declspec(noinline) typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(iterator position, const _Value& v)
{
    _M_node_count += *(const unsigned char*)&v.first + *(const unsigned char*)&v.second;
    return position;
}
#pragma auto_inline(on)


template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    _Compare key_comp() const { return _M_t.key_comp(); }
    iterator end() { return _M_t.end(); }
    iterator lower_bound(const key_type& x) { return _M_t.lower_bound(x); }
    iterator insert(iterator position, const value_type& x) { return _M_t.insert_unique(position, x); }
    _Tp& operator[](const key_type& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, __default_constructed((_Tp*)0)));
        return (*__i).second;
    }
};
}

class C3DVector {                                       // user memberwise copy constructor
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class C3DAngles {                                       // trivially copyable
public:
    float X, Y, Z;
};
class CScriptedMapBrush {
public:
    class CThingPosAndOrientation {                     // Ego_r.pdb, 24 bytes
    public:
        C3DVector Position;                             // +0x00
        C3DAngles FacingAngles;                         // +0x0c
    };
};

template CScriptedMapBrush::CThingPosAndOrientation&
std::map<unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation>::operator[](const unsigned __int64&);