// Behaviour test for 0x0070ADB0 std::_Rb_tree<long, pair<const long, CEngineTextureDecal>, ...>::_M_erase: the
// STLport body verbatim over a hand-built 7-node tree; every node and every non-null blend buffer is freed once.
#include <stdio.h>
#include <stdlib.h>
int main();
static int g_freed = 0;
static void CountedFree(void* p) { ++g_freed; free(p); }
#define FREE CountedFree
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
class vector : public _Vector_base<_Tp, _Alloc> {   // public here so the test can build raw vectors
public:
    ~vector() { }                                      // trivially destructible elements: _Destroy is empty
    friend int ::main();
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
    friend int ::main();
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

typedef TDecalTree::_Node TNode;
static TNode* MakeNode(long key, TNode* left, TNode* right)
{
    TNode* n = (TNode*)malloc(sizeof(TNode));
    n->_M_color = false; n->_M_parent = 0; n->_M_left = left; n->_M_right = right;
    *(long*)&n->_M_value_field.first = key;
    n->_M_value_field.second.OriginalTextureIndex = key;
    n->_M_value_field.second.OverlaidBlends._M_start = (key & 1) ? (CCompositeBlendDesc*)malloc(sizeof(CCompositeBlendDesc)) : 0;
    n->_M_value_field.second.OverlaidBlends._M_finish = n->_M_value_field.second.OverlaidBlends._M_start;
    n->_M_value_field.second.OverlaidBlends._M_end_of_storage._M_data = n->_M_value_field.second.OverlaidBlends._M_start;
    return n;
}
int main()
{
    bool ok = sizeof(CEngineTextureDecal) == 20 && sizeof(TNode) == 0x28;
    TNode* root = MakeNode(4, MakeNode(2, MakeNode(1, 0, 0), MakeNode(3, 0, 0)), MakeNode(6, MakeNode(5, 0, 0), MakeNode(7, 0, 0)));
    TDecalTree tree; tree._M_header._M_data = 0; tree._M_node_count = 7;
    tree._M_erase(root);
    ok = ok && g_freed == 7 + 4;             // 7 nodes + the 4 odd keys' blend buffers
    tree._M_erase(0);
    ok = ok && g_freed == 11;
    printf(ok ? "0070adb0_TEST PASS\n" : "0070adb0_TEST FAIL\n");
    return ok ? 0 : 1;
}
