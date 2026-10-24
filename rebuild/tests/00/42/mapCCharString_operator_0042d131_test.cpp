// Behaviour test for std::map<CCharString, unsigned long>::operator[] (0x0042D131): an existing key returns
// its value slot without inserting, a new key is inserted (in order) with value 0 and its slot returned; the
// temporary pair's key copy is destroyed again.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
inline void* operator new(unsigned int, void* p) { return p; }
static int g_liveStrings = 0;
static int g_inserts = 0;
template <class C> class CBasicString {
public:
    const C* Text;
    bool operator<(const CBasicString& other) const { return strcmp(Text, other.Text) < 0; }
};

class CCharString
{
public:
    CBasicString<char>* PData;                           // +0x00
    CCharString(CBasicString<char>* data) : PData(data) { ++g_liveStrings; }
    CCharString(const CCharString& other) : PData(other.PData) { ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
    __forceinline bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
};

namespace std {
template <class T> struct less {
    __declspec(noinline) bool operator()(const T& x, const T& y) const { return x < y; }   // 0x0042AAB3 for CCharString
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y) const { return _M_node == y._M_node; }
};
template <class V> struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    V& operator*() const { return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
};
template <class K, class T, class C = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    C                   _M_key_compare;
    // test stand-ins: an ordered chain through _M_right (header -> first ... -> header)
    _Rb_tree_node_base* _M_lower_bound(const K& k) const
    {
        _Rb_tree_node_base* n = _M_header->_M_right;
        while (n != _M_header && static_cast<_Rb_tree_node<value_type>*>(n)->_M_value_field.first < k)
            n = n->_M_right;
        return n;
    }
    iterator insert(iterator position, const value_type& v)
    {
        _Rb_tree_node<value_type>* node = (_Rb_tree_node<value_type>*)malloc(sizeof(_Rb_tree_node<value_type>));
        new (&node->_M_value_field) value_type(v);
        _Rb_tree_node_base* prev = _M_header;
        while (prev->_M_right != position._M_node) prev = prev->_M_right;
        node->_M_right = position._M_node;
        prev->_M_right = node;
        ++_M_node_count;
        ++g_inserts;
        return iterator(node);
    }
    iterator lower_bound(const K& k) { return iterator(_M_lower_bound(k)); }
    iterator end() { return iterator(_M_header); }
    C key_comp() const { return _M_key_compare; }
    T& operator[](const K& k)
    {
        iterator i = lower_bound(k);
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, T()));
        return (*i).second;
    }
};
template unsigned long& map<CCharString, unsigned long>::operator[](const CCharString&);
}

typedef std::map<CCharString, unsigned long> CNameMap;

static const char* KeyAt(CNameMap& m, int i)
{
    std::_Rb_tree_node_base* n = m._M_header->_M_right;
    while (i-- > 0) n = n->_M_right;
    return static_cast<std::_Rb_tree_node<CNameMap::value_type>*>(n)->_M_value_field.first.PData->Text;
}

int main()
{
    CBasicString<char> sB = { "beta" }, sA = { "alpha" }, sC = { "gamma" };
    std::_Rb_tree_node_base header;
    CNameMap m;
    header._M_right = &header;
    m._M_header = &header;
    m._M_node_count = 0;
    {
        CCharString b(&sB), a(&sA), c(&sC);
        m[b] = 7;
        if (g_inserts != 1 || m._M_node_count != 1 || m[b] != 7 || g_inserts != 1) { printf("FAIL existing\n"); return 1; }
        unsigned long& va = m[a];
        if (g_inserts != 2 || va != 0) { printf("FAIL new value\n"); return 1; }
        va = 3;
        m[c] += 5;
        if (g_inserts != 3 || m[a] != 3 || m[b] != 7 || m[c] != 5) { printf("FAIL values\n"); return 1; }
        if (strcmp(KeyAt(m, 0), "alpha") || strcmp(KeyAt(m, 1), "beta") || strcmp(KeyAt(m, 2), "gamma")) { printf("FAIL order\n"); return 1; }
    }
    if (g_liveStrings != 3) { printf("FAIL temporaries %d\n", g_liveStrings); return 1; }   // only the three stored keys remain
    printf("PASS_0042d131\n");
    return 0;
}
