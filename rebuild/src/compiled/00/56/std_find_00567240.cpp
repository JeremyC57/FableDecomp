// std::__find(list<CWideString>::iterator first, list<CWideString>::iterator last, const CWideString& val, const input_iterator_tag&) -- STLport input-iterator find over a list of wide strings
// Leaf: ??$__find
// Retail: Fable.exe (TLC, VC7.1) @ 0x00567240, 91 bytes, /Gr `__fastcall` returning the iterator class through
//   the hidden pointer (ecx), `val` in edx, first / last / the tag on the stack (`ret 0Ch`). Callers: the
//   `find(first, last, val)` front end 0x00567A40 (landed; it builds both iterators with the user copy ctor and
//   passes the tag temporary) and the scoreboard code at 0x005642FA. The catalog label "DataList_FindEqualNode"
//   is descriptive.
// Callees: __char_traits_base<wchar_t, wint_t>::compare @ 0x00403C60 (wchar_t == wint_t == unsigned short) (landed as MemCmp_Unsigned16: the
//   out-of-line static traits compare, `__cdecl`, tested against 0).
// Layout: STLport list node {_M_next, _M_prev, _M_data}; CWideString = one CWideStringData* whose first 12
//   bytes are an STLport basic_string<unsigned short> (start, finish, end of storage), NoRefs at +0xC.
// Shape: `while (first != last && !(*first == val)) ++first; return first;` with CWideString::operator==
//   inline in the landed form (0x00639BA0): same block -> equal, either null -> unequal, other's begin read
//   before ours, lengths, then `if (compare(..) != 0) return false; return true;` (a `== 0` return adds
//   neg/sbb). The traits compare's read-only body sits in the TU (`__declspec(noinline)`), which is what
//   lets VC7.1 keep val's block pointer in esi across the call; `last` stays in its stack slot.
namespace std {

struct input_iterator_tag { };

template <class _CharT, class _IntT> struct __char_traits_base {   // STLport 4.6 char_traits.h; wint_t == unsigned short
    static bool eq(const _CharT& __c1, const _CharT& __c2) { return __c1 == __c2; }
    static bool lt(const _CharT& __c1, const _CharT& __c2) { return __c1 < __c2; }
    static __declspec(noinline) int __cdecl compare(const _CharT* __s1, const _CharT* __s2,
                                                    unsigned int __n)        // 0x00403C60
    {
        for (unsigned int __i = 0; __i < __n; ++__i)
            if (!eq(__s1[__i], __s2[__i]))
                return lt(__s1[__i], __s2[__i]) ? -1 : 1;
        return 0;
    }
};

class basic_string_ushort {                    // STLport basic_string<unsigned short>
public:
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
};

}

class CWideStringData { public:
    std::basic_string_ushort Data;             // +0x00
    long NoRefs;                               // +0x0C
};

class CWideString {
public:
    CWideStringData* PStringData;
    bool operator==(const CWideString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        const unsigned short* otherBegin = other.PStringData->Data._M_start;
        const unsigned short* begin = PStringData->Data._M_start;
        unsigned int length = PStringData->Data._M_finish - begin;
        if (length != (unsigned int)(other.PStringData->Data._M_finish - otherBegin))
            return false;
        if (std::__char_traits_base<unsigned short, unsigned short>::compare(begin, otherBegin, length) != 0)
            return false;
        return true;
    }
};

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node<_Tp>* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};

template <class _InputIter, class _Tp>
_InputIter __fastcall __find(_InputIter __first, _InputIter __last, const _Tp& __val, const input_iterator_tag&)
{
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}

template _List_iterator<CWideString> __fastcall __find<_List_iterator<CWideString>, CWideString>(
    _List_iterator<CWideString>, _List_iterator<CWideString>, const CWideString&, const input_iterator_tag&);

}