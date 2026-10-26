// std::_Rb_tree<ECategory, pair<const ECategory, vector<CHeroLogBookEntry> >, _Select1st, less<ECategory> >::_M_insert(iterator* result, _Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- link a new node for v left or right of y, fix root/leftmost/rightmost, rebalance, count, return the iterator
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047C500, 202 bytes, `ret 14h`.
// Callees: malloc @ 0x00BFEA0E (the 0x20-byte node: 0x10 links + the 4-byte key + a 12-byte STLport vector),
// vector<CHeroLogBookEntry>::vector(const vector&) @ 0x0047BB90 (also called by the landed _M_clone_node 0x00619950),
// _Rb_global<bool>::_Rebalance @ 0x0042971D (__cdecl (node, root&)).
// Shape: STLport 4.x `_M_insert` with the explicit iterator out-pointer (as the landed 0x0085DED0); _M_create_node is
// malloc plus the placement copy of the pair under VC's null test (the implicit pair copy stores the key, then calls
// the vector copy constructor); the node goes left when y is the header, or (w null) when x is set or v's key orders
// before y's (signed `jl`: the enum key). Speed TU (the CHeroLogBook module, map CHeroLogBook::LogEntries).
// Label correction: catalog "ListNode_Insert" is a guess.
// Leaf: ?_M_insert@?$_Rb_tree@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@U?$pair@$$CBW4ECategory@CHeroLogBookEntry@CHeroLogBook@@V?$vector@VCHeroLogBookEntry@CHeroLogBook@@@std@@@std@@@std@@AAE?AU?$_Rb_tree_iterator@U?$pair@$$CBW4ECategory@CHeroLogBookEntry@CHeroLogBook@@V?$vector@VCHeroLogBookEntry@CHeroLogBook@@@std@@@std@@@2@PAU_Rb_tree_node_base@2@0ABU?$pair@$$CBW4ECategory@CHeroLogBookEntry@CHeroLogBook@@V?$vector@VCHeroLogBookEntry@CHeroLogBook@@@std@@@2@0@Z
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E

class CWideString {
public:
    void* PStringData;
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
template <class _Value, class _Tp> class _STLP_alloc_proxy {
public:
    _Value _M_data;
};

template <class _Tp> class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp> _M_end_of_storage;         // +0x08
    vector(const vector& __x);                              // 0x0047BB90 for CHeroLogBookEntry
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

template <class _Dummy> struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);   // 0x0042971D
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }   // user ctor: returned through a hidden pointer
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Key, class _Value> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;

    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value> > _M_header;   // +0x00
    unsigned int _M_node_count;                                         // +0x04

protected:
    _Link_type _M_create_node(const _Value& __x)
    {
        _Link_type __tmp = (_Link_type)malloc(sizeof(_Rb_tree_node<_Value>));
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
private:
    iterator _M_insert(_Rb_tree_node_base* __x, _Rb_tree_node_base* __y, const _Value& __v,
                       _Rb_tree_node_base* __w = 0);
};

template <class _Key, class _Value>
typename _Rb_tree<_Key, _Value>::iterator
_Rb_tree<_Key, _Value>::_M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_, const _Value& __v,
                                  _Rb_tree_node_base* __w_)
{
    _Link_type __w = (_Link_type)__w_;
    _Link_type __x = (_Link_type)__x_;
    _Link_type __y = (_Link_type)__y_;
    _Link_type __z;

    if (__y == this->_M_header._M_data ||
        (__w == 0 && (__x != 0 || __v.first < __y->_M_value_field.first))) {
        __z = _M_create_node(__v);
        __y->_M_left = __z;
        if (__y == this->_M_header._M_data) {
            this->_M_header._M_data->_M_parent = __z;
            this->_M_header._M_data->_M_right = __z;
        }
        else if (__y == this->_M_header._M_data->_M_left)
            this->_M_header._M_data->_M_left = __z;
    }
    else {
        __z = _M_create_node(__v);
        __y->_M_right = __z;
        if (__y == this->_M_header._M_data->_M_right)
            this->_M_header._M_data->_M_right = __z;
    }
    __z->_M_parent = __y;
    __z->_M_left = 0;
    __z->_M_right = 0;
    _Rb_global<bool>::_Rebalance(__z, this->_M_header._M_data->_M_parent);
    ++this->_M_node_count;
    return iterator(__z);
}

typedef pair<const CHeroLogBook::CHeroLogBookEntry::ECategory, vector<CHeroLogBook::CHeroLogBookEntry> > _LogValue;
template _Rb_tree<CHeroLogBook::CHeroLogBookEntry::ECategory, _LogValue>::iterator
_Rb_tree<CHeroLogBook::CHeroLogBookEntry::ECategory, _LogValue>::_M_insert(_Rb_tree_node_base*, _Rb_tree_node_base*,
                                                                           const _LogValue&, _Rb_tree_node_base*);
}