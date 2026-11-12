#pragma optimize("s",on)
// std::__destroy_aux(std::pair<long, CDefPointer<const CHeroAbilityDef> >* first, std::pair<...>* last, const std::__false_type&) -- STLport 4.6 _construct.h: run the out-of-line pair destructor over every 8-byte element of [first, last)
// Leaf: ??$__destroy_aux@PAU?$pair@JV?$CDefPointer@$$CBVCHeroAbilityDef@@@@@std@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F9A7D, 27 bytes, `__fastcall` (first ecx, last edx, the tag on the stack), `ret 4`.
// Name (agentNF20, 2026-11-11; was the descriptive Destroy_range / Elem / Destroy_pair stand-in): the element dtor it calls is
//   0x005F8736 ??1?$pair@JV?$CDefPointer@$$CBVCHeroAbilityDef@@@@@std@@QAE@XZ (FableWin.pdb), and its caller 0x005FA1BA is a
//   vector<pair<long, CDefPointer<const CHeroAbilityDef> > > member (Ego_r.pdb CTCInventoryAbilities::Abilities); the 8-byte stride
//   is that pair. STLport's nontrivial-destructor range helper; the size-optimised TU keeps the pair destructor out of line.
class CHeroAbilityDef;
template <class T> class CDefPointer { public:
    T* Object;
};

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    ~pair();                                       // 0x005F8736
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template void __fastcall __destroy_aux<pair<long, CDefPointer<const CHeroAbilityDef> >*>(pair<long, CDefPointer<const CHeroAbilityDef> >*, pair<long, CDefPointer<const CHeroAbilityDef> >*, const __false_type&);
}
