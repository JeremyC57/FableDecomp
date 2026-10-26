// std::_Rb_tree<ECategory, pair<const ECategory, vector<CHeroLogBookEntry> >, _Select1st, less<ECategory> >::_M_erase(_Rb_tree_node<value_type>* x) -- erase a subtree without rebalancing: recurse right, destroy the node's entry vector, free the node, walk left
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047C6A0, 128 bytes (+ int3 padding), `ret 4`, recursive.
// Callees: itself, ~CWideString @ 0x0099B510 (x3 per entry: the implicit ~CHeroLogBookEntry), free @ 0x00BFEA14
// (the vector storage behind ~_Vector_base's null test, then the node: __malloc_alloc deallocate).
// Shape: STLport 4.x `_M_erase` loop: `_M_erase(_S_right(x)); y = _S_left(x); _Destroy(&x->_M_value_field);
// deallocate(x); x = y;`; the pair destructor inlines ~vector (`_Destroy(start, finish)` -> the __destroy_aux loop,
// aligned) and ~_Vector_base; `this` lives in a frame slot across the recursion. Speed TU (CHeroLogBook LogEntries).
// Label correction: catalog "TreeNode_RecursiveDelete" is a behavioural guess; the STLport member is _M_erase.
// Leaf: ?_M_erase@?$_Rb_tree@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@U?$pair@$$CBW4ECategory@CHeroLogBookEntry@CHeroLogBook@@V?$vector@VCHeroLogBookEntry@CHeroLogBook@@@std@@@std@@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBW4ECategory@CHeroLogBookEntry@CHeroLogBook@@V?$vector@VCHeroLogBookEntry@CHeroLogBook@@@std@@@std@@@2@@Z
#include <stdlib.h>

class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    ~CWideString();                                         // 0x0099B510
};

class CHeroLogBook {
public:
    class CHeroLogBookEntry {
    public:
        enum ECategory { CATEGORY_NONE };
        CWideString Name;                                   // +0x00
        CWideString AbbreviatedName;                        // +0x04
        CWideString Content;                                // +0x08
    };
};

namespace std {
struct __false_type { };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Value, class _Tp> class _STLP_alloc_proxy {
public:
    _Value _M_data;
};

template <class _Tp> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp> _M_end_of_storage;         // +0x08
    ~_Vector_base()
    {
        if (_M_start != 0)
            free(_M_start);
    }
};

template <class _Tp> class vector : public _Vector_base<_Tp> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

template <class _T1, class _T2> struct pair {
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0c
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Key, class _Value> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;

    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value> > _M_header;   // +0x00
    unsigned int _M_node_count;                                         // +0x04

    static _Link_type& _S_left(_Rb_tree_node_base* __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Rb_tree_node_base* __x) { return (_Link_type&)(__x->_M_right); }
private:
    void _M_erase(_Link_type __x);
};

template <class _Key, class _Value> void _Rb_tree<_Key, _Value>::_M_erase(_Link_type __x)
{
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _Destroy(&__x->_M_value_field);
        free(__x);                                          // _M_header.deallocate(__x, 1)
        __x = __y;
    }
}

typedef pair<const CHeroLogBook::CHeroLogBookEntry::ECategory, vector<CHeroLogBook::CHeroLogBookEntry> > _LogValue;
template void _Rb_tree<CHeroLogBook::CHeroLogBookEntry::ECategory, _LogValue>::_M_erase(_Rb_tree_node<_LogValue>*);
}