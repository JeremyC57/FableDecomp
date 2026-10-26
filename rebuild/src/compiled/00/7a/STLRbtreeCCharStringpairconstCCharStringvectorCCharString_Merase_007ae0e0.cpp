// std::_Rb_tree<CCharString, pair<const CCharString, vector<CCharString> >, ...>::_M_erase(_Rb_tree_node* x) -- STLport recursive subtree release without rebalancing: for each node erase its right subtree, destroy the value (the string vector, then the key) and free the node, continuing with its left child
// Leaf: ?_M_erase@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AE0E0, 110 bytes, thiscall, `ret 4`; recursive (calls itself at 0x007AE0F9).
// Callees: ~CCharString 0x0099EAE0, free 0x00BFEA14.
// Shape: STLport 4.5 `_M_erase(x) { while (x != 0) { _M_erase(_S_right(x)); _Link_type y = _S_left(x);
//   _Destroy(&x->_M_value_field); _M_header.deallocate(x, 1); x = y; } }`. The value is a pair<const CCharString,
//   vector<CCharString> > (key +0x10, vector +0x14..+0x1C): the inlined ~vector destroys [start, finish) (finish in ebp,
//   start in esi) and frees the null-tested start (re-read through the pair's address in ebx), then the key string is
//   destroyed; the node free has no null test (x was dereferenced). Register pressure puts `this` in the `push ecx` word
//   and y in x's dead argument slot. The catalog label "List_DestroyRecursiveWithFree" is descriptive.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
};

namespace std {

template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        _Destroy(first);
}

template <class T> class vector {
public:
    T* _M_start;                                            // +0
    T* _M_finish;                                           // +4
    T* _M_end_of_storage;                                   // +8
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start != 0)
            free(_M_start);
    }
};

template <class T1, class T2> class pair {
public:
    T1 first;
    T2 second;
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};

template <class K, class V> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>* _Link_type;
    _Rb_tree_node_base* _M_header;                          // +0x00
    unsigned long       _M_node_count;                      // +0x04
    static _Link_type _S_left(_Link_type x) { return (_Link_type)x->_M_left; }
    static _Link_type _S_right(_Link_type x) { return (_Link_type)x->_M_right; }
private:
    void _M_erase(_Link_type x);
};

template <class K, class V> void _Rb_tree<K, V>::_M_erase(_Link_type x)
{
    while (x != 0) {
        _M_erase(_S_right(x));
        _Link_type y = _S_left(x);
        _Destroy(&x->_M_value_field);
        free(x);
        x = y;
    }
}

template class _Rb_tree<CCharString, pair<const CCharString, vector<CCharString> > >;
}