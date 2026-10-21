// Behaviour test for 00a0ed80: src/00a0ed80.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. std::__copy_aux(_Deque_iterator<SPair8>, _Deque_iterator<SPair8>, _Deque_iterator<SPair8>, const __false_type&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// std::__copy_aux(_Deque_iterator<SPair8>, _Deque_iterator<SPair8>, _Deque_iterator<SPair8>, const __false_type&) -- STLport copy() step for deque iterators: forward to the random-access __copy with the iterator category and a null distance pointer
// Leaf: __copy_aux
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0ED80, 118 bytes (+ int3 padding) (`ret 30h`).
// Label correction: the catalog calls this CNavigatorAStar::MoveToNavLineOfSightProximity (a propagated name; that PDB signature
//   has 12 arguments and a thiscall `this`). The body is a `__fastcall` template: ecx = the hidden result iterator (returned in
//   eax), edx = the unused `const __false_type&`, three 16-byte deque iterators {cur, first, last, node} by value (0x30 bytes).
//   It is called from the copy() forwarder at 0x00A0EF90 and calls __copy (call target 0x00A0E860),
//   whose distance arithmetic (`sar 3`, 16 elements per node) gives 8-byte elements: another deque of 8-byte elements (a second instantiation; the exact type is not pinned).
// Shape: `__copy(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` -- the empty tag temporary lives in the
//   dead slot of `result` and goes in edx by reference; the three iterator copies are built in the argument area right to left.
//   The tag shares that dead slot only when __copy (the const-ref taker of the tag) has a same-TU noinline stand-in;
//   extern, VC7.1 opens a `push ecx` frame word for it (the "tag over a dead argument slot" lane gap was this escape).
struct SPair8 {                                  // an 8-byte deque element (type not identified)
    long First, Second;
};
namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { };
template <class T> struct _Deque_iterator {        // STLport deque iterator, 16 bytes
    T*  _M_cur;
    T*  _M_first;
    T*  _M_last;
    T** _M_node;
    _Deque_iterator() { }
    _Deque_iterator(const _Deque_iterator& other) : _M_cur(other._M_cur), _M_first(other._M_first), _M_last(other._M_last), _M_node(other._M_node) { }
};
// Same-TU stand-in for __copy (not the retail body).
void CopyRange_00a0e860(void* out, const void* first, const void* last, const void* result);
#pragma auto_inline(off)
template <class T> __declspec(noinline) _Deque_iterator<T> __fastcall __copy(_Deque_iterator<T> first, _Deque_iterator<T> last, _Deque_iterator<T> result,
                                                       const random_access_iterator_tag& tag, int* distance)   // 0x00A0E860 stand-in
{
    _Deque_iterator<T> out;
    CopyRange_00a0e860(&out, &first, &last, &result);
    return out;
}
#pragma auto_inline(on)
template <class T> _Deque_iterator<T> __fastcall __copy_aux(_Deque_iterator<T> first, _Deque_iterator<T> last, _Deque_iterator<T> result,
                                                           const __false_type& trivial)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}
template _Deque_iterator<SPair8> __fastcall __copy_aux(_Deque_iterator<SPair8>, _Deque_iterator<SPair8>, _Deque_iterator<SPair8>, const __false_type&);
}

// ---- stubs and driver ----
static int g_calls = 0;
static unsigned char g_zero[16];
static SPair8 g_buf[64];
static SPair8* g_node[4] = { g_buf, g_buf + 16, g_buf + 32, g_buf + 48 };
static void* g_seen[3][4];
void std::CopyRange_00a0e860(void* out, const void* first, const void* last, const void* result)
{
    ++g_calls;
    memcpy(g_seen[0], first, 16); memcpy(g_seen[1], last, 16); memcpy(g_seen[2], result, 16);
    // copy the range element by element (one node of 16 elements here) and advance the destination
    const std::_Deque_iterator<SPair8>* f = (const std::_Deque_iterator<SPair8>*)first;
    const std::_Deque_iterator<SPair8>* l = (const std::_Deque_iterator<SPair8>*)last;
    std::_Deque_iterator<SPair8>* o = (std::_Deque_iterator<SPair8>*)out;
    const std::_Deque_iterator<SPair8>* r = (const std::_Deque_iterator<SPair8>*)result;
    long n = l->_M_cur - f->_M_cur;
    for (long i = 0; i < n; ++i) r->_M_cur[i] = f->_M_cur[i];
    memcpy(o, r, 16); o->_M_cur += n;
}
static std::_Deque_iterator<SPair8> mk(SPair8* cur, SPair8* first, SPair8* last, SPair8** node)
{ std::_Deque_iterator<SPair8> it; it._M_cur = cur; it._M_first = first; it._M_last = last; it._M_node = node; return it; }
int main()
{
    for (int i = 0; i < 16; ++i) { memset(&g_buf[i], 0, sizeof(SPair8)); *(long*)&g_buf[i] = 100 + i; }
    std::_Deque_iterator<SPair8> first = mk(g_buf + 2, g_buf, g_buf + 16, &g_node[0]);
    std::_Deque_iterator<SPair8> last = mk(g_buf + 6, g_buf, g_buf + 16, &g_node[0]);
    std::_Deque_iterator<SPair8> result = mk(g_buf + 33, g_buf + 32, g_buf + 48, &g_node[2]);
    std::_Deque_iterator<SPair8> r = std::__copy_aux(first, last, result, std::__false_type());
    CHECK(g_calls == 1);
    CHECK(g_seen[0][0] == g_buf + 2 && g_seen[0][1] == g_buf && g_seen[0][2] == g_buf + 16 && g_seen[0][3] == &g_node[0]);
    CHECK(g_seen[1][0] == g_buf + 6 && g_seen[2][0] == g_buf + 33 && g_seen[2][1] == g_buf + 32 && g_seen[2][3] == &g_node[2]);
    CHECK(r._M_cur == g_buf + 37 && r._M_first == g_buf + 32 && r._M_last == g_buf + 48 && r._M_node == &g_node[2]);
    CHECK(*(long*)&g_buf[33] == 102 && *(long*)&g_buf[36] == 105);
    CHECK(first._M_cur == g_buf + 2 && result._M_cur == g_buf + 33);   // by-value arguments untouched
    if (fails == 0) printf("00a0ed80_TEST PASS\n");
    return fails;
}
