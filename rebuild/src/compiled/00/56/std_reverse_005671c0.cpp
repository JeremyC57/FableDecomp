// std::__reverse(pair<long, CWideString>* first, pair<long, CWideString>* last, const random_access_iterator_tag&) -- STLport random-access reverse: swap the ends inward
// Leaf: ??$__reverse
// Retail: Fable.exe (TLC, VC7.1) @ 0x005671C0, 113 bytes, /Gr `__fastcall` (first in ecx, last in edx, the tag
//   by reference on the stack: `ret 4`). Callers: the `reverse(first, last)` front end 0x00567A30 (`lea eax,
//   [esp+3] ; push eax` = the tag temporary) and the scoreboard GUI code at 0x0056386E. The catalog label
//   "WideStringPair_SwapRange" is descriptive.
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720, CWideString::operator=(const CWideString&)
//   @ 0x0099B7D0, CWideString::~CWideString() @ 0x0099B510.
// Layout: 8-byte elements {4-byte integer copied as a dword, CWideString at +4}: an STLport pair; the 8-byte
//   pair temporary gives the `and esp,-8` frame.
// Shape: STLport 4.x `for (; first < last; ++first) iter_swap(first, --last);` with iter_swap ->
//   `_Tp tmp = *a; *a = *b; *b = tmp;` (pair copy ctor = memberwise, assignment = implicit memberwise).
class CWideString { public:
    void* m_pData;
    CWideString(const CWideString& other);              // 0x0099B720
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    ~CWideString();                                      // 0x0099B510
};

namespace std {

struct random_access_iterator_tag { };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, __value_type(__i1));
}

template <class _RandomAccessIter>
void __fastcall __reverse(_RandomAccessIter __first, _RandomAccessIter __last, const random_access_iterator_tag&)
{
    for (; __first < __last; ++__first)
        iter_swap(__first, --__last);
}

template void __fastcall __reverse<pair<long, CWideString>*>(pair<long, CWideString>*, pair<long, CWideString>*,
                                                             const random_access_iterator_tag&);

}