// Leaf: ??$__copy@PAVC3DVector@@PAV1@H@std@@YIPAVC3DVector@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"

namespace std {
template C3DVector* FABLE_FASTCALL __copy<C3DVector*,C3DVector*,int>(
    C3DVector*,C3DVector*,C3DVector*,const random_access_iterator_tag&,int*);
}
