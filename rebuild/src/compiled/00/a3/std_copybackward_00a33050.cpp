// std::__copy_backward(pair<unsigned long, C3DMeshVolume>* first, pair<unsigned long, C3DMeshVolume>* last, pair<unsigned long, C3DMeshVolume>* result, const random_access_iterator_tag&, int*) -- STLport 4.6 backward copy of the mesh-volume table entries (assign from the back), returns the new start
// Leaf: ??$__copy_backward@PAU?$pair@KVC3DMeshVolume@@@std@@PAU12@H@std@@YIPAU?$pair@KVC3DMeshVolume@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A33050, 151 bytes, `ret 0Ch` (__fastcall: first in ecx, last in edx; result,
//   the tag and the null distance pushed).
// Identity: the out-of-line STLport __copy_backward of the C3DMeshStats volume table; the landed callers __linear_insert 0x00A34820
//   and vector<pair<unsigned long, C3DMeshVolume> >::_M_fill_insert 0x00A35010 already bind it by this STLport spelling. Callees:
//   CCharString::operator= 0x0099EFB0 and operator delete 0x00BFE9BC (the counted pointer's assignment inline).
// NF91 (2026-11-17, claimed with q11193): the old row was catalogued _global::CopyBackward_PairULongMeshVolume_00a33050 on row-local
//   CCharString / CCountedPointer / C3DMeshVolume / pair models; now the stlport/fable_containers.h FABLE_RETAIL_STLP_SORT_INTERNALS
//   __copy_backward (the 0x0077E170 recipe) by explicit instantiation over engine/C3DMeshVolume.h. STLport instance name:
//   descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template std::pair<unsigned long,C3DMeshVolume>* FABLE_STLP_FASTCALL std::__copy_backward(std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>*,std::pair<unsigned long,C3DMeshVolume>*,const std::random_access_iterator_tag&,int*);
