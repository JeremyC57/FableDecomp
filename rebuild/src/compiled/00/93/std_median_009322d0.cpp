// std::__median<CThing*, bool (__fastcall*)(CThing*, CThing*)>(const CThing*& a, b, c, comp) -- the STLport median of
//   three with a function-pointer comparator: b when a<b<c, else c when a<c, else a; symmetrically when !(a<b)
// Retail: Fable.exe (TLC, VC7.1) @ 0x009322D0, 96 bytes; `__fastcall` (a in ecx -> ebx, b in edx -> edi, c and the
//   comparator on the stack -> ebp / esi), `ret 8`. No direct callees: the comparator is called through esi (`call esi`).
// Identity: the propagated label `CPredicate_And<...CIsThingAlive...>::operator()` is wrong. The five compares in the SGI
//   order and the returned element address are __median (cf. the landed functor twin 0x009B2930). It is the out-of-line
//   copy of the __median that the landed TU sibling __introsort_loop<CThing**, CThing*, int, ThingCompare> 0x009327F0
//   inlines: the TU (0x009322D0..0x00932940: __push_heap 0x00932370, __insertion_sort 0x00932660, sort 0x009328D0) sorts
//   the CThing* range of CAIStateGroup_MinionReactToCriminal::Update (0x0093150A) under the plain `bool __fastcall
//   (CThing*, CThing*)` height compare 0x00930D90, so the element is CThing* and the comparator takes the two pointers BY
//   VALUE (every call loads both elements: `mov edx,[edi] ; mov ecx,[ebx]`). FINDINGS chunk 406.
// Leaf: ??$__median@PAVCThing@@P6I_NPAV1@0@Z@std@@YIABQAVCThing@@ABQAV1@00P6I_NPAV1@1@Z@Z
class CThing;
typedef bool (__fastcall* ThingCompare)(CThing* a, CThing* b);   // the height compare 0x00930D90

namespace std {
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}
template CThing* const& __fastcall __median<CThing*, ThingCompare>(CThing* const& __a, CThing* const& __b, CThing* const& __c,
                                                                     ThingCompare __comp);
}
