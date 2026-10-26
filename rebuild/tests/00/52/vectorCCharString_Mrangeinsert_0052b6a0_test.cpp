#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#endif
// vector<CCharString>::_M_range_insert(iterator pos, set<CCharString>::const_iterator first, last, const
// forward_iterator_tag&) -- (manifest: "CCombatWheel::Insert") STLport range insert from a tree range: insert in place
// when capacity allows (two layouts by the elements after pos), else reallocate to old size + max(old size, n)
// Leaf: CCombatWheel::Insert
// Callees (STLport helpers instantiated out of line): __distance 0x00528040 (cdecl), __advance 0x00528090 (cdecl),
//   __uninitialized_copy ptr 0x0044B120 / tree 0x00527FC0 (__fastcall), __copy_backward_ptrs 0x0049B180, __copy tree
//   0x00528140, malloc 0x00BFEA0E, the vector's clear-and-free 0x00433040.
// Tried: value-initialised tags (a), tags with empty constructors (b, this), STLport inline wrappers taking iterators by value (c).
#include <stdlib.h>
#include <string.h>
class CCharString { public: void* PStringData; };
struct forward_iterator_tag { forward_iterator_tag() { } };
struct __false_type { __false_type() { } };
struct STreeNode;
struct CTreeIterator { STreeNode* _M_node; };
void __fastcall ConstructString(CCharString* place, const CCharString& source);   // CCharString copy ctor 0x0099EC30
void __fastcall AssignString(CCharString* target, const CCharString& source);       // CCharString::operator=
struct STreeNode { long _M_color; STreeNode* _M_parent; STreeNode* _M_left; STreeNode* _M_right; };
// _Rb_global<bool>::_M_increment (an STLport template, so instantiated in this TU as well)
template <class Dummy> struct _Rb_global {
    static __declspec(noinline) STreeNode* __fastcall _M_increment(STreeNode* node)
    {
        if (node->_M_right != 0) {
            node = node->_M_right;
            while (node->_M_left != 0)
                node = node->_M_left;
        } else {
            STreeNode* parent = node->_M_parent;
            while (node == parent->_M_right) {
                node = parent;
                parent = parent->_M_parent;
            }
            if (node->_M_right != parent)
                node = parent;
        }
        return node;
    }
};
#define _Rb_tree_increment _Rb_global<bool>::_M_increment
// Same-TU template bodies of the STLport helpers (retail instantiated them in this TU, out of line). VC7.1 sees that the
// tag parameters are never read, so the empty tag temporaries no longer pin their own frame slots.
template <class It, class Tag> __declspec(noinline) long __cdecl __distance(const It& first, const It& last, const Tag&)
{
    long n = 0;
    It it = first;
    while (it._M_node != last._M_node) {
        it._M_node = _Rb_tree_increment(it._M_node);
        ++n;
    }
    return n;
}
template <class It, class Tag> __declspec(noinline) void __cdecl __advance(It& i, long n, const Tag&)
{
    while (n--)
        i._M_node = _Rb_tree_increment(i._M_node);
}
template <class Tag> __declspec(noinline) CCharString* __fastcall __uninitialized_copy(CCharString* first, CCharString* last, CCharString* result, const Tag&)
{
    for (; first != last; ++first, ++result)
        ConstructString(result, *first);
    return result;
}
template <class Tag> __declspec(noinline) CCharString* __fastcall __uninitialized_copy(CTreeIterator first, CTreeIterator last, CCharString* result, const Tag&)
{
    for (; first._M_node != last._M_node; first._M_node = _Rb_tree_increment(first._M_node), ++result)
        ConstructString(result, *(const CCharString*)((char*)first._M_node + sizeof(STreeNode)));
    return result;
}
template <class Tag> __declspec(noinline) CCharString* __fastcall __copy_backward_ptrs(CCharString* first, CCharString* last, CCharString* result, const Tag&)
{
    while (last != first)
        AssignString(--result, *--last);
    return result;
}
template <class Tag> __declspec(noinline) CCharString* __fastcall __copy(CTreeIterator first, CTreeIterator last, CCharString* result, const Tag&, long*)
{
    for (; first._M_node != last._M_node; first._M_node = _Rb_tree_increment(first._M_node), ++result)
        AssignString(result, *(const CCharString*)((char*)first._M_node + sizeof(STreeNode)));
    return result;
}
template<class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

struct CStringVector {                                // STLport vector<CCharString>
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    void _M_clear();                                  // 0x00433040 (destroy the elements, free the block)
    unsigned int size() const { return _M_finish - _M_start; }
    void _M_range_insert(CCharString* position, CTreeIterator first, CTreeIterator last, const forward_iterator_tag&);
};

