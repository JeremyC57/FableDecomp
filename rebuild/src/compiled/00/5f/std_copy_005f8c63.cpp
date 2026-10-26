// std::__copy(const std::pair<long, CDefPointer<const CHeroAbilityDef> >* first, const std::pair<long, CDefPointer<const CHeroAbilityDef> >* last, std::pair<long, CDefPointer<const CHeroAbilityDef> >* result, const random_access_iterator_tag&, int*) -- STLport counted forward assignment of pair<long, CDefPointer<const CHeroAbilityDef> > elements; returns the advanced result
// Leaf: ??$__copy@PBU?$pair@JV?$CDefPointer@$$CBVCHeroAbilityDef@@@@@std@@PAU12@H@std@@YIPAU?$pair@JV?$CDefPointer@$$CBVCHeroAbilityDef@@@@@0@PBU10@0PAU10@ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F8C63, 45 bytes, __fastcall (first ecx, last edx; result, the tag reference and the distance pointer on the stack), `ret 0Ch`; size-optimised TU (result advanced in place, `add dword ptr [esp+0Ch],8`).
// Identity (proven): std::__copy<const std::pair<long, CDefPointer<const CHeroAbilityDef> >*, std::pair<long, CDefPointer<const CHeroAbilityDef> >*, int>; evidence: the 8-byte element's assignment 0x005F853E (landed pair<long, CDefPointer<const CHeroAbilityDef> >::operator=) copies the long and re-points a counted def; the live caller 0x005FA1BA calls it twice -- the two `__copy_ptrs((const_pointer)x.start, ...)` arms of vector::operator= -- and is reached from the CTCInventoryAbilities constructor 0x005F3DC6, whose Ego_r.pdb Abilities member is vector<pair<long, CDefPointer<const CHeroAbilityDef> > >: const source iterators.
// Label: the manifest's "_Copy_impl<CArray<CRandomAppearanceMorph::CBodyPartMesh>*, ...>" is propagated (family H060: six out-of-line __copy loops, one per element).
// Callee: pair<long, CDefPointer<const CHeroAbilityDef> >::operator= 0x005F853E (the element assignment).
// Shape: STLport 4.6 _algobase.h `__copy(first, last, result, random_access_iterator_tag, Distance*)` as written (the landed CWideString
//   0x00433CEC / CArray<unsigned short> 0x00F3B8A0 bodies), `__fastcall` on the template definition.
#pragma optimize("s", on)
class CHeroAbilityDef;
template <class T> class CDefPointer {                      // 4 bytes: the counted def pointer
public:
    T* Object;
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair& operator=(const pair& other);                     // the implicit assignment, out of line
};
}

namespace std {
struct random_access_iterator_tag { };
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
template std::pair<long, CDefPointer<const CHeroAbilityDef> >* __fastcall __copy<const std::pair<long, CDefPointer<const CHeroAbilityDef> >*, std::pair<long, CDefPointer<const CHeroAbilityDef> >*, int>(const std::pair<long, CDefPointer<const CHeroAbilityDef> >*, const std::pair<long, CDefPointer<const CHeroAbilityDef> >*, std::pair<long, CDefPointer<const CHeroAbilityDef> >*, const random_access_iterator_tag&, int*);
}