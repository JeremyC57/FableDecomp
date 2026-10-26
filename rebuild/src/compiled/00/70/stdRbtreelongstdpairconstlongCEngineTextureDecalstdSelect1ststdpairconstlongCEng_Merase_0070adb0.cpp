// std::_Rb_tree<long, pair<const long, CEngineTextureDecal>, _Select1st<..>, less<long>, allocator<..> >::_M_erase(_Link_type x) -- STLport erase without rebalancing for CTCTextureDecal's map<long, CEngineTextureDecal>: recurse right, destroy the decal (its blend vector frees its buffer), free the node, continue left
// Leaf: ?_M_erase@?$_Rb_tree@JU?$pair@$$CBJVCEngineTextureDecal@@@std@@U?$_Select1st@U?$pair@$$CBJVCEngineTextureDecal@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJVCEngineTextureDecal@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBJVCEngineTextureDecal@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0070ADB0, 67 bytes, thiscall, `ret 4`, self-recursive (0x0070ADC6).
// Identity (sweep3; FINDINGS chunk 477 "0x70ADB0 mislabelled STL erase"; was the stand-in CRetailTree_erase0070adb0::Erase over
//   a "12-byte key + buffer" value): the node value at +0x10 is pair<const long, CEngineTextureDecal> (key +0x10, Ego_r
//   CEngineTextureDecal {OriginalTextureIndex, DecalID, vector<CCompositeBlendDesc> OverlaidBlends} = 20 bytes with the
//   12-byte STLport vector, so the freed buffer at +0x1C is OverlaidBlends._M_start); its caller 0x0070A73E is the landed
//   CTCTextureDecal::PeekEngineDecals 0x0070A570 (chunk 477: a local map<long, CEngineTextureDecal> cleared before it dies),
//   and the other callers 0x0070AF90 / 0x0070B0E0 are tree destructors of the same TU (still CRetailTree stand-ins). The
//   manifest "LTextTreeWalkThrough::BuildTreeArray" is propagated. `_M_erase` is private (STLport 4.6 _tree.h, sweep2 1d).
// Callees: free @ 0x00BFEA14 (the blend buffer, null-tested; the node).
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
#define FREE free
namespace std {

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) FREE(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                     // +0x0
    _Tp* _M_finish;                                    // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x8
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { }                                      // trivially destructible elements: _Destroy is empty
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

struct _Rb_tree_node_base {
    bool                _M_color;                      // +0x00
    _Rb_tree_node_base* _M_parent;                     // +0x04
    _Rb_tree_node_base* _M_left;                       // +0x08
    _Rb_tree_node_base* _M_right;                      // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                             // +0x10
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    _STLP_alloc_proxy<_Link_type, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                          // +0x04
    _Compare     _M_key_compare;                                         // +0x08
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
private:
    void _M_erase(_Link_type __x);                     // private in STLport 4.6 _tree.h (sweep2 decision 1d)
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Link_type __x)
{
    // erase without rebalancing
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _Destroy(&__x->_M_value_field);
        this->_M_header.deallocate(__x, 1);
        __x = __y;
    }
}

}   // namespace std

class CRGBColour {
public:
    unsigned char B, G, R, A;
};
class CCompositeBlendDesc {                            // 16 bytes (Ego_r), trivially destructible
public:
    unsigned long TextureIndex;                        // +0x00
    long          BlendType;                           // +0x04 ECompositeBlendType
    CRGBColour    DiffuseColour;                       // +0x08
    CRGBColour    SpecularColour;                      // +0x0C
};

class CEngineTextureDecal {                            // 20 bytes (retail)
public:
    long OriginalTextureIndex;                         // +0x0
    long DecalID;                                      // +0x4
    std::vector<CCompositeBlendDesc> OverlaidBlends;   // +0x8
};

typedef std::pair<const long, CEngineTextureDecal> TDecalValue;
typedef std::_Rb_tree<long, TDecalValue, std::_Select1st<TDecalValue>, std::less<long>, std::allocator<TDecalValue> > TDecalTree;

template void TDecalTree::_M_erase(TDecalTree::_Link_type);
