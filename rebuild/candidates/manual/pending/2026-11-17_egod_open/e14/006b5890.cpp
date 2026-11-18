// std::__lower_bound<pair<long,float>*, pair<long,float>, less<pair<long,float> >, int>(first, last, const pair<long,float>& val, less<> comp, int*) -- STLport binary search for the first (long, float) pair not less than val (CEnvironment::SetWeightedThemes' ThemeWeights)
// Grade: DIFFER(76v76) -- x87 enregistration only: retail hoists `fld [val+4]` above the loop and compares `fcom [middle+4] ; test ah,41h ; jne` (popping st(0) once at the exit, 0x006B58A0 / 0x006B58B3 / 0x006B58D3); VC7.1 RTM here keeps &val in ebx and loads per iteration (`fld [middle+4] ; fcomp [ebx+4] ; test ah,5 ; jp`). Every other byte matches.
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
#define FABLE_RETAIL_STLP_DECAL_HEAP
#define FABLE_RETAIL_STLP_PASS22_LOWER_BOUND_COMPARE
#define FABLE_RETAIL_STLP_PASS22_LOWER_BOUND_BODY
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
namespace std {
template pair<long, float>* FABLE_FASTCALL __lower_bound(pair<long, float>*, pair<long, float>*, const pair<long, float>&,
                                                         less<pair<long, float> >, int*);
}
