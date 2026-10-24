// _STL::_Rb_tree::insert_unique(const value_type& v) -- map<CWideString, CChecksumCacheFile::CEntry> unique insert: walk to the leaf position, step back one node when the walk ended left, and insert only when that node's key sorts before v's key; returns pair<iterator, bool>
// Leaf: ?insert_unique@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A61360, 155 bytes (+5 int3), `ret 8` (hidden pair result + v).
// Callees: CWideString::operator< @ 0x0099AF70 (less<CWideString> inline), _Rb_tree_iterator::operator--() @ 0x00A60890
//   (out of line, _Rb_global::_M_decrement inlined into it), _Rb_tree::_M_insert @ 0x00A61270.
// Label: the manifest's `LinkedList_FindInsertionPoint` is a guess; STLport 4.6 `_Rb_tree::insert_unique(const value_type&)`.
// Shape: operator-- and _M_insert get same-TU noinline bodies (both live in this TU in retail) so VC7.1 homes j and the
//   _M_insert result temporary in v's dead argument slot. The iterator j is homed in v's dead argument slot (its address goes to operator--).
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

namespace _STL {

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
    typedef _Rb_tree_iterator<Value> _Self;
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    __declspec(noinline) _Self& operator--()    // 0x00A60890 (same-TU body; _Rb_global::_M_decrement inline)
    {
        _Rb_tree_node_base* x = _M_node;
        if (x->_M_color == false && x->_M_parent->_M_parent == x)
            x = x->_M_right;
        else if (x->_M_left != 0) {
            _Rb_tree_node_base* y = x->_M_left;
            while (y->_M_right != 0)
                y = y->_M_right;
            x = y;
        }
        else {
            _Rb_tree_node_base* y = x->_M_parent;
            while (x == y->_M_left) {
                x = y;
                y = y->_M_parent;
            }
            x = y;
        }
        _M_node = x;
        return *this;
    }
    bool operator==(const _Self& y) const { return _M_node == y._M_node; }
};

template <class T1> class pair<T1, bool> {
public:
    T1 first;
    bool second;
    pair(const T1& a, const bool& b) : first(a), second(b) { }
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Rb_tree_iterator<Value> iterator;
    _Rb_tree_node_base* _M_header;              // +0x00
    size_t _M_node_count;                       // +0x04
    Compare _M_key_compare;                     // +0x08

    static const Key& _S_key(_Rb_tree_node_base* x) { return KeyOfValue()(((_Node*)x)->_M_value_field); }
    _Node* _M_root() const { return (_Node*)_M_header->_M_parent; }
    iterator begin() { return iterator(_M_header->_M_left); }
    _Node* _M_create_node(const Value& x)
    {
        _Node* tmp = (_Node*)malloc(sizeof(_Node));
        new (&tmp->_M_value_field) Value(x);
        return tmp;
    }
    __declspec(noinline) iterator _M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const Value& v,
                                            _Rb_tree_node_base* w_ = 0);   // 0x00A61270 (same-TU body)
    pair<iterator, bool> insert_unique(const Value& v);
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

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const Value& v)
{
    _Node* y = (_Node*)_M_header;
    _Node* x = _M_root();
    bool comp = true;
    while (x != 0) {
        y = x;
        comp = _M_key_compare(KeyOfValue()(v), _S_key(x));
        x = comp ? (_Node*)x->_M_left : (_Node*)x->_M_right;
    }
    iterator j = iterator(y);
    if (comp)
        if (j == begin())
            return pair<iterator, bool>(_M_insert(/* x */ y, y, v), true);
        else
            --j;
    if (_M_key_compare(_S_key(j._M_node), KeyOfValue()(v)))
        return pair<iterator, bool>(_M_insert(x, y, v), true);
    return pair<iterator, bool>(j, false);
}

}   // namespace _STL

typedef _STL::pair<const CWideString, CChecksumCacheFile::CEntry> CChecksumEntryValue;

template class _STL::_Rb_tree<CWideString, CChecksumEntryValue, _STL::_Select1st<CChecksumEntryValue>,
    _STL::less<CWideString>, _STL::allocator<CChecksumEntryValue> >;