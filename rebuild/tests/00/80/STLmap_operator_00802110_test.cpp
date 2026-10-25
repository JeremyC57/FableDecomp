// Behaviour test for 0x00802110 (source body copied from src/00802110.cpp; out-of-line callees stubbed below).
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
class CAIStateGroupBase;
enum EIdleStateGroup { IDLE_STATE_GROUP_NONE };

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};

template <class _Value> struct _Rb_tree_node {
    long                _M_color;
    _Rb_tree_node*      _M_parent;               // +0x04
    _Rb_tree_node*      _M_left;                 // +0x08
    _Rb_tree_node*      _M_right;                // +0x0c
    _Value              _M_value_field;          // +0x10
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node<_Value>* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node<_Value>* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
    _Value& operator*() const { return _M_node->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Key, class _Value> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>     _Node;
    typedef _Rb_tree_iterator<_Value> iterator;
    _Node*        _M_header;
    unsigned long _M_node_count;
    less<_Key>    _M_key_compare;                // empty, padded to a dword
    less<_Key> key_comp() const { return _M_key_compare; }
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    iterator lower_bound(const _Key& __k)
    {
        _Node* __y = _M_header;
        _Node* __x = _M_header->_M_parent;
        while (__x != 0)
            if (!(__x->_M_value_field.first < __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return iterator(__y);
    }
    iterator insert_unique(iterator __position, const _Value& __v);   // 0x00801CB0
};
#pragma auto_inline(off)
// Same-TU stand-in for insert_unique(hint, v) 0x00801CB0 (reads v, never keeps its address).
template <class _Key, class _Value>
__declspec(noinline) typename _Rb_tree<_Key, _Value>::iterator
_Rb_tree<_Key, _Value>::insert_unique(iterator __position, const _Value& v)
{
    _M_node_count += *(const unsigned char*)&v.first + *(const unsigned char*)&v.second;
    return __position;
}
#pragma auto_inline(on)


template <class _Key, class _Tp> class map {
public:
    typedef pair<const _Key, _Tp>              value_type;
    typedef _Rb_tree<_Key, value_type>         _Rep_type;
    typedef typename _Rep_type::iterator       iterator;
    _Rep_type _M_t;
    iterator end() { return _M_t.end(); }
    less<_Key> key_comp() const { return _M_t.key_comp(); }
    iterator lower_bound(const _Key& __x) { return _M_t.lower_bound(__x); }
    iterator insert(iterator __position, const value_type& __x) { return _M_t.insert_unique(__position, __x); }
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, _Tp()));
        return (*__i).second;
    }
};
}

template class std::map<EIdleStateGroup, CAIStateGroupBase*>;

// ---- test harness ----
// The source's insert_unique is the same-TU stand-in (adds the low bytes of the inserted key and value to the node count,
// returns the hint), so the test checks operator[]'s own logic: the lower_bound walk, the end / key_comp test, the hinted
// insert of {k, T()} and the returned mapped slot.

typedef std::map<EIdleStateGroup, CAIStateGroupBase*> TMap;
typedef TMap::value_type TValue;
struct STestNode { long color; STestNode* parent; STestNode* left; STestNode* right; TValue value; };
static STestNode* MakeNode(EIdleStateGroup k)
{
    STestNode* n = (STestNode*)malloc(sizeof(STestNode));
    memset(n, 0, sizeof *n);
    *(EIdleStateGroup*)&n->value.first = k;
    return n;
}
static unsigned long CountOf(const TMap& m) { return *(const unsigned int*)((const char*)&m + sizeof(void*)); }
int main()
{
    int ok = 1;
    STestNode* header = MakeNode((EIdleStateGroup)0);
    STestNode* n5 = MakeNode((EIdleStateGroup)5); STestNode* n2 = MakeNode((EIdleStateGroup)2); STestNode* n9 = MakeNode((EIdleStateGroup)9);
    header->parent = n5; n5->parent = header;
    n5->left = n2; n5->right = n9; n2->parent = n5; n9->parent = n5;
    static unsigned char raw[sizeof(TMap) + 16];
    memset(raw, 0, sizeof raw);
    TMap& m = *(TMap*)raw;
    *(STestNode**)raw = header;
    *(unsigned int*)(raw + sizeof(void*)) = 3;
    EIdleStateGroup k5 = (EIdleStateGroup)5, k2 = (EIdleStateGroup)2, k7 = (EIdleStateGroup)7, k10 = (EIdleStateGroup)10;
    if ((void*)&m[k5] != (void*)&n5->value.second || CountOf(m) != 3) ok = 0;
    if ((void*)&m[k2] != (void*)&n2->value.second || CountOf(m) != 3) ok = 0;
    if ((void*)&m[k7] != (void*)&n9->value.second || CountOf(m) != 3 + 7) ok = 0;          // hinted insert before 9, T() low byte 0
    if ((void*)&m[k10] != (void*)&header->value.second || CountOf(m) != 3 + 7 + 10) ok = 0;  // insert at end()
    printf("00802110_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}