// Behaviour test for 0x00A547C0: std::_Rb_tree<CCharString, pair<const CCharString, long>>::_M_copy (structural copy: recurse right, walk left)
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

namespace std {
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2>
class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};
template <class _Tp>
class less {
public:
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp>
class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
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
template <class _Key, class _Value, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Rb_tree_node<_Value>, allocator<_Rb_tree_node<_Value> > > _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    static _Link_type& _S_left(_Rb_tree_node_base* __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Rb_tree_node_base* __x) { return (_Link_type&)(__x->_M_right); }
    _Link_type _M_create_node(const _Value& __x)
    {
        _Link_type __tmp = _M_header.allocate(1);
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
    _Link_type _M_clone_node(_Link_type __x)
    {
        _Link_type __tmp = _M_create_node(__x->_M_value_field);
        __tmp->_M_color = __x->_M_color;
        __tmp->_M_left = 0;
        __tmp->_M_right = 0;
        return __tmp;
    }
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
};
template <class _Key, class _Value, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _Compare, _Alloc>::_Link_type
_Rb_tree<_Key, _Value, _Compare, _Alloc>::_M_copy(_Link_type __x, _Link_type __p)
{
    _Link_type __top = _M_clone_node(__x);
    __top->_M_parent = __p;
    if (__x->_M_right)
        __top->_M_right = _M_copy(_S_right(__x), __top);
    __p = __top;
    __x = _S_left(__x);
    while (__x != 0) {
        _Link_type __y = _M_clone_node(__x);
        __p->_M_left = __y;
        __y->_M_parent = __p;
        if (__x->_M_right)
            __y->_M_right = _M_copy(_S_right(__x), __y);
        __p = __y;
        __x = _S_left(__x);
    }
    return __top;
}
}

typedef std::pair<const CCharString, long> TValue;
typedef std::_Rb_tree<CCharString, TValue, std::less<CCharString>, std::allocator<TValue> > TTree;
typedef std::_Rb_tree_node<TValue> TNode;

static TNode* MakeNode(const char* key, long value, char color, TNode* parent)
{
    TNode* n = (TNode*)malloc(sizeof(TNode));
    new (&n->_M_value_field) TValue(CCharString(key), value);
    n->_M_color = color;
    n->_M_parent = parent;
    n->_M_left = n->_M_right = 0;
    return n;
}
static void FreeTree(std::_Rb_tree_node_base* x)
{
    if (x == 0) return;
    FreeTree(x->_M_left);
    FreeTree(x->_M_right);
    ((TNode*)x)->_M_value_field.~TValue();
    free(x);
}
static bool SameTree(const std::_Rb_tree_node_base* a, const std::_Rb_tree_node_base* b, const std::_Rb_tree_node_base* parentB)
{
    if (a == 0 || b == 0) return a == b;
    if (a == b || b->_M_parent != parentB || a->_M_color != b->_M_color) return false;
    const TValue& va = ((const TNode*)a)->_M_value_field;
    const TValue& vb = ((const TNode*)b)->_M_value_field;
    if (!Eq(vb.first, va.first.c_str()) || va.second != vb.second || va.first.m_pData == vb.first.m_pData) return false;
    return SameTree(a->_M_left, b->_M_left, b) && SameTree(a->_M_right, b->_M_right, b);
}

int main()
{
    {
        //            d
        //        b       f
        //      a   c   e
        //     0
        TNode* d = MakeNode("d", 4, 1, 0);
        TNode* b = MakeNode("b", 2, 0, d);
        TNode* f = MakeNode("f", 6, 1, d);
        TNode* a = MakeNode("a", 1, 1, b);
        TNode* c = MakeNode("c", 3, 1, b);
        TNode* e = MakeNode("e", 5, 0, f);
        TNode* z = MakeNode("0", 0, 0, a);
        d->_M_left = b; d->_M_right = f; b->_M_left = a; b->_M_right = c; f->_M_left = e; a->_M_left = z;
        TTree t;
        std::_Rb_tree_node_base header;
        TNode* copy = t._M_copy(d, (TNode*)&header);
        Check(copy != d && copy->_M_parent == (std::_Rb_tree_node_base*)&header);
        Check(SameTree(d->_M_left, copy->_M_left, copy) && SameTree(d->_M_right, copy->_M_right, copy));
        Check(Eq(copy->_M_value_field.first, "d") && copy->_M_value_field.second == 4 && copy->_M_color == 1);
        int live = g_liveStrings;
        FreeTree(copy);
        Check(g_liveStrings == live - 7);
        FreeTree(d);
    }
    Check(g_liveStrings == 0);
    if (g_fail) {
        printf("00a547c0_TEST FAIL\n");
        return 1;
    }
    printf("00a547c0_TEST PASS\n");
    return 0;
}