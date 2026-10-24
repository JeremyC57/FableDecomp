// Behaviour test for 0x00431606 _Rb_tree<CCharString,...>::insert_unique (body copied from the source; callees stubbed).
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#define __stdcall
#endif
long __fastcall Compare(const char* a, const char* b);  // 0x00411570

template <class C> struct CBasicString {
    C* PData;                                            // +0x00
    __forceinline bool operator<(const CBasicString& other) const { return Compare(PData, other.PData) < 0; }
};

template <class T> struct CComparePointer {
    __forceinline bool operator()(const T* a, const T* b) const
    {
        bool result;
        if (b == a)
            goto isFalse;
        if (!a)
            result = true;
        else if (!b) {
        isFalse:
            result = false;
        }
        else
            result = *a < *b;
        return result;
    }
};

namespace std {
struct _Rb_tree_node {
    char           _M_color;                             // +0x00
    _Rb_tree_node* _M_parent;                            // +0x04
    _Rb_tree_node* _M_left;                              // +0x08
    _Rb_tree_node* _M_right;                             // +0x0C
    CBasicString<char>* _M_value_field;                  // +0x10
};
struct _Rb_global_inst {
    static _Rb_tree_node* __cdecl _M_decrement(_Rb_tree_node* x);   // 0x004294E2
};
struct _Rb_tree_iterator {
    _Rb_tree_node* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node* x) : _M_node(x) {}
    void _M_decrement() { _M_node = _Rb_global_inst::_M_decrement(_M_node); }
};
template <class A, class B> struct pair {
    A first;
    B second;
    pair(const A& a, const B& b) : first(a), second(b) {}
};
struct _Rb_tree {
    typedef CBasicString<char>* value_type;
    _Rb_tree_node*                       _M_header;      // +0x00
    unsigned long                        _M_node_count;  // +0x04
    CComparePointer<CBasicString<char> > _M_key_compare;
    _Rb_tree_iterator _M_insert(_Rb_tree_node* x, _Rb_tree_node* y, const value_type& v, _Rb_tree_node* w);   // 0x004316CF
    pair<_Rb_tree_iterator, bool> insert_unique(const value_type& v);
};

pair<_Rb_tree_iterator, bool> _Rb_tree::insert_unique(const value_type& v)
{
    _Rb_tree_node* y = _M_header;
    _Rb_tree_node* x = _M_header->_M_parent;
    bool comp = true;
    while (x != 0) {
        y = x;
        comp = _M_key_compare(v, x->_M_value_field);
        x = comp ? x->_M_left : x->_M_right;
    }
    _Rb_tree_iterator j(y);
    if (comp) {
        if (j._M_node == _M_header->_M_left)
            return pair<_Rb_tree_iterator, bool>(_M_insert(y, y, v, 0), true);
        else
            j._M_decrement();
    }
    if (_M_key_compare(j._M_node->_M_value_field, v))
        return pair<_Rb_tree_iterator, bool>(_M_insert(x, y, v, 0), true);
    return pair<_Rb_tree_iterator, bool>(j, false);
}
}

long __fastcall Compare(const char* a, const char* b) { return strcmp(a, b); }
namespace std {
static _Rb_tree_node* g_pred_of = 0; static _Rb_tree_node* g_pred = 0; static int g_dec = 0;
_Rb_tree_node* __cdecl _Rb_global_inst::_M_decrement(_Rb_tree_node* x) { ++g_dec; return x == g_pred_of ? g_pred : 0; }
static _Rb_tree_node* g_ix; static _Rb_tree_node* g_iy; static int g_ins = 0; static _Rb_tree_node g_new;
_Rb_tree_iterator _Rb_tree::_M_insert(_Rb_tree_node* x, _Rb_tree_node* y, const value_type& v, _Rb_tree_node* w)
{
    ++g_ins; g_ix = x; g_iy = y;
    g_new._M_value_field = v;
    return _Rb_tree_iterator(&g_new);
}
}
using namespace std;
static int fail(int n) { printf("00431606_TEST FAIL %d\n", n); return 1; }
int main()
{
    char sb[] = "b", sd[] = "d", sc[] = "c", sb2[] = "b", sa[] = "a";
    CBasicString<char> kb = { sb }, kd = { sd }, kc = { sc }, kb2 = { sb2 }, ka = { sa };
    _Rb_tree_node header, nb, nd;
    memset(&header, 0, sizeof header); memset(&nb, 0, sizeof nb); memset(&nd, 0, sizeof nd);
    nb._M_value_field = &kb; nd._M_value_field = &kd;
    header._M_parent = &nb; header._M_left = &nb; header._M_right = &nd;
    nb._M_right = &nd; nd._M_parent = &nb; nb._M_parent = &header;
    _Rb_tree t; t._M_header = &header; t._M_node_count = 2;
    g_pred_of = &nd; g_pred = &nb;
    // "c": lands left of d, the predecessor b is smaller -> inserted under d
    CBasicString<char>* v = &kc;
    pair<_Rb_tree_iterator, bool> r = t.insert_unique(v);
    if (!r.second || r.first._M_node != &g_new || g_ins != 1 || g_ix != 0 || g_iy != &nd || g_dec != 1) return fail(1);
    // an equal string ("b" in another block) is found, not inserted
    v = &kb2;
    r = t.insert_unique(v);
    if (r.second || r.first._M_node != &nb || g_ins != 1 || g_dec != 2) return fail(2);
    // "a" goes left of the leftmost b: inserted at once (no decrement)
    v = &ka;
    r = t.insert_unique(v);
    if (!r.second || g_ins != 2 || g_ix != &nb || g_iy != &nb || g_dec != 2) return fail(3);
    // empty tree: inserted under the header
    _Rb_tree_node h2; memset(&h2, 0, sizeof h2); h2._M_left = &h2; h2._M_right = &h2;
    _Rb_tree t2; t2._M_header = &h2; t2._M_node_count = 0;
    r = t2.insert_unique(v);
    if (!r.second || g_ins != 3 || g_iy != &h2 || g_ix != &h2) return fail(4);
    printf("00431606_TEST PASS\n");
    return 0;
}