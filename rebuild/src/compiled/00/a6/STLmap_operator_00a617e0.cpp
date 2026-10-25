// std::map::operator[](const CWideString& k) -- map<CWideString, CChecksumCacheFile::CEntry> subscript: lower_bound(k); when that is end() or a greater key, insert (k, CEntry()) at that hint; return the mapped CEntry
// Leaf: ??A?$map@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A617E0, 136 bytes, `ret 4`, 0x44-byte frame (the CEntry()
//   temporary +0x10, the value_type temporary +0x30).
// Callees: _Rb_tree::_M_lower_bound @ 0x00A609C0, CWideString::operator< @ 0x0099AF70 (less<CWideString> inline),
//   CDateAndTime::CDateAndTime() @ 0x009E9AB0 (CEntry's implicit default ctor), CWideString(const CWideString&)
//   @ 0x0099B720, _Rb_tree::insert_unique(iterator, const value_type&) @ 0x00A614B0, ~CWideString @ 0x0099B510.
// Label: the manifest's `TreeNode_GetByKey` is a guess; STLport 4.6 `map::operator[]`.
// Layout (FableWin/Ego_r PDB): CChecksumCacheFile::CEntry = { CDateAndTime TimeStamp +0 (0x1c), ulong Checksum +0x1c };
//   node value at +0x10 (key) / +0x14 (mapped).
#include <stddef.h>

class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
    bool operator<(const CWideString& other) const;         // 0x0099AF70
};
class CDateAndTime {
public:
    long          Year;                                     // +0x00
    unsigned long Month;                                    // +0x04
    unsigned long Day;                                      // +0x08
    unsigned long Hour;                                     // +0x0c
    unsigned long Minute;                                   // +0x10
    unsigned long Second;                                   // +0x14
    unsigned long Millisecond;                              // +0x18
    CDateAndTime();                                         // 0x009E9AB0
};
class CChecksumCacheFile {
public:
    class CEntry {
    public:
        CDateAndTime  TimeStamp;                            // +0x00
        unsigned long Checksum;                             // +0x1c
    };
};

namespace std {

template <class T1, class T2> class pair {
public:
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
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

template <class Value> struct _Rb_tree_iterator {
    typedef _Rb_tree_iterator<Value> _Self;
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Self& it) : _M_node(it._M_node) { }
    Value& operator*() const { return ((_Rb_tree_node<Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Self& y) const { return _M_node == y._M_node; }
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Rb_tree_iterator<Value> iterator;
    _Rb_tree_node_base* _M_header;              // +0x00
    size_t _M_node_count;                       // +0x04
    Compare _M_key_compare;                     // +0x08

    _Rb_tree_node_base* _M_lower_bound(const Key& k) const;               // 0x00A609C0
    iterator lower_bound(const Key& k) { return iterator(_M_lower_bound(k)); }
    iterator end() { return iterator(_M_header); }
    Compare key_comp() const { return _M_key_compare; }
    iterator insert_unique(iterator position, const Value& v);           // 0x00A614B0
};

template <class Key, class Tp, class Compare, class Alloc> class map {
public:
    typedef pair<const Key, Tp> value_type;
    typedef _Rb_tree<Key, value_type, _Select1st<value_type>, Compare, Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;

    iterator lower_bound(const Key& k) { return _M_t.lower_bound(k); }
    iterator end() { return _M_t.end(); }
    Compare key_comp() const { return _M_t.key_comp(); }
    iterator insert(iterator position, const value_type& x) { return _M_t.insert_unique(position, x); }
    Tp& operator[](const Key& k);
};

template <class Key, class Tp, class Compare, class Alloc>
Tp& map<Key, Tp, Compare, Alloc>::operator[](const Key& k)
{
    iterator i = lower_bound(k);
    // i->first is greater than or equivalent to k.
    if (i == end() || key_comp()(k, (*i).first))
        i = insert(i, value_type(k, Tp()));
    return (*i).second;
}

template class map<CWideString, CChecksumCacheFile::CEntry, less<CWideString>,
                   allocator<pair<const CWideString, CChecksumCacheFile::CEntry> > >;

}   // namespace std