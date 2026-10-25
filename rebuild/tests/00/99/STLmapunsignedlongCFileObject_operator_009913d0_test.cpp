// Behaviour test for 0x009913D0 (source body copied from src/009913d0.cpp; out-of-line callees stubbed below).
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
#endif
#include <stddef.h>

class CThreadedFileManager {
public:
    class CFileObject;
};

namespace std {

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

template <class Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) { }
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
// Same-TU stand-in for insert_unique(hint, v) 0x00991030 (reads v, never keeps its address).
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
__declspec(noinline) typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(iterator position, const Value& v)
{
    if (v.second == 0)
        ++_M_node_count;
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

}   // namespace std

template class std::map<unsigned long, CThreadedFileManager::CFileObject*>;

// ---- test harness ----
// The source's insert_unique is the same-TU stand-in (counts the insert, returns the hint), so the test checks
// operator[]'s own logic: lower_bound walk, the end / key_comp test, the hinted insert of {k, 0} and the returned slot.
typedef std::map<unsigned long, CThreadedFileManager::CFileObject*> TMap;
typedef std::_Rb_tree_node<TMap::value_type> TNode;
static TNode* MakeNode(unsigned long k)
{
    TNode* n = (TNode*)malloc(sizeof(TNode));
    memset(n, 0, sizeof *n);
    *(unsigned long*)&n->_M_value_field.first = k;
    n->_M_value_field.second = (CThreadedFileManager::CFileObject*)(k * 16);
    return n;
}
int main()
{
    int ok = 1;
    TNode* header = MakeNode(0);
    TNode* n5 = MakeNode(5); TNode* n2 = MakeNode(2); TNode* n9 = MakeNode(9);
    header->_M_parent = n5; n5->_M_parent = header;
    n5->_M_left = n2; n5->_M_right = n9; n2->_M_parent = n5; n9->_M_parent = n5;
    TMap m; m._M_t._M_header = header; m._M_t._M_node_count = 3;
    unsigned long k5 = 5, k2 = 2, k7 = 7, k10 = 10;
    if (&m[k5] != &n5->_M_value_field.second || m._M_t._M_node_count != 3) ok = 0;
    if (&m[k2] != &n2->_M_value_field.second || m._M_t._M_node_count != 3) ok = 0;
    if (&m[k7] != &n9->_M_value_field.second || m._M_t._M_node_count != 4) ok = 0;      // hinted insert before 9
    if (&m[k10] != &header->_M_value_field.second || m._M_t._M_node_count != 5) ok = 0;  // insert at end()
    printf("009913d0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}