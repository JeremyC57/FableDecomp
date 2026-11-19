// std::__lower_bound<pair<long,float>*, pair<long,float>, less<pair<long,float> >, int>(first, last, const pair<long,float>& val, less<> comp, int*) -- STLport binary search for the first (long, float) pair not less than val (CEnvironment::SetWeightedThemes' ThemeWeights)
// Flags: /Gr
// Leaf: ??$__lower_bound@PAU?$pair@JM@std@@U12@U?$less@U?$pair@JM@std@@@2@H@std@@YIPAU?$pair@JM@0@PAU10@0ABU10@U?$less@U?$pair@JM@std@@@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B5890, 76 bytes, /Gr __fastcall (first ecx, last edx; &val, the empty comparator slot, the null
//   distance-type pointer on the stack), `ret 0Ch`. Only caller: the landed lower_bound<pair<long,float>*, pair<long,float> > 0x006B5A50
//   (called from CEnvironment::SetWeightedThemes 0x006B3E60, environment.obj -- not tc_mapwho.obj).
// Identity (proven, two sources): rebuild/oracles/reloc_bindings.tsv binds 0x006B5890 to exactly this decorated name (from the landed caller
//   0x006B5A50); Ego_r.pdb / FableWin.pdb hold the Dinkumware twin ??$_Lower_bound@PAU?$pair@JM@std@@U12@H@std@@... (Ego_r 0x006F1B30, next to
//   ?SetWeightedThemes@CEnvironment@@ 0x006EFD20 in environment.obj order). The catalog label `_Lower_bound<...>` is that Dinkumware spelling;
//   retail is STLport (less<> and the int* distance type are in the binding).
// Source: the shared STLport 4.6 _algobase.c body (fable_algorithm.h PASS22_LOWER_BOUND_BODY) and _pair.h operator< (fable_containers.h,
//   carried by the DECAL_HEAP opt-in), instantiated explicitly.
// FITTED (coordinator decision): new overlay opt-in FABLE_RETAIL_STLP_PAIR_LESS_RIGHT_BYVALUE (hdr/stlport/fable_containers.h, nested in
//   DECAL_HEAP's pair operator<): the right operand by value. Retail hoists val.second onto the x87 stack (0x006B58A0 `fld [eax+4]`,
//   `fcom [ecx+edx*8+4]` at 0x006B58B3, `fstp st(0)` at 0x006B58D3); the const& STLport text reloads it every pass (DIFFER 76v76, ebx = &val)
//   and no flag (/G5-/G7 /Op /Oa /Ox /Ot /Os /Ow /O1 /Og /GL) or operand order moves it; by value on both sides is DIFFER(93v76). No
//   source evidence for the spelling (STLport 4.6.2 _pair.h is const&; no other retail float-pair comparison exists to cross-check;
//   no landed row instantiates the pair operator<). Same value for every input.
// Inferred: (owner's 0x006E89B0 precedent; hand4a w24 re-check, 9 grades) the right operand of the pair operator< by value is the only
//   text that gives retail's hoisted x87 val.second (0x006B58A0 fld [eax+4] / 0x006B58B3 fcom [ecx+edx*8+4] / 0x006B58D3 fstp st(0)).
//   Sourced readings ruled out: (1) STLport 4.6.2 3-arg lower_bound compares with less<value_type of the iterator> and val is const _Tp&;
//   the caller 0x006B3F3A passes the same key (&[esp+10h]) to vector<pair<long,float> >::insert, so val's type IS the element type and no
//   converting pair temporary exists (stock text: DIFFER 76v76, &val in ebx, fld [middle+4] / fcomp [ebx+4]); (2) implicit instantiation
//   through the 0x006B5A50 forwarder in the same TU, a POD comparator (no binary_function base), /Ob1 /Oy- /GX- (plus hand3a's /G5-/G7
//   /Op /Oa /Ox /Ot /Os /Ow /O1 /Og): no change; (3) donors: FableWin std::operator< <long,float> and Ego_r _Lower_bound 0x006F1B30 are
//   const& / const& (VC10 Ego_r keeps &val in ebx too). Same value for every input.
#define FABLE_RETAIL_STLP_PAIR_LESS_RIGHT_BYVALUE
#define FABLE_RETAIL_STLP_DECAL_HEAP
#define FABLE_RETAIL_STLP_PASS22_LOWER_BOUND_COMPARE
#define FABLE_RETAIL_STLP_PASS22_LOWER_BOUND_BODY
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
namespace std {
template pair<long, float>* FABLE_FASTCALL __lower_bound(pair<long, float>*, pair<long, float>*, const pair<long, float>&,
                                                         less<pair<long, float> >, int*);
}