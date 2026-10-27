// std::__uninitialized_fill_n(pair<long, CCountedPointer<CNavConnectorNode> >* first, unsigned int n, const pair<...>& x, const __false_type&) -- copy-construct x into n raw slots from first, return the end
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@std@@IU12@@std@@YIPAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a7f960, 52 bytes, /Gr `__fastcall` (first ecx, n edx, x and the tag on the stack), `ret 8`.
// Identity (proven): called only by the landed vector<pair<long, CCountedPointer<CNavConnectorNode> > >::_M_insert_overflow 0x00A838F0 (proven there: CNavQuadTree's connector map) which binds it ??$__uninitialized_fill_n@PAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@std@@IU12@@std@@....
// Body: STLport 4.6 `for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` -- placement new's null test, the key dword,
//   then CCountedPointer's copy constructor (Data, Info, `if (Info) ++Info->RefCount`).
// Shape (family L074): the fill_n body the landed _M_insert_overflow 0x00A838F0 keeps as a same-TU noinline helper.
class CNavConnectorNode;
class CCPPointerInfo {                                     // 12 bytes
public:
    unsigned long RefCount;                                // +0x00
    void (__fastcall* DeleteFunc)(void* data);             // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes
public:
    T* Data;                                               // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    CCountedPointer(const CCountedPointer<T>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};
inline void* operator new(unsigned int, void* __p) { return __p; }
namespace std {
struct __false_type { };
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6's user copy constructor
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new ((void*)__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template pair<long, CCountedPointer<CNavConnectorNode> >* __fastcall __uninitialized_fill_n(pair<long, CCountedPointer<CNavConnectorNode> >*, unsigned int, const pair<long, CCountedPointer<CNavConnectorNode> >&, const __false_type&);
}   // namespace std