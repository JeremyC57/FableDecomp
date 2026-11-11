// std::_Rb_tree::_M_erase(_Rb_tree_node* x) -- STLport erase-without-rebalancing for CEngineScene::MapList (map<long, map<long, CCountedPointer<CEngineSceneMap> > >): recurse right, destroy the row's inner map (clear + free its header), free the node, continue left
// Leaf: ?_M_erase@?$_Rb_tree@JU?$pair@$$CBJV?$map@JV?$CCountedPointer@VCEngineSceneMap@@@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJV?$CCountedPointer@VCEngineSceneMap@@@@@std@@@3@@std@@@std@@U?$_Select1st@U?$pair@$$CBJV?$map@JV?$CCountedPointer@VCEngineSceneMap@@@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJV?$CCountedPointer@VCEngineSceneMap@@@@@std@@@3@@std@@@std@@@2@U?$less@J@2@V?$allocator@U?$pair@$$CBJV?$map@JV?$CCountedPointer@VCEngineSceneMap@@@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJV?$CCountedPointer@VCEngineSceneMap@@@@@std@@@3@@std@@@std@@@2@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBJV?$map@JV?$CCountedPointer@VCEngineSceneMap@@@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJV?$CCountedPointer@VCEngineSceneMap@@@@@std@@@3@@std@@@std@@@2@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B98350, 114 bytes, thiscall, `ret 4`.
// Identity: recursive (0x00B9836B) and called by the outer tree's destructor / clear (0x00B94BBF, 0x00B985A2, 0x00B98812,
//   0x00B988A2; the landed CRetailTree_erase00b98350 destructors). The 0x20-byte node holds a long key and an STLport
//   map {header, count, less<long>} whose own _M_erase is 0x00B97580: Ego_r.pdb CEngineScene::MapList. Catalog
//   "Utility_DestroyRefcountedTreeNode" is descriptive.
// Callees: the inner map's _M_erase 0x00B97580 (out of line), free @ 0x00BFEA14 (the inner header, null-tested; the
//   node, known non-null).
// Shape: STLport 4.x `while (x != 0) { _M_erase(_S_right(x)); y = _S_left(x); _Destroy(&x->_M_value_field);
//   deallocate(x); x = y; }` with the inner map's destructor inline (`clear()`: count test, _M_erase(root), header
//   relinked through the re-read header pointer, count zeroed; then the header freed). `this` parks in the `push ecx`
//   frame word. Same source shape as the landed list<CVertexGroupBlock>::erase 0x00ADC4E0 tree teardown.
// Name fix (agentNF23): the trees are STLport 4.6 _Rb_tree<K, V, KeyOfValue, Compare, Alloc> and map<K, T, Compare, Alloc> (5 / 4 template arguments): the row's own symbol is now the queue's (land1011da, agentFAM07; the spelling CEngineScene::~CEngineScene 0x00B94B50 binds) and the inner map's _M_erase 0x00B97580 binds the landed 5-argument spelling. Was a 3-argument _Rb_tree<K, V, Compare> model.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class CEngineSceneMap;

#include "engine/CCountedPointer.h"

namespace std {

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class T> class allocator {
public:
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};

template <class T> class _STLP_alloc_proxy : public allocator<T> {
public:
    T* _M_data;
};

template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};
template <class T1, class T2> struct pair {
    typedef T1 first_type;
    T1 first;
    T2 second;
};
template <class P> struct _Select1st {                       // STLport 4.6 <stl/_function.h> (the map's key-of-value)
    const typename P::first_type& operator()(const P& x) const { return x.first; }
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};

template <class V> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    _STLP_alloc_proxy<_Node> _M_header;                     // +0x00
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};

template <class K, class V, class KoV, class Compare, class Alloc> class _Rb_tree : public _Rb_tree_base<V> {
public:
    typedef _Rb_tree_node<V>    _Node;
    typedef _Node*              _Link_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    unsigned int _M_node_count;                             // +0x04
    Compare      _M_key_compare;                            // +0x08

    protected:
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    public:
    protected:
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    public:
    protected:
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    public:
    static _Link_type& _S_left(_Base_ptr x) { return (_Link_type&)x->_M_left; }
    static _Link_type& _S_right(_Base_ptr x) { return (_Link_type&)x->_M_right; }

private:
    void _M_erase(_Link_type __x);
public:
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
    ~_Rb_tree() { clear(); }
};

template <class K, class V, class KoV, class Compare, class Alloc>
void _Rb_tree<K, V, KoV, Compare, Alloc>::_M_erase(_Link_type __x)
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

template <class K, class T, class Compare = less<K>, class Alloc = allocator<pair<const K, T> > > class map {
public:
    typedef pair<const K, T> value_type;
    _Rb_tree<K, value_type, _Select1st<value_type>, Compare, Alloc> _M_t;
};

typedef pair<const long, CCountedPointer<CEngineSceneMap> > TSceneMapValue;
template <> void _Rb_tree<long, TSceneMapValue, _Select1st<TSceneMapValue>, less<long>, allocator<TSceneMapValue> >::_M_erase(_Link_type __x);   // 0x00B97580 (inner map)

typedef map<long, CCountedPointer<CEngineSceneMap> > TSceneMapRow;
template void _Rb_tree<long, pair<const long, TSceneMapRow>, _Select1st<pair<const long, TSceneMapRow> >, less<long>, allocator<pair<const long, TSceneMapRow> > >::_M_erase(_Link_type);

} // namespace std