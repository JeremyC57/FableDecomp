// std::__copy_backward(std::pair<CCharString,C3DMeshVolume>* first, std::pair<CCharString,C3DMeshVolume>* last, std::pair<CCharString,C3DMeshVolume>* result, const random_access_iterator_tag&, int*) -- STLport 4.6 backward copy (assign from the back), returns the new start (the name-keyed C3DMeshVolume map (CVectorMap<CCharString, C3DMeshVolume>))
// Leaf: ??$__copy_backward@PAU?$pair@VCCharString@@VC3DMeshVolume@@@std@@PAU12@H@std@@YIPAU?$pair@VCCharString@@VC3DMeshVolume@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A70AB0, 152 bytes (__fastcall: first / last in ecx / edx, the rest pushed).
// Identity: the name-keyed C3DMeshVolume map (CVectorMap<CCharString, C3DMeshVolume>) sort family (std::sort 0x00A73BA0, NF91 q11191 / q11192).
// NF91 (2026-11-17, claimed as the q11191 / q11192 family follow-up): the old row spelt the element as a row-local `class pair` (PAV) on local
//   CCharString / C3DMeshVolume / CKeyPairCompareLess models (own leaf ?CopyBackward_PairCharStringMeshVolume_00a70ab0@@YIPAUPairCharStringMeshVolume@@PAU1@00ABUrandom_access_iterator_tag@@PAH@Z); STLport's pair is a struct (PAU), as the sort front end
//   now binds it. Now the STLport 4.6 body of stlport/fable_containers.h by explicit instantiation over engine/C3DMeshVolume.h and the global
//   comparator of engine/CVectorMap.h (the 0x00A33050 recipe); `// Flags: /Gr`. STLport instance name: descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template std::pair<CCharString,C3DMeshVolume>* FABLE_STLP_FASTCALL std::__copy_backward(std::pair<CCharString,C3DMeshVolume>*,std::pair<CCharString,C3DMeshVolume>*,std::pair<CCharString,C3DMeshVolume>*,const std::random_access_iterator_tag&,int*);
