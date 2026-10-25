// Behaviour test for 0x009B7BC0 (source body copied from src/009b7bc0.cpp; out-of-line callees stubbed below).
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
class CDefClassBase {                                  // counted def object: vptr, reference count +4
public:
    virtual ~CDefClassBase();
    long RefCount;                                      // +0x04
};
enum ENullDef { NULL_DEF };

template <class T> class CDefPointer {                  // 4 bytes: the counted def object
public:
    CDefPointer(ENullDef = NULL_DEF) : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    T* Object;
};

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) {}
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Key, class _Value, class _Compare> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value> iterator;
    _Rb_tree_node_base* _M_root() const { return _M_header->_M_parent; }
    static const _Key& _S_key(_Rb_tree_node_base* __x) { return ((_Rb_tree_node<_Value>*)__x)->_M_value_field.first; }
    _Compare key_comp() const { return _M_key_compare; }
    iterator end() { return iterator(_M_header); }
    _Rb_tree_node_base* _M_lower_bound(const _Key& __k) const
    {
        _Rb_tree_node_base* __y = _M_header;
        _Rb_tree_node_base* __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return __y;
    }
    iterator lower_bound(const _Key& __k) { return iterator(_M_lower_bound(__k)); }
    iterator insert_unique(iterator __position, const _Value& __v);   // 0x009B6580
public:
    _Rb_tree_node_base* _M_header;                      // +0x00
    unsigned int        _M_node_count;                  // +0x04
    _Compare            _M_key_compare;                 // +0x08
};
#pragma auto_inline(off)
// Same-TU stand-in for insert_unique(hint, v) 0x009B6580 (reads v, never keeps its address).
template <class _Key, class _Value, class _Compare>
__declspec(noinline) typename _Rb_tree<_Key, _Value, _Compare>::iterator
_Rb_tree<_Key, _Value, _Compare>::insert_unique(iterator __position, const _Value& v)
{
    _M_node_count += *(const unsigned char*)&v.first + *(const unsigned char*)&v.second;
    return __position;
}
#pragma auto_inline(on)


template <class _Key, class _Tp, class _Compare = less<_Key> > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Compare> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    iterator end() { return _M_t.end(); }
    _Compare key_comp() const { return _M_t.key_comp(); }
    iterator lower_bound(const _Key& __x) { return _M_t.lower_bound(__x); }
    iterator insert(iterator __position, const value_type& __x) { return _M_t.insert_unique(__position, __x); }
    _Tp& operator[](const _Key& __k);
public:
    _Rep_type _M_t;
};

template <class _Key, class _Tp, class _Compare> _Tp& map<_Key, _Tp, _Compare>::operator[](const _Key& __k)
{
    iterator __i = lower_bound(__k);
    if (__i == end() || key_comp()(__k, (*__i).first))
        __i = insert(__i, value_type(__k, _Tp()));
    return (*__i).second;
}

template class map<unsigned long, CDefPointer<CDefClassBase> >;
}

// ---- test harness ----
// The source's insert_unique is the same-TU stand-in (adds the low bytes of the inserted key and value to the node count,
// returns the hint), so the test checks operator[]'s own logic: the lower_bound walk, the end / key_comp test, the hinted
// insert of {k, T()} and the returned mapped slot.
CDefClassBase::~CDefClassBase() { }
typedef std::map<unsigned long, CDefPointer<CDefClassBase> > TMap;
typedef TMap::value_type TValue;
struct STestNode { long color; STestNode* parent; STestNode* left; STestNode* right; TValue value; };
static STestNode* MakeNode(unsigned long k)
{
    STestNode* n = (STestNode*)malloc(sizeof(STestNode));
    memset(n, 0, sizeof *n);
    *(unsigned long*)&n->value.first = k;
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
    unsigned long k5 = 5, k2 = 2, k7 = 7, k10 = 10;
    if ((void*)&m[k5] != (void*)&n5->value.second || CountOf(m) != 3) ok = 0;
    if ((void*)&m[k2] != (void*)&n2->value.second || CountOf(m) != 3) ok = 0;
    if ((void*)&m[k7] != (void*)&n9->value.second || CountOf(m) != 3 + 7) ok = 0;          // hinted insert before 9, T() low byte 0
    if ((void*)&m[k10] != (void*)&header->value.second || CountOf(m) != 3 + 7 + 10) ok = 0;  // insert at end()
    printf("009b7bc0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}