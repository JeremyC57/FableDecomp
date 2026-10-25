// Behaviour test for std::list<pair<CWideString,long> >::push_back (0x005D86B2): forwards to insert(end(), x) with the
// caller's element reference. The source is copied verbatim; insert is a recording stub.
#include <stdio.h>
static const void* g_x = 0; static const void* g_pos = 0; static int g_calls = 0;
// ---- source (verbatim) ----
// std::list<std::pair<CWideString,long> >::push_back(const value_type& x) -- STLport: insert(end(), x), the insert out of line
// Leaf: ?push_back@?$list@U?$pair@VCWideString@@J@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D86B2, 26 bytes, thiscall `ret 4`; frame-pointer size TU.
// Identity: the sole caller is the landed CTCInventoryStats::ConvertMap 0x005CD6B7, whose Ego_r.pdb / FableWin.pdb
//   signature is ConvertMap(const map<unsigned long,long>&, list<pair<CWideString,long> >&): it appends to that list.
//   FableWin.pdb / Ego_r.pdb carry list<pair<CWideString,long> > instantiations (push_back among them). The callee
//   0x005D8626 is the list's insert(iterator, const value_type&). The earlier generated ListPushFrontWrapper_005d86b2 /
//   catalog "Lua_CreateClosure" labels were propagated.
// Shape: `end()` is the sentinel node word (`mov edx,[ecx]`); the iterator has a user copy constructor, so the by-value
//   argument is copy-built in its stack slot (`push ecx ; mov eax,esp ; mov [eax],edx`); the hidden iterator result
//   reuses the dead `x` argument slot (`lea eax,[ebp+8]`).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CWideString;

namespace std {
template <class _Tp> class allocator;
template <class _T1, class _T2> struct pair;
struct _List_node_base;

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    __forceinline _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;                   // +0x00 the sentinel node
    iterator end() { return iterator(_M_node); }
    iterator insert(iterator __position, const _Tp& __x);   // 0x005D8626 for pair<CWideString,long>
    void push_back(const _Tp& __x) { insert(end(), __x); }
};

template void list<pair<CWideString, long> >::push_back(const pair<CWideString, long>& __x);
}

// ---- stubs ----
class CWideString { public: void* p; };
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
typedef list<pair<CWideString, long> > TList;
template <> TList::iterator TList::insert(iterator __position, const pair<CWideString, long>& __x)
{ ++g_calls; g_pos = __position._M_node; g_x = &__x; return __position; }
}
int main()
{
    static unsigned char sentinel[16];
    std::TList l; l._M_node = (std::_List_node_base*)sentinel;
    std::pair<CWideString, long> v; v.first.p = 0; v.second = 5;
    l.push_back(v);
    int ok = g_calls == 1 && g_x == &v && g_pos == sentinel;
    printf(ok ? "005d86b2_TEST PASS\n" : "005d86b2_TEST FAIL\n");
    return ok ? 0 : 1;
}
