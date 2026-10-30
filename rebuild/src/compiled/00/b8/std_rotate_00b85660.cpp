// std::__rotate(CEngineSubPrimitiveRenderer::CSortElement* __first, CSortElement* __middle, CSortElement* __last, int*, CSortElement*) -- STLport 4.6 random-access rotate of the sort-element array: swap_ranges when both halves are equal, else gcd(n, k) cycles of element moves; returns first + (last - middle)
// Leaf: ??$__rotate@PAVCSortElement@CEngineSubPrimitiveRenderer@@HV12@@std@@YIPAVCSortElement@CEngineSubPrimitiveRenderer@@PAV12@00PAH0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B85660, 356 bytes, __fastcall (first ecx, middle edx; last, the (int*)0 distance
//   and the (CSortElement*)0 value-type pointers on the stack), `ret 0Ch`.
// Identity (proven): the spelling the landed caller 0x00B86140 __merge_without_buffer binds (reloc_bindings.tsv), the
//   CEngineSubPrimitiveRenderer stable_sort chain (0x00B84E20..0x00B86710; element CSortElement, 8 bytes, Ego_r.pdb /
//   FableWin.pdb); the body is STLport 4.6 _algo.c's random-access `__rotate` verbatim with `__gcd` inlined (the
//   cdq/idiv loop). The manifest label "RotateElements_CNavigationPosition" is a propagated fold label (wrong element).
// Callees: swap_ranges<CSortElement*, CSortElement*> 0x00B85390 (landed; fastcall, first2 pushed, `ret 4`).
class CEngineSubPrimitiveBase;

class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {                                // 8 bytes (Ego_r.pdb / FableWin.pdb)
    public:
        unsigned long SortValue;                        // +0x00
        CEngineSubPrimitiveBase* SubPrimitive;          // +0x04
        CSortElement() {}
        bool operator<(const CSortElement& other) const { return SortValue < other.SortValue; }
    };
};
typedef CEngineSubPrimitiveRenderer::CSortElement CSortElement;

namespace std {
template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter2 __fastcall swap_ranges(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2);   // 0x00B85390

template <class _EuclideanRingElement>
_EuclideanRingElement __gcd(_EuclideanRingElement __m, _EuclideanRingElement __n)
{
    while (__n != 0) {
        _EuclideanRingElement __t = __m % __n;
        __m = __n;
        __n = __t;
    }
    return __m;
}

template <class _RandomAccessIter, class _Distance, class _Tp>
_RandomAccessIter __fastcall __rotate(_RandomAccessIter __first,
                                      _RandomAccessIter __middle,
                                      _RandomAccessIter __last,
                                      _Distance *, _Tp *)
{
    _Distance __n = __last   - __first;
    _Distance __k = __middle - __first;
    _Distance __l = __n - __k;
    _RandomAccessIter __result = __first + (__last - __middle);

    if (__k==0)  /* __first == middle */
        return __last;

    if (__k == __l) {
        swap_ranges(__first, __middle, __middle);
        return __result;
    }

    _Distance __d = __gcd(__n, __k);

    for (_Distance __i = 0; __i < __d; __i++) {
        _Tp __tmp = *__first;
        _RandomAccessIter __p = __first;

        if (__k < __l) {
            for (_Distance __j = 0; __j < __l/__d; __j++) {
                if (__p > __first + __l) {
                    *__p = *(__p - __l);
                    __p -= __l;
                }

                *__p = *(__p + __k);
                __p += __k;
            }
        }

        else {
            for (_Distance __j = 0; __j < __k/__d - 1; __j ++) {
                if (__p < __last - __k) {
                    *__p = *(__p + __k);
                    __p += __k;
                }

                *__p = * (__p - __l);
                __p -= __l;
            }
        }

        *__p = __tmp;
        ++__first;
    }

    return __result;
}

template CSortElement* __fastcall __rotate<CSortElement*, int, CSortElement>(CSortElement*, CSortElement*, CSortElement*,
    int*, CSortElement*);
}