// Behaviour test for 0x00A4A670: _STL::map<unsigned long, CCharString>::operator[] (lower_bound, insert a default value when the key is absent)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

static int g_liveStrings = 0;
class CCharString {                                   // test model: a deep-copying string with a live count
public:
    char* m_pData;
    CCharString() : m_pData(0) { ++g_liveStrings; }
    CCharString(const char* s) : m_pData(Dup(s)) { ++g_liveStrings; }
    CCharString(const CCharString& other) : m_pData(Dup(other.m_pData)) { ++g_liveStrings; }
    ~CCharString() { free(m_pData); --g_liveStrings; }
    CCharString& operator=(const CCharString& other)
    {
        if (this != &other) {
            char* p = Dup(other.m_pData);
            free(m_pData);
            m_pData = p;
        }
        return *this;
    }
    const char* c_str() const { return m_pData ? m_pData : ""; }
    bool operator<(const CCharString& o) const { return strcmp(c_str(), o.c_str()) < 0; }
    static char* Dup(const char* s)
    {
        if (s == 0) return 0;
        char* p = (char*)malloc(strlen(s) + 1);
        strcpy(p, s);
        return p;
    }
};
static bool Eq(const CCharString& s, const char* t) { return strcmp(s.c_str(), t) == 0; }

namespace _STL {
struct __false_type { __false_type() { } };
template <class _Tp>
inline _Tp __default_constructed_aux(_Tp*, const __false_type&) { return _Tp(); }
template <class _Tp>
inline _Tp __default_constructed(_Tp* __p) { return __default_constructed_aux(__p, __false_type()); }
template <class _Tp>
class less {
public:
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _T1, class _T2>
class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp>
class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value>
struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value>
struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};
static int g_inserts = 0;
template <class _Key, class _Value, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value> iterator;
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Rb_tree_node<_Value>, _Alloc> _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    _Rb_tree()
    {
        _M_header._M_data = (_Rb_tree_node_base*)malloc(sizeof(_Rb_tree_node_base));
        _M_header._M_data->_M_color = 0;
        _M_header._M_data->_M_parent = 0;
        _M_header._M_data->_M_left = _M_header._M_data;
        _M_header._M_data->_M_right = _M_header._M_data;
        _M_node_count = 0;
    }
    ~_Rb_tree()
    {
        Erase(_M_root());
        free(_M_header._M_data);
    }
    void Erase(_Rb_tree_node_base* x)
    {
        if (x == 0) return;
        Erase(x->_M_left);
        Erase(x->_M_right);
        ((_Rb_tree_node<_Value>*)x)->_M_value_field.~_Value();
        free(x);
    }
    _Rb_tree_node_base* _M_root() const { return _M_header._M_data->_M_parent; }
    static const _Key& _S_key(_Rb_tree_node_base* __x) { return ((_Rb_tree_node<_Value>*)__x)->_M_value_field.first; }
    iterator end() { return iterator(_M_header._M_data); }
    iterator lower_bound(const _Key& __k)
    {
        _Rb_tree_node_base* __y = _M_header._M_data;
        _Rb_tree_node_base* __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return iterator(__y);
    }
    // test stand-in for the out-of-line 0x00A49F60: an unbalanced binary insert (the hint is ignored)
    iterator insert_unique(iterator, const _Value& __v)
    {
        ++g_inserts;
        _Rb_tree_node<_Value>* n = (_Rb_tree_node<_Value>*)malloc(sizeof(_Rb_tree_node<_Value>));
        new (&n->_M_value_field) _Value(__v);
        n->_M_color = 0;
        n->_M_left = n->_M_right = 0;
        _Rb_tree_node_base* parent = _M_header._M_data;
        _Rb_tree_node_base** link = &_M_header._M_data->_M_parent;
        while (*link != 0) {
            parent = *link;
            link = _M_key_compare(__v.first, _S_key(parent)) ? &parent->_M_left : &parent->_M_right;
        }
        *link = n;
        n->_M_parent = parent;
        ++_M_node_count;
        return iterator(n);
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator end() { return _M_t.end(); }
    iterator lower_bound(const _Key& __x) { return _M_t.lower_bound(__x); }
    _Compare key_comp() const { return _M_t._M_key_compare; }
    iterator insert(iterator __position, const value_type& __x) { return _M_t.insert_unique(__position, __x); }
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, __default_constructed((_Tp*)0)));
        return (*__i).second;
    }
};
}

int main()
{
    {
        _STL::map<unsigned long, CCharString> m;
        m[5ul] = CCharString("five");
        m[2ul] = CCharString("two");
        m[9ul] = CCharString("nine");
        Check(_STL::g_inserts == 3 && m._M_t._M_node_count == 3);
        Check(Eq(m[5ul], "five") && Eq(m[2ul], "two") && Eq(m[9ul], "nine"));
        Check(_STL::g_inserts == 3);
        Check(Eq(m[7ul], "") && _STL::g_inserts == 4);     // a missing key inserts a default string
        m[7ul] = CCharString("seven");
        Check(Eq(m[7ul], "seven") && m._M_t._M_node_count == 4);
        Check(&m[2ul] == &m[2ul]);
    }
    Check(g_liveStrings == 0);
    if (g_fail) {
        printf("00a4a670_TEST FAIL\n");
        return 1;
    }
    printf("00a4a670_TEST PASS\n");
    return 0;
}