// Behaviour test for 0x009E5360 CSymbolMap::RemoveSymbolsOfGroupType: the source body verbatim (comments and CRT declarations dropped), mocks for its callees.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#define std tstd
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
#define private public
class CCharStringData { public: const char* Text; };
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;                   // +0x00
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CSymbolMap;

namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

#pragma auto_inline(off)
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)
{                                                   // 0x009E6600 (same-TU body: the tag is never read)
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                  // +0x00
    _Tp* _M_finish;                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};

template <class _ForwardIter, class _Predicate>
_ForwardIter __fastcall remove_if(_ForwardIter __first, _ForwardIter __last, _Predicate __pred);   // 0x009E7380

template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const; };
template <class _Tp> struct _Identity { const _Tp& operator()(const _Tp& __x) const { return __x; } };
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool _M_color;                                  // +0x00
    _Base_ptr _M_parent;                            // +0x04
    _Base_ptr _M_left;                              // +0x08
    _Base_ptr _M_right;                             // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const iterator& __it) { _M_node = __it._M_node; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    typedef _Rb_tree_iterator<_Value, _Const_traits<_Value> > const_iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                       // +0x04
    _Compare _M_key_compare;                                          // +0x08
    _Base_ptr _M_lower_bound(const _Key& __k) const;                  // 0x004B9D00
    _Base_ptr _M_upper_bound(const _Key& __k) const;                  // 0x004B9D50
    iterator lower_bound(const _Key& __k) { return iterator(_M_lower_bound(__k)); }
    const_iterator lower_bound(const _Key& __k) const { return const_iterator(_M_lower_bound(__k)); }
    iterator upper_bound(const _Key& __k) { return iterator(_M_upper_bound(__k)); }
    const_iterator upper_bound(const _Key& __k) const { return const_iterator(_M_upper_bound(__k)); }
    void erase(iterator __first, iterator __last);                    // 0x004B9C10
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator _Rep_iterator;
    typedef typename _Rep_type::const_iterator iterator;
    _Rep_type _M_t;                                 // 12 bytes
    iterator lower_bound(const _Key& __x) const { return _M_t.lower_bound(__x); }
    iterator upper_bound(const _Key& __x) const { return _M_t.upper_bound(__x); }
    void erase(iterator __first, iterator __last) { _M_t.erase((_Rep_iterator&)__first, (_Rep_iterator&)__last); }
};
}

template <class T> class CCompareString { };
template <class K, class V, class C> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef typename std::vector<value_type>::iterator iterator;
    std::vector<value_type> Data;                   // +0x00 (12-byte STLport vector)
    C    Compare;                                   // +0x0c (empty)
    bool Dirty;                                     // +0x0d
    iterator begin() { return Data.begin(); }
    iterator end() { return Data.end(); }
    iterator erase(iterator first, iterator last) { return Data.erase(first, last); }
};

class CSymbolMap {
public:
    class CSymbolInfo {
    public:
        CCharString GroupName;                      // +0x00
        CCharString EntryName;                      // +0x04
        long        Val;                            // +0x08
    };
    void RemoveSymbolsOfGroupType(const CCharString& group_name);
private:
    CVectorMap<CCharString, CSymbolInfo, CCompareString<CSymbolInfo> > SLONGMap;   // +0x04
    std::set<CCharString> NamesUsed;                // +0x14
    bool CheckForDuplicateSymbols;                  // +0x20
    virtual void v00();
};

class CPred_HasGroupName {
public:
    CPred_HasGroupName(const CCharString& name) : Name(name) { }
    bool operator()(const std::pair<CCharString, CSymbolMap::CSymbolInfo>& entry);
    CCharString Name;                               // +0x00
};

void CSymbolMap::RemoveSymbolsOfGroupType(const CCharString& group_name)
{
    std::pair<CCharString, CSymbolInfo>* new_end = std::remove_if(SLONGMap.begin(), SLONGMap.end(), CPred_HasGroupName(group_name));
    for (std::pair<CCharString, CSymbolInfo>* it = new_end; it != SLONGMap.end(); ++it)
        NamesUsed.erase(NamesUsed.lower_bound(it->second.EntryName), NamesUsed.upper_bound(it->second.EntryName));
    SLONGMap.erase(new_end, SLONGMap.end());
}

static char g_log[512];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { }
CCharString::~CCharString() { }
CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }
void CSymbolMap::v00() { }
bool CPred_HasGroupName::operator()(const tstd::pair<CCharString, CSymbolMap::CSymbolInfo>& entry)
{
    return entry.second.GroupName.PStringData == Name.PStringData;
}
namespace tstd {
template <class _ForwardIter, class _Predicate>
_ForwardIter __fastcall remove_if(_ForwardIter __first, _ForwardIter __last, _Predicate __pred)
{
    _ForwardIter __out = __first;
    for (; __first != __last; ++__first)
        if (!__pred(*__first)) { *__out = *__first; ++__out; }
    return __out;
}
static _Rb_tree_node_base g_nodes[2];
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node_base* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_lower_bound(const _Key& __k) const
{
    char b[64]; sprintf(b, "lower(%s)", __k.PStringData->Text); Log(b); return &g_nodes[0];
}
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
_Rb_tree_node_base* _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_upper_bound(const _Key& __k) const
{
    return __k.PStringData ? &g_nodes[1] : 0;                   // not logged: argument order is unspecified
}
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::erase(iterator __first, iterator __last)
{
    Log(__first._M_node == &g_nodes[0] && __last._M_node == &g_nodes[1] ? "erase" : "erase(bad)");
}
}
typedef tstd::pair<CCharString, CSymbolMap::CSymbolInfo> Entry;
int main()
{
    CCharStringData qa = { "Quest" }, qb = { "Other" }, n0 = { "A" }, n1 = { "B" }, n2 = { "C" }, n3 = { "D" };
    CCharStringData* groups[4] = { &qa, &qb, &qa, &qb };
    CCharStringData* names[4] = { &n0, &n1, &n2, &n3 };
    CSymbolMap* map = (CSymbolMap*)malloc(sizeof(CSymbolMap)); memset(map, 0, sizeof(CSymbolMap));
    Entry* e = (Entry*)malloc(4 * sizeof(Entry)); memset(e, 0, 4 * sizeof(Entry));
    for (int i = 0; i < 4; ++i) { e[i].second.GroupName.PStringData = groups[i]; e[i].second.EntryName.PStringData = names[i]; e[i].second.Val = i; }
    map->SLONGMap.Data._M_start = e; map->SLONGMap.Data._M_finish = e + 4; map->SLONGMap.Data._M_end_of_storage._M_data = e + 4;
    CCharString* group = (CCharString*)malloc(sizeof(CCharString)); group->PStringData = &qa;
    map->RemoveSymbolsOfGroupType(*group);
    int ok = map->SLONGMap.Data._M_finish == e + 2 && e[0].second.Val == 1 && e[1].second.Val == 3;
    ok &= strcmp(g_log, "lower(C);erase;lower(D);erase;") == 0;   // the tail left by remove_if holds the old C and D entries
    if (!ok) printf("log %s\n", g_log);
    if (ok) printf("009e5360_TEST PASS\n");
    return ok ? 0 : 1;
}