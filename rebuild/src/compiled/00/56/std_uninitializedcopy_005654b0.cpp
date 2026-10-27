// std::__uninitialized_copy(const std::pair<float, CWideString>* first, const std::pair<float, CWideString>* last, std::pair<float, CWideString>* result, const __false_type&) -- STLport placement copy-construct each record (the scalar, then the member's out-of-line copy constructor) and return the advanced result
// Leaf: ??$__uninitialized_copy@PBU?$pair@MVCWideString@@@std@@PAU12@@std@@YIPAU?$pair@MVCWideString@@@0@PBU10@0PAU10@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005654b0, 54 bytes, __fastcall (first in ecx, last in edx, result + tag on the stack), `ret 8`.
// Identity (proven): callers the landed std::vector<pair<float, CWideString> >::vector(const vector&) 0x00566CD0 and the landed operator= 0x00568D40 (const sources; the online-scoreboard rows); bound as the non-template stand-in ?__uninitialized_copy@std@@YIPAU?$pair@MVCWideString@@@1@PBU21@0PAU21@...; stride 8, CWideString copy at +4.
// Callees: CWideString::CWideString(const CWideString&) 0x0099B720 (bound).
// Shape (family L011): STLport 4.6 `__uninitialized_copy(first, last, result, const __false_type&)` verbatim; the record's
//   implicit copy constructor under placement new's null test; stride 8.
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {             // STLport pair: implicit copy constructor
    _T1 first;
    _T2 second;
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}
class CWideString {
public:
    void* PStringData;                                    // +0x00
    CWideString(const CWideString& other);                // 0x0099B720
};
template std::pair<float, CWideString>* __fastcall std::__uninitialized_copy<const std::pair<float, CWideString>*, std::pair<float, CWideString>*>(const std::pair<float, CWideString>*, const std::pair<float, CWideString>*, std::pair<float, CWideString>*, const std::__false_type&);