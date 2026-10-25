// Behaviour test for 0x00A2D810 (source body copied from src/00a2d810.cpp; out-of-line callees stubbed below).
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
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

struct SCountBlock {
    long  RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4, Data in ecx
    void* Data;                                         // +8
};

template <class T> class CCountedPointer {
public:
    T*           Data;                                  // +0
    SCountBlock* Info;                                  // +4
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        SCountBlock* info = Info;
        if (info && --info->RefCount == 0) {
            info->DeleteFunc(info->Data);
            delete info;
        }
    }
};

class C3DAnimation2;

namespace std {

struct _Rb_tree_node_base {
    unsigned char       _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template <class T> class allocator { };

template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    typedef K key_type;
    typedef pair<const K, T> value_type;
    struct _Node : _Rb_tree_node_base {
        value_type _M_value_field;                      // +0x10 key, +0x14 mapped
    };
    struct iterator {
        _Rb_tree_node_base* _M_node;
        iterator(_Rb_tree_node_base* n) : _M_node(n) { }
        iterator(const iterator& o) : _M_node(o._M_node) { }
        value_type& operator*() const { return static_cast<_Node*>(_M_node)->_M_value_field; }
        bool operator==(const iterator& o) const { return _M_node == o._M_node; }
    };
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    C                   _M_key_compare;

    static const key_type& _S_key(_Rb_tree_node_base* x) { return static_cast<_Node*>(x)->_M_value_field.first; }
    _Rb_tree_node_base* _M_lower_bound(const key_type& k) const
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
    iterator lower_bound(const key_type& k) { return iterator(_M_lower_bound(k)); }
    iterator end() { return iterator(_M_header); }
    C key_comp() const { return _M_key_compare; }
    iterator insert_unique(iterator position, const value_type& v);   // 0x00A2CCF0
    iterator insert(iterator position, const value_type& v) { return insert_unique(position, v); }

    T& operator[](const key_type& k)
    {
        iterator i = lower_bound(k);
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, T()));
        return (*i).second;
    }
};
#pragma auto_inline(off)
// Same-TU stand-in for insert_unique(hint, v) 0x00A2CCF0: reads v and MAY write its counted pointer (so VC7.1 keeps the
// pair's release after the call), never keeps the address.
template <class K, class T, class C, class A>
__declspec(noinline) typename map<K, T, C, A>::iterator map<K, T, C, A>::insert_unique(iterator position, const value_type& v)
{
    _M_node_count += *(const unsigned char*)&v.first;
    if (position._M_node == 0)                          // never (the hint is a node or the header): a possible write to v
        const_cast<T&>(v.second) = T();
    return position;
}
#pragma auto_inline(on)


}

template CCountedPointer<C3DAnimation2>& std::map<unsigned long, CCountedPointer<C3DAnimation2> >::operator[](const unsigned long& k);

// ---- test harness ----
// The source's insert_unique is the same-TU stand-in (adds the key's low byte to the node count, returns the hint), so
// the test checks operator[]'s own logic: the lower_bound walk, the end / key_comp test, the hinted insert of {k, T()}
// (whose empty counted pointer is released without touching a block) and the returned mapped slot.
class C3DAnimation2 { };
typedef std::map<unsigned long, CCountedPointer<C3DAnimation2> > TMap;
typedef TMap::_Node TNode;
static TNode* MakeNode(unsigned long k)
{
    TNode* n = (TNode*)malloc(sizeof(TNode));
    memset(n, 0, sizeof *n);
    *(unsigned long*)&n->_M_value_field.first = k;
    return n;
}
int main()
{
    int ok = 1;
    TNode* header = MakeNode(0);
    TNode* n5 = MakeNode(5); TNode* n2 = MakeNode(2); TNode* n9 = MakeNode(9);
    header->_M_parent = n5; n5->_M_parent = header;
    n5->_M_left = n2; n5->_M_right = n9; n2->_M_parent = n5; n9->_M_parent = n5;
    static unsigned char raw[sizeof(TMap) + 16];
    memset(raw, 0, sizeof raw);
    TMap& m = *(TMap*)raw;
    m._M_header = header;
    m._M_node_count = 3;
    unsigned long k5 = 5, k2 = 2, k7 = 7, k10 = 10;
    if (&m[k5] != &n5->_M_value_field.second || m._M_node_count != 3) ok = 0;
    if (&m[k2] != &n2->_M_value_field.second || m._M_node_count != 3) ok = 0;
    if (&m[k7] != &n9->_M_value_field.second || m._M_node_count != 3 + 7) ok = 0;            // hinted insert before 9
    if (&m[k10] != &header->_M_value_field.second || m._M_node_count != 3 + 7 + 10) ok = 0;  // insert at end()
    if (n9->_M_value_field.second.Info != 0 || header->_M_value_field.second.Data != 0) ok = 0;
    printf("00a2d810_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}