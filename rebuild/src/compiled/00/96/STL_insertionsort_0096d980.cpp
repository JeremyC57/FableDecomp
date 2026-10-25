// std::__insertion_sort(CThing** first, CThing** last, CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> comp) -- guarded insertion sort of the screamer pursue-target sort (each element goes through the inlined __linear_insert)
// Leaf: ??$__insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0096D980, 115 bytes, __fastcall (/Gr algorithm TU: first in ecx, last in edx,
//   the comparator on the stack), `ret 4`.
// Instantiation: the screamer target sort: std::partial_sort / sort<CThing**,
//   CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> > of CAIStateGroup_ScreamerPursueTarget::Update
//   (0x0096DF50) and its __introsort_loop (0x0096E410).
// Callees: CThingCompare_Nearest::operator()<CThing> @ 0x004A8D80 (through the adapter, inline), memmove (import
//   thunk) @ 0x00BFEAE6, __unguarded_linear_insert @ 0x0096D590.
// Shape: STLport __insertion_sort with __linear_insert, copy_backward and the memmove inlined;
//   __unguarded_linear_insert stays a call.
// Label: catalog "std_sort_DownHeap" -> the STLport 4.x std::__insertion_sort instantiation for
//   CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> (retail links STLport; the Dinkumware names in Ego_r.pdb
//   are a different build).
// Build: the lane defaults (/O2 /Oy /W3), no pragma.
class CThing;
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6 (import thunk)

// C3DVector with the engine's memberwise user copy constructor (Ego_r.pdb: X, Y, Z).
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

// Ego_r.pdb CThingCompare_Nearest: 12 bytes, Pos (C3DVector) +0x00; operator() returns the nearer thing.
class CThingCompare_Nearest {
public:
    C3DVector Pos;                                        // +0x00
    template <class T> T* operator()(T* a, T* b) const;   // 0x004A8D80 (the nearer of the two; a null loses)
};

// CAdaptThingCompareToSTLComparitor<TCompare> (Ego_r.pdb: 4 bytes, Functor pointer): turns a "pick the better thing"
// functor into a strict-weak-ordering predicate -- a sorts first when the functor picks it. Inline, so every
// comparison is `push b ; push a ; mov ecx,[functor] ; call 0x004A8D80 ; cmp eax,a`.
template <class TCompare>
class CAdaptThingCompareToSTLComparitor {
public:
    TCompare* Functor;                                    // +0x00
    template <class T> bool operator()(T* a, T* b) const { return (*Functor)(a, b) == a; }
};
typedef CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest> TComp;

namespace std {
struct __true_type { __true_type() { } };

// copy_backward over a trivially assignable pointer range is one memmove (`(_Num > 0) ? memmove : result`).
inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : __result;
}
template <class _Tp>
inline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial_backward(__first, __last, __result);
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                         _BidirectionalIter2 __result)
{
    return __copy_backward_ptrs(__first, __last, __result, __true_type());
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x0096D590

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}
template void __fastcall __insertion_sort<CThing**, TComp>(CThing**, CThing**, TComp);
} // namespace std