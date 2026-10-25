// Behaviour test for 0x007EA1A0 (source body copied from src/007ea1a0.cpp; out-of-line callees stubbed below).
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
#include <stddef.h>

class CThing;

namespace _STL {

template <class T1, class T2> struct pair {
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

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};

template <class Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {   // STLport: body assignments
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    Value& operator*() const { return static_cast<_Rb_tree_node<Value>*>(_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& other) const { return _M_node == other._M_node; }
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Rb_tree_iterator<Value> iterator;
    _Rb_tree_node_base* _M_header;              // +0x00
    size_t _M_node_count;                       // +0x04
    Compare _M_key_compare;                     // +0x08

    static const Key& _S_key(_Rb_tree_node_base* x) { return KeyOfValue()(((_Node*)x)->_M_value_field); }
    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    iterator end() { return iterator(_M_header); }
    Compare key_comp() const { return _M_key_compare; }
    iterator lower_bound(const Key& k)
    {
        _Rb_tree_node_base* y = _M_header;      // last node which is not less than k
        _Rb_tree_node_base* x = _M_root();      // current node
        while (x != 0)
            if (!_M_key_compare(_S_key(x), k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return iterator(y);
    }
    iterator insert_unique(iterator position, const Value& v);   // 0x00991030
};

#pragma auto_inline(off)
// Same-TU stand-in for insert_unique(hint, v) 0x007E9DE0 (reads v, never keeps its address).
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
__declspec(noinline) typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(iterator position, const Value& v)
{
    _M_node_count += *(const unsigned char*)&v.first + *(const unsigned char*)&v.second;
    return position;
}
#pragma auto_inline(on)

template <class Key, class T, class Compare = less<Key>, class Alloc = allocator<pair<const Key, T> > > class map {
public:
    typedef Key key_type;
    typedef pair<const Key, T> value_type;
    typedef _Rb_tree<Key, value_type, _Select1st<value_type>, Compare, Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;

    iterator lower_bound(const key_type& k) { return _M_t.lower_bound(k); }
    iterator end() { return _M_t.end(); }
    Compare key_comp() const { return _M_t.key_comp(); }
    iterator insert(iterator position, const value_type& v) { return _M_t.insert_unique(position, v); }
    T& operator[](const key_type& k)
    {
        iterator i = lower_bound(k);
        // i->first is greater than or equivalent to k.
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, T()));
        return (*i).second;
    }
};

}   // namespace _STL

template class _STL::map<CThing*, float>;

// ---- test harness ----
// The source's insert_unique is the same-TU stand-in (adds the low bytes of the inserted key and value to the node count,
// returns the hint), so the test checks operator[]'s own logic: the lower_bound walk, the end / key_comp test, the hinted
// insert of {k, T()} and the returned mapped slot.

typedef _STL::map<CThing*, float> TMap;
typedef TMap::value_type TValue;
struct STestNode { long color; STestNode* parent; STestNode* left; STestNode* right; TValue value; };
static STestNode* MakeNode(CThing* k)
{
    STestNode* n = (STestNode*)malloc(sizeof(STestNode));
    memset(n, 0, sizeof *n);
    *(CThing**)&n->value.first = k;
    return n;
}
static unsigned long CountOf(const TMap& m) { return *(const unsigned int*)((const char*)&m + sizeof(void*)); }
int main()
{
    int ok = 1;
    STestNode* header = MakeNode((CThing*)0);
    STestNode* n5 = MakeNode((CThing*)5); STestNode* n2 = MakeNode((CThing*)2); STestNode* n9 = MakeNode((CThing*)9);
    header->parent = n5; n5->parent = header;
    n5->left = n2; n5->right = n9; n2->parent = n5; n9->parent = n5;
    static unsigned char raw[sizeof(TMap) + 16];
    memset(raw, 0, sizeof raw);
    TMap& m = *(TMap*)raw;
    *(STestNode**)raw = header;
    *(unsigned int*)(raw + sizeof(void*)) = 3;
    typedef CThing* TKey;
    TKey k5 = (TKey)5, k2 = (TKey)2, k7 = (TKey)7, k10 = (TKey)10;
    if ((void*)&m[k5] != (void*)&n5->value.second || CountOf(m) != 3) ok = 0;
    if ((void*)&m[k2] != (void*)&n2->value.second || CountOf(m) != 3) ok = 0;
    if ((void*)&m[k7] != (void*)&n9->value.second || CountOf(m) != 3 + 7) ok = 0;          // hinted insert before 9, T() low byte 0
    if ((void*)&m[k10] != (void*)&header->value.second || CountOf(m) != 3 + 7 + 10) ok = 0;  // insert at end()
    printf("007ea1a0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}