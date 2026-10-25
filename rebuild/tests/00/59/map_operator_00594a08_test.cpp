// Behaviour test for 0x00594A08 map<CDateAndTime, CWideString>::operator[] (source body copied; tree helpers stubbed
// with a plain binary search tree under the STLport header node).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#endif
class CDateAndTime {
public:
    unsigned char _body[0x1c];                       // 0x1C bytes: the CWideString value follows at node + 0x2C
    bool operator<(const CDateAndTime& other) const; // 0x009E9B80
};
class CWideString {
public:
    void* PStringData;
    CWideString();                                   // 0x0099AED0
    ~CWideString();                                  // 0x0099B510
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b);            // 0x005921A4
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> inline _Tp __default_constructed(_Tp*) { return _Tp(); }   // _STLP_DEFAULT_CONSTRUCTED
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : _Rb_tree_node_base {
    _Value _M_value_field;                           // +0x10
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) {}
    _Value& operator*() const { return static_cast<_Rb_tree_node<_Value>*>(_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};
template <class _Key, class _Tp, class _Compare = less<_Key> > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                   // +0x00 (_M_t._M_header._M_data)
    size_t              _M_node_count;               // +0x04
    _Compare            _M_key_compare;              // +0x08
    _Rb_tree_node_base* _M_lower_bound(const _Key& __k) const;          // 0x0059339A
    iterator insert(iterator __position, const value_type& __x);       // 0x0059483E
    iterator lower_bound(const _Key& __x) { return iterator(_M_lower_bound(__x)); }
    iterator end() { return iterator(_M_header); }
    _Compare key_comp() const { return _M_key_compare; }
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        // __i->first is greater than or equivalent to __k.
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, __default_constructed((_Tp*)0)));
        return (*__i).second;
    }
};
template CWideString& map<CDateAndTime, CWideString>::operator[](const CDateAndTime&);
}

static int g_ctor = 0, g_dtor = 0, g_insert = 0;
static int g_defaultMarker;
bool CDateAndTime::operator<(const CDateAndTime& other) const { return memcmp(_body, other._body, sizeof _body) < 0; }
CWideString::CWideString() { PStringData = &g_defaultMarker; ++g_ctor; }
CWideString::~CWideString() { ++g_dtor; }
namespace std {
template <> pair<const CDateAndTime, CWideString>::pair(const CDateAndTime& a, const CWideString& b) : first(a), second(b) {}
typedef map<CDateAndTime, CWideString> TMap;
typedef _Rb_tree_node<TMap::value_type> TNode;
template <> _Rb_tree_node_base* TMap::_M_lower_bound(const CDateAndTime& k) const
{
    _Rb_tree_node_base* y = _M_header;
    _Rb_tree_node_base* x = _M_header->_M_parent;
    while (x != 0)
        if (!(((TNode*)x)->_M_value_field.first < k)) { y = x; x = x->_M_left; }
        else x = x->_M_right;
    return y;
}
template <> TMap::iterator TMap::insert(iterator, const value_type& v)
{
    ++g_insert;
    TNode* n = (TNode*)malloc(sizeof(TNode));
    new (&n->_M_value_field) value_type(v);
    n->_M_left = n->_M_right = 0;
    _Rb_tree_node_base** link = &_M_header->_M_parent;
    _Rb_tree_node_base* parent = _M_header;
    while (*link) { parent = *link; link = (v.first < ((TNode*)*link)->_M_value_field.first) ? &(*link)->_M_left : &(*link)->_M_right; }
    n->_M_parent = parent; *link = n; ++_M_node_count;
    return iterator(n);
}
}
static CDateAndTime Key(int v) { CDateAndTime d; memset(&d, 0, sizeof d); d._body[0] = (unsigned char)v; return d; }
int main()
{
    int ok = 1;
    std::_Rb_tree_node_base header; memset(&header, 0, sizeof header);
    std::TMap m; m._M_header = &header; m._M_node_count = 0;
    int marker1, marker2;
    CDateAndTime k5 = Key(5), k2 = Key(2), k9 = Key(9);
    CWideString& a = m[k5];
    if (a.PStringData != &g_defaultMarker || g_insert != 1) ok = 0;
    a.PStringData = &marker1;
    CWideString& b = m[k2];
    b.PStringData = &marker2;
    if (g_insert != 2) ok = 0;
    if (m[k5].PStringData != &marker1 || m[k2].PStringData != &marker2 || g_insert != 2) ok = 0;
    if (m[k9].PStringData != &g_defaultMarker || g_insert != 3 || m._M_node_count != 3) ok = 0;
    printf("00594a08_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}