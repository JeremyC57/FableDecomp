// std::__copy(pair<C3DVector,bool>* first, last, result, const random_access_iterator_tag&, int*) -- STLport random-access assignment copy of a prong-vector range; returns the end of the destination
// Leaf: ??$__copy@PAU?$pair@VC3DVector@@_N@std@@PAU12@H@std@@YIPAU?$pair@VC3DVector@@_N@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe @ 0x006EF000, 80 bytes, /Gr `__fastcall` (first ecx, last edx; result, the tag reference and the
// null distance pointer on the stack, `ret 0Ch`).
// Identity (probable): STLport 4.6 `__copy(first, last, result, random_access_iterator_tag, _Distance*)`
// (`for (n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }`) over 13-byte elements (/13 reciprocal
// 0x4EC4EC4F, sar 2): pair<C3DVector, bool> with retail's 1-aligned C3DVector. Its callers 0x006EBBFF, 0x006EFF4C,
// 0x006F2587, 0x006F3153 sit in the CCombatManager / CDefendingCombatantInfo code whose PDB names pass
// vector<pair<C3DVector,bool> >& (CCombatRing / CCombatWheel::DebugGetAllProngVectors, SetupInitialPlacementVectors);
// 0x006EBBFF calls it as the vector's `erase(begin(), end())` (`copy(end, end, begin)`, then `_M_finish = result`).
// Shape: the element assignment is pair's implicit operator= (C3DVector's implicit one, three dwords through ebx, then
// the bool byte); the empty range returns the result argument straight from its slot.
#pragma pack(push, 1)
class C3DVector { public:                           // retail C3DVector is 1-aligned (GOTCHAS)
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
#pragma pack(pop)
namespace std {
#pragma pack(push, 1)
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
#pragma pack(pop)
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template pair<C3DVector, bool>* __fastcall __copy(pair<C3DVector, bool>*, pair<C3DVector, bool>*, pair<C3DVector, bool>*,
                                                  const random_access_iterator_tag&, int*);
}