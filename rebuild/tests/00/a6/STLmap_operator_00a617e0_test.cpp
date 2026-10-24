// Behaviour test for 0x00A617E0 _STL::map<CWideString, CChecksumCacheFile::CEntry>::operator[](const CWideString&)
#include <cstdio>
#include <cstdlib>
#include <new>

class CWideString {
public:
    const wchar_t* PStringData;
    CWideString(const wchar_t* text) : PStringData(text) { }
    CWideString(const CWideString& other) : PStringData(other.PStringData) { }
    ~CWideString() { }
    bool operator<(const CWideString& other) const          // 0x0099AF70 stand-in: ordinal wide compare
    {
        const wchar_t* a = PStringData;
        const wchar_t* b = other.PStringData;
        while (*a != 0 && *a == *b) { ++a; ++b; }
        return *a < *b;
    }
};
class CDateAndTime {
public:
    long Year;
    unsigned long Month, Day, Hour, Minute, Second, Millisecond;
    CDateAndTime() : Year(1970), Month(1), Day(1), Hour(0), Minute(0), Second(0), Millisecond(0) { }   // 0x009E9AB0 stand-in
};
class CChecksumCacheFile {
public:
    class CEntry {
    public:
        CDateAndTime  TimeStamp;
        unsigned long Checksum;
    };
};

namespace _STL {
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
template <class Pair> struct _Select1st {
    const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class Value> struct _Rb_tree_node : _Rb_tree_node_base {
    Value _M_value_field;
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
template <class Key, class Value, class KeyOfValue, class Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Rb_tree_iterator<Value> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int _M_node_count;
    Compare _M_key_compare;
    _Rb_tree() : _M_node_count(0)
    {
        _M_header = (_Rb_tree_node_base*)std::malloc(sizeof(_Node));
        _M_header->_M_color = false;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header->_M_right = _M_header;
    }
    static const Key& _S_key(_Rb_tree_node_base* x) { return KeyOfValue()(((_Node*)x)->_M_value_field); }
    _Rb_tree_node_base* _M_lower_bound(const Key& k) const                   // 0x00A609C0
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        while (x != 0)
            if (!_M_key_compare(_S_key(x), k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
    iterator lower_bound(const Key& k) { return iterator(_M_lower_bound(k)); }
    iterator end() { return iterator(_M_header); }
    Compare key_comp() const { return _M_key_compare; }
    iterator insert_unique(iterator, const Value& v)    // 0x00A614B0 stand-in: plain (unbalanced) insert, hint ignored
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        bool left = true;
        while (x != 0) {
            y = x;
            left = _M_key_compare(KeyOfValue()(v), _S_key(x));
            x = left ? x->_M_left : x->_M_right;
        }
        _Node* z = (_Node*)std::malloc(sizeof(_Node));
        new (&z->_M_value_field) Value(v);
        z->_M_parent = y;
        z->_M_left = z->_M_right = 0;
        if (y == _M_header) { _M_header->_M_parent = _M_header->_M_left = _M_header->_M_right = z; }
        else if (left) { y->_M_left = z; if (y == _M_header->_M_left) _M_header->_M_left = z; }
        else { y->_M_right = z; if (y == _M_header->_M_right) _M_header->_M_right = z; }
        ++_M_node_count;
        return iterator(z);
    }
};
template <class Key, class Tp, class Compare> class map {
public:
    typedef pair<const Key, Tp> value_type;
    typedef _Rb_tree<Key, value_type, _Select1st<value_type>, Compare> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator lower_bound(const Key& k) { return _M_t.lower_bound(k); }
    iterator end() { return _M_t.end(); }
    Compare key_comp() const { return _M_t.key_comp(); }
    iterator insert(iterator position, const value_type& x) { return _M_t.insert_unique(position, x); }
    Tp& operator[](const Key& k)
    {
        iterator i = lower_bound(k);
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, Tp()));
        return (*i).second;
    }
};
}   // namespace _STL

typedef _STL::map<CWideString, CChecksumCacheFile::CEntry, _STL::less<CWideString> > CEntryMap;

int main()
{
    CEntryMap m;
    CChecksumCacheFile::CEntry& a = m[CWideString(L"art.big")];
    bool ok = m._M_t._M_node_count == 1 && a.TimeStamp.Year == 1970 && a.TimeStamp.Month == 1;   // default CEntry
    a.Checksum = 0x1234;
    CChecksumCacheFile::CEntry& b = m[CWideString(L"level.wld")];
    b.Checksum = 0x5678;
    CChecksumCacheFile::CEntry& c = m[CWideString(L"ant.big")];                // sorts before the others
    c.Checksum = 0x9abc;
    ok = ok && m._M_t._M_node_count == 3 && &m[CWideString(L"art.big")] == &a && m[CWideString(L"art.big")].Checksum == 0x1234 &&
         m[CWideString(L"level.wld")].Checksum == 0x5678 && &m[CWideString(L"ant.big")] == &c && m._M_t._M_node_count == 3;
    if (!ok) { std::printf("00a617e0_TEST FAIL\n"); return 1; }
    std::printf("00a617e0_TEST PASS\n");
    return 0;
}