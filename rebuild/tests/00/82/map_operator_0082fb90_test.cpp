// Behaviour test for 0x0082FB90 (source body copied from src/0082fb90.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
namespace std {
template <class _Tp> _Tp __fastcall __default_constructed(_Tp* __p);   // 0x0082F5C0 (`return _Tp();`)

template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };
template <class A, class B> struct pair {
    typedef A first_type;
    typedef B second_type;
    A first;
    B second;
    pair(const A& a, const B& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
template <class _Pair> struct _Select1st { const typename _Pair::first_type& operator()(const _Pair& x) const { return x.first; } };
template <class T> class allocator { };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                 // +0x00
    _Base_ptr _M_parent;                                // +0x04
    _Base_ptr _M_left;                                  // +0x08
    _Base_ptr _M_right;                                 // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& o) const { return _M_node == o._M_node; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value> iterator;
    _Link_type   _M_header;                             // +0x00
    unsigned int _M_node_count;                         // +0x04
    _Compare     _M_key_compare;                        // +0x08
    _Compare key_comp() const { return _M_key_compare; }
    iterator end() { return iterator(_M_header); }
    _Rb_tree_node_base* _M_lower_bound(const _Key& k) const;   // 0x0082F0D0
    iterator lower_bound(const _Key& k) { return iterator(_M_lower_bound(k)); }
    iterator insert_unique(iterator position, const _Value& v);   // 0x0082F930
};
#pragma auto_inline(off)
// Same-TU stand-in for insert_unique(hint, v) 0x0082F930 (reads v, never keeps its address).
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
__declspec(noinline) typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::insert_unique(iterator position, const _Value& v)
{
    _M_node_count += *(const unsigned char*)&v.first + *(const unsigned char*)&v.second;
    return position;
}
#pragma auto_inline(on)


template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    _Compare key_comp() const { return _M_t.key_comp(); }
    iterator end() { return _M_t.end(); }
    iterator lower_bound(const key_type& x) { return _M_t.lower_bound(x); }
    iterator insert(iterator position, const value_type& x) { return _M_t.insert_unique(position, x); }
    _Tp& operator[](const key_type& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, __default_constructed((_Tp*)0)));
        return (*__i).second;
    }
};
}

class C3DVector {                                       // user memberwise copy constructor
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class C3DAngles {                                       // trivially copyable
public:
    float X, Y, Z;
};
class CScriptedMapBrush {
public:
    class CThingPosAndOrientation {                     // Ego_r.pdb, 24 bytes
    public:
        C3DVector Position;                             // +0x00
        C3DAngles FacingAngles;                         // +0x0c
    };
};

template CScriptedMapBrush::CThingPosAndOrientation&
std::map<unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation>::operator[](const unsigned __int64&);

// ---- test harness ----
// The source's insert_unique is the same-TU stand-in (adds the low bytes of the inserted key and value to the node count,
// returns the hint), so the test checks operator[]'s own logic: the lower_bound walk, the end / key_comp test, the hinted
// insert of {k, T()} and the returned mapped slot.
static unsigned char g_zeroValue[sizeof(CScriptedMapBrush::CThingPosAndOrientation)];
namespace std {
template <> CScriptedMapBrush::CThingPosAndOrientation __fastcall __default_constructed(CScriptedMapBrush::CThingPosAndOrientation*)
{
    return *(const CScriptedMapBrush::CThingPosAndOrientation*)g_zeroValue;
}
template <> _Rb_tree_node_base* _Rb_tree<unsigned __int64, pair<const unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation>,
    _Select1st<pair<const unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation> >, less<unsigned __int64>,
    allocator<pair<const unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation> > >::_M_lower_bound(const unsigned __int64& k) const
{
    _Rb_tree_node_base* y = _M_header;
    _Rb_tree_node_base* x = _M_header->_M_parent;
    while (x != 0)
        if (!(((_Rb_tree_node<pair<const unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation> >*)x)->_M_value_field.first < k)) { y = x; x = x->_M_left; }
        else x = x->_M_right;
    return y;
}
}
typedef std::map<unsigned __int64, CScriptedMapBrush::CThingPosAndOrientation> TMap;
typedef TMap::value_type TValue;
struct STestNode { long color; STestNode* parent; STestNode* left; STestNode* right; TValue value; };
static STestNode* MakeNode(unsigned __int64 k)
{
    STestNode* n = (STestNode*)malloc(sizeof(STestNode));
    memset(n, 0, sizeof *n);
    *(unsigned __int64*)&n->value.first = k;
    return n;
}
static unsigned long CountOf(const TMap& m) { return *(const unsigned int*)((const char*)&m + sizeof(void*)); }
int main()
{
    int ok = 1;
    STestNode* header = MakeNode(0);
    STestNode* n5 = MakeNode(5); STestNode* n2 = MakeNode(2); STestNode* n9 = MakeNode(9);
    header->parent = n5; n5->parent = header;
    n5->left = n2; n5->right = n9; n2->parent = n5; n9->parent = n5;
    static unsigned char raw[sizeof(TMap) + 16];
    memset(raw, 0, sizeof raw);
    TMap& m = *(TMap*)raw;
    *(STestNode**)raw = header;
    *(unsigned int*)(raw + sizeof(void*)) = 3;
    unsigned __int64 k5 = 5, k2 = 2, k7 = 7, k10 = 10;
    if ((void*)&m[k5] != (void*)&n5->value.second || CountOf(m) != 3) ok = 0;
    if ((void*)&m[k2] != (void*)&n2->value.second || CountOf(m) != 3) ok = 0;
    if ((void*)&m[k7] != (void*)&n9->value.second || CountOf(m) != 3 + 7) ok = 0;          // hinted insert before 9, T() low byte 0
    if ((void*)&m[k10] != (void*)&header->value.second || CountOf(m) != 3 + 7 + 10) ok = 0;  // insert at end()
    printf("0082fb90_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}