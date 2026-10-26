// std::_Rb_tree::_M_lower_bound(const key_type& k) const -- (map<unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation >) STLport lower-bound walk: from the root keep the last node whose 64-bit key is not below k (going left), else go right; the header when none
// Leaf: ?_M_lower_bound@?$_Rb_tree@_KU?$pair@$$CB_KVCThingPosAndOrientation@CScriptedMapBrush@@@std@@U?$_Select1st@U?$pair@$$CB_KVCThingPosAndOrientation@CScriptedMapBrush@@@std@@@2@U?$less@_K@2@V?$allocator@U?$pair@$$CB_KVCThingPosAndOrientation@CScriptedMapBrush@@@std@@@2@@std@@QBEPAU?$_Rb_tree_node@U?$pair@$$CB_KVCThingPosAndOrientation@CScriptedMapBrush@@@std@@@2@AB_K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082F0D0, 55 bytes (+ int3 padding), thiscall, `ret 4`; returns the node pointer.
// Identity (proven): std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation >, _Select1st<...>, less<unsigned __int64>,
//   allocator<...> >::_M_lower_bound; evidence: its callers are 0x0082F490, 0x0082F790 and map::operator[] 0x0082FB90, which CScriptedMapBrush::SetThingToMove 0x0082EA50 runs (twice); Ego_r.pdb CScriptedMapBrush::ThingsToMove is map<unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation>, the only map of that value.
// Label: the manifest's "TimingTree_FindClosestLTE" is behavioural / propagated from a byte-identical body elsewhere (family J089: the 55-byte
//   64-bit-key lower-bound walks; retail has no identical-code folding, so each address is its own tree type).
// Callees: none (less<unsigned __int64> inline: the high halves compare unsigned first, then the low halves).
// Shape: STLport 4.6 _tree.c `_M_lower_bound` (agent950 F012's _M_find walk without the final test); node: color +0, parent
//   +4, left +8, right +0xC, the 8-aligned value (key first) at +0x10; tree: header node pointer +0 (root = header->parent).
class C3DVector {                                         // 12 bytes (Ego_r.pdb)
public:
    float X;                                               // +0x00
    float Y;                                               // +0x04
    float Z;                                               // +0x08
};
class C3DAngles {                                          // 12 bytes (Ego_r.pdb)
public:
    float AngleXY;                                         // +0x00
    float AngleYZ;                                         // +0x04
    float AngleXZ;                                         // +0x08
};
class CScriptedMapBrush {                                  // partial view: only the nested type is used here
public:
    class CThingPosAndOrientation {                        // 24 bytes (Ego_r.pdb)
    public:
        C3DVector Position;                                // +0x00
        C3DAngles FacingAngles;                            // +0x0C
    };
};
namespace std {
template <class _Tp> class allocator { };
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
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
struct _Rb_tree_node_base {
    bool                _M_color;                          // +0x00
    _Rb_tree_node_base* _M_parent;                         // +0x04
    _Rb_tree_node_base* _M_left;                           // +0x08
    _Rb_tree_node_base* _M_right;                          // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                 // +0x10
};
template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, allocator<_Rb_tree_node<_Tp> > > _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    unsigned int _M_node_count;                            // +0x04
    _Compare _M_key_compare;                               // +0x08
    static const _Key& _S_key(_Link_type __x) { return _KeyOfValue()(__x->_M_value_field); }
    static _Link_type _S_left(_Link_type __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Link_type __x) { return (_Link_type)__x->_M_right; }
    _Link_type _M_root() const { return (_Link_type)this->_M_header._M_data->_M_parent; }
    _Rb_tree_node<_Value>* _M_lower_bound(const _Key& __k) const;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node<_Value>* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_lower_bound(const _Key& __k) const
{
    _Link_type __y = this->_M_header._M_data;              // last node which is not less than __k
    _Link_type __x = _M_root();                            // current node
    while (__x != 0)
        if (!_M_key_compare(_S_key(__x), __k))
            __y = __x, __x = _S_left(__x);
        else
            __x = _S_right(__x);
    return __y;
}
typedef pair<const unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation > TValue_0082f0d0;
template _Rb_tree_node<TValue_0082f0d0 >* _Rb_tree<unsigned __int64, TValue_0082f0d0, _Select1st<TValue_0082f0d0 >, less<unsigned __int64> >::
    _M_lower_bound(const unsigned __int64&) const;
} // namespace std