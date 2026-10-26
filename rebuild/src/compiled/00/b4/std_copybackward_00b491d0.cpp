// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (CEngineLightingManager::CThemeChannel elements) STLport random-access copy_backward: assign [first, last) from the back so that it ends at result, return the new start of the destination
// Leaf: ??$__copy_backward@PAUCThemeChannel@CEngineLightingManager@@PAU12@H@std@@YIPAUCThemeChannel@CEngineLightingManager@@PAU12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B491D0, 73 bytes (+ int3 padding), /Gr __fastcall (first ecx, last edx; result,
//   the tag reference and the distance-type pointer on the stack), `ret 0Ch`; speed TU (count by `imul 92492493h ; sar edx,6`).
// Identity (proven): __copy_backward<CEngineLightingManager::CThemeChannel*, CEngineLightingManager::CThemeChannel*, int>; evidence: its call site 0x00B4A36F is in the _M_fill_insert 0x00B4A2C0 of resize(n, x) 0x00B4A4C0, which the CEngineLightingManager constructor 0x00B482A0 (stores the RTTI vtable .?AVCEngineLightingManager@@ 0x012A223C) calls on this+0x3C: Ego_r.pdb CEngineLightingManager::ThemeChannels, CArray<CEngineLightingManager::CThemeChannel> (a 112-byte struct; FableWin.pdb ?size@?$vector@UCThemeChannel@CEngineLightingManager@@).
// Label: the manifest's "_Copy_backward<std::pair<CCharString,C3DDummyObject>*,std::pair<class_CCharString,class_C3DDummyObject>*>" is propagated across family H040 (six bodies that differ in nothing but their
//   address; retail has no identical-code folding, so each address is one instantiation).
// Shape: STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result;`
//   over a trivially assignable 112-byte element (the assignment is `rep movsd` with ecx = 28).
class CEngineLightingManager {
public:
    struct CThemeChannel {                              // Ego_r.pdb, 112 bytes (five colours, four floats, FogTransform)
        long Word[28];
    };
};
namespace std {
struct random_access_iterator_tag {};

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
}
typedef CEngineLightingManager::CThemeChannel TElem;
template TElem* __fastcall std::__copy_backward<TElem*, TElem*, int>(TElem*, TElem*, TElem*, const std::random_access_iterator_tag&, int*);