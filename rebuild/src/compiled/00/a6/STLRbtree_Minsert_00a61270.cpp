// std::_Rb_tree::_M_insert(_Rb_tree_node_base* x, _Rb_tree_node_base* y, const value_type& v, _Rb_tree_node_base* w) -- map<CWideString, CChecksumCacheFile::CEntry> node insert: allocate a node, copy the pair in, link it left or right of y (keeping leftmost/rightmost), rebalance and count
// Leaf: ?_M_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A61270, 233 bytes (+7 int3), `ret 14h` (hidden iterator result + four
//   arguments).
// Callees: CWideString::operator< @ 0x0099AF70 (less<CWideString> inline), malloc @ 0x00BFEA0E (0x34-byte node),
//   CWideString::CWideString(const CWideString&) @ 0x0099B720 (the 0x20-byte CEntry is a `rep movsd` copy),
//   _Rb_global<bool>::_Rebalance @ 0x0042971D.
// Label: the manifest's `ListNode_Insert` is a guess; STLport 4.6 `_Rb_tree::_M_insert`, the same body as the landed
//   CPendingObjectSortKey _M_insert @ 0x00990830.
#include <stddef.h>

extern "C" void* __cdecl malloc(size_t size);  // 0x00BFEA0E
inline void* operator new(size_t, void* where) { return where; }

class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
    bool operator<(const CWideString& other) const;         // 0x0099AF70
};
class CChecksumCacheFile {
public:
    class CEntry {                                          // 0x20 bytes, trivially copyable (checksum + CDateAndTime)
    public:
        unsigned long Data[8];
    };
};

namespace std {

template <class T1, class T2> class pair {
public:
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
};

template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};

template <class T> class allocator { };

template <class Pair> struct _Select1st {
    const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};

struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
};

template <class Value> struct _Rb_tree_node : _Rb_tree_node_base {
    Value _M_value_field;                       // +0x10
};

struct _Rb_global {
    static void __cdecl _Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root);   // 0x0042971D
};

template <class Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Rb_tree_iterator<Value> iterator;
    _Rb_tree_node_base* _M_header;              // +0x00
    size_t _M_node_count;                       // +0x04
    Compare _M_key_compare;                     // +0x08

    static const Key& _S_key(_Rb_tree_node_base* x) { return KeyOfValue()(((_Node*)x)->_M_value_field); }
protected:
    _Node* _M_create_node(const Value& x)
    {
        _Node* tmp = (_Node*)malloc(sizeof(_Node));
        new (&tmp->_M_value_field) Value(x);
        return tmp;
    }
private:
    iterator _M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const Value& v, _Rb_tree_node_base* w_ = 0);
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const Value& v,
                                                           _Rb_tree_node_base* w_)
{
    _Node* w = (_Node*)w_;
    _Node* x = (_Node*)x_;
    _Node* y = (_Node*)y_;
    _Node* z;
    if (y == _M_header || (w == 0 && (x != 0 || _M_key_compare(KeyOfValue()(v), _S_key(y))))) {
        z = _M_create_node(v);
        y->_M_left = z;
        if (y == _M_header) {
            _M_header->_M_parent = z;
            _M_header->_M_right = z;
        }
        else if (y == _M_header->_M_left)
            _M_header->_M_left = z;
    }
    else {
        z = _M_create_node(v);
        y->_M_right = z;
        if (y == _M_header->_M_right)
            _M_header->_M_right = z;
    }
    z->_M_parent = y;
    z->_M_left = 0;
    z->_M_right = 0;
    _Rb_global::_Rebalance(z, _M_header->_M_parent);
    ++_M_node_count;
    return iterator(z);
}

}   // namespace std

typedef std::pair<const CWideString, CChecksumCacheFile::CEntry> CChecksumEntryValue;

template class std::_Rb_tree<CWideString, CChecksumEntryValue, std::_Select1st<CChecksumEntryValue>,
    std::less<CWideString>, std::allocator<CChecksumEntryValue> >;