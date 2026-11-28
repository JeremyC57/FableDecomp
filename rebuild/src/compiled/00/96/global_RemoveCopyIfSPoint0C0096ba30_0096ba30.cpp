// Leaf: ??$remove_copy_if@PAVC3DVector@@PAV1@V?$unary_negate@VCVectorFilter_IsWithinHeightRange@@@std@@@std@@YIPAVC3DVector@@PAV1@00V?$unary_negate@VCVectorFilter_IsWithinHeightRange@@@0@@Z
// Flags: /Gr
#define FABLE_RETAIL_CVECTORFILTER_ISWITHINHEIGHTRANGE_PASS30
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
#include "engine/CVectorFilter_IsWithinHeightRange.h"
#include "stlport/fable_algorithm.h"

namespace std {
template C3DVector* FABLE_FASTCALL remove_copy_if<C3DVector*,C3DVector*,unary_negate<CVectorFilter_IsWithinHeightRange> >(
    C3DVector*,C3DVector*,C3DVector*,unary_negate<CVectorFilter_IsWithinHeightRange>);
}