void CStringVector::_M_range_insert(CCharString* position, CTreeIterator first, CTreeIterator last, const forward_iterator_tag&)
{
    if (first._M_node != last._M_node) {
        unsigned int n = __distance(first, last, forward_iterator_tag());
        if ((unsigned int)(_M_end_of_storage - _M_finish) >= n) {
            const unsigned int elems_after = _M_finish - position;
            CCharString* old_finish = _M_finish;
            if (elems_after > n) {
                __uninitialized_copy(_M_finish - n, _M_finish, _M_finish, __false_type());
                _M_finish += n;
                __copy_backward_ptrs(position, old_finish - n, old_finish, __false_type());
                __copy(first, last, position, forward_iterator_tag(), (long*)0);
            } else {
                CTreeIterator mid = first;
                __advance(mid, elems_after, forward_iterator_tag());
                __uninitialized_copy(mid, last, _M_finish, __false_type());
                _M_finish += n - elems_after;
                __uninitialized_copy(position, old_finish, _M_finish, __false_type());
                _M_finish += elems_after;
                __copy(first, mid, position, forward_iterator_tag(), (long*)0);
            }
        } else {
            const unsigned int old_size = size();
            const unsigned int len = old_size + (max)(old_size, n);
            CCharString* new_start = len ? (CCharString*)malloc(len * sizeof(CCharString)) : 0;
            CCharString* new_finish = new_start;
            new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
            new_finish = __uninitialized_copy(first, last, new_finish, __false_type());
            new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
            _M_clear();
            _M_start = new_start;
            _M_finish = new_finish;
            _M_end_of_storage = new_start + len;
        }
    }
}

// ---- behaviour test stubs ----
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d\n", __LINE__); g_fail = 1; } } while (0)
static int g_constructs = 0, g_assigns = 0, g_clears = 0;
void __fastcall ConstructString(CCharString* place, const CCharString& source) { place->PStringData = source.PStringData; ++g_constructs; }
void __fastcall AssignString(CCharString* target, const CCharString& source) { target->PStringData = source.PStringData; ++g_assigns; }
void CStringVector::_M_clear() { ++g_clears; free(_M_start); }
struct SNode { STreeNode n; CCharString value; };
static SNode g_header, g_nodes[3];
static void BuildTree(long a, long b, long c)    // right-leaning chain a -> b -> c under the header
{
    memset(&g_header, 0, sizeof(g_header)); memset(g_nodes, 0, sizeof(g_nodes));
    STreeNode* h = &g_header.n; STreeNode* x = &g_nodes[0].n; STreeNode* y = &g_nodes[1].n; STreeNode* z = &g_nodes[2].n;
    h->_M_parent = x; h->_M_left = x; h->_M_right = z;
    x->_M_parent = h; x->_M_right = y; y->_M_parent = x; y->_M_right = z; z->_M_parent = y;
    g_nodes[0].value.PStringData = (void*)a; g_nodes[1].value.PStringData = (void*)b; g_nodes[2].value.PStringData = (void*)c;
}
static void MakeVec(CStringVector& v, const long* d, int n, int cap)
{
    v._M_start = (CCharString*)malloc(cap * sizeof(CCharString));
    for (int i = 0; i < n; ++i) v._M_start[i].PStringData = (void*)d[i];
    v._M_finish = v._M_start + n; v._M_end_of_storage = v._M_start + cap;
}
static bool Equals(const CStringVector& v, const long* d, int n)
{
    if (v._M_finish - v._M_start != n) return false;
    for (int i = 0; i < n; ++i) if (v._M_start[i].PStringData != (void*)d[i]) return false;
    return true;
}
int main()
{
    forward_iterator_tag tag; CStringVector v; CTreeIterator first, mid, last;
    BuildTree(10, 11, 12);
    first._M_node = &g_nodes[0].n; mid._M_node = &g_nodes[2].n; last._M_node = &g_header.n;
    long a[4] = { 1, 2, 3, 4 };
    // empty range
    MakeVec(v, a, 4, 8); v._M_range_insert(v._M_start + 1, first, first, tag);
    CHECK(Equals(v, a, 4) && g_constructs == 0 && g_assigns == 0);
    // in place, more elements after pos than inserted: [1,10,11,2,3,4]
    v._M_range_insert(v._M_start + 1, first, mid, tag);
    { long e[6] = { 1, 10, 11, 2, 3, 4 }; CHECK(Equals(v, e, 6)); CHECK(v._M_end_of_storage - v._M_start == 8); }
    // in place, fewer elements after pos: [1,2] + 3 at pos 1 -> [1,10,11,12,2]
    free(v._M_start); MakeVec(v, a, 2, 8); v._M_range_insert(v._M_start + 1, first, last, tag);
    { long e[5] = { 1, 10, 11, 12, 2 }; CHECK(Equals(v, e, 5)); CHECK(g_clears == 0); }
    // reallocate: [1,2] cap 2 + 3 at end -> [1,2,10,11,12], capacity 2 + max(2,3) = 5
    free(v._M_start); MakeVec(v, a, 2, 2); v._M_range_insert(v._M_finish, first, last, tag);
    { long e[5] = { 1, 2, 10, 11, 12 }; CHECK(Equals(v, e, 5)); CHECK(g_clears == 1); CHECK(v._M_end_of_storage - v._M_start == 5); }
    if (g_fail) { printf("0052b6a0_TEST FAIL\n"); return 1; }
    printf("0052b6a0_TEST PASS\n");
    return 0;
}