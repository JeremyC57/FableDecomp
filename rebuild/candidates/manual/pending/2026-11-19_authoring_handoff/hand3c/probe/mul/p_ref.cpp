#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS
#include "engine/CMatrix4x4.h"
extern "C" void ProbeMul(CMatrix4x4& d, const CMatrix4x4& a, const CMatrix4x4& b) { d.AlignedSetMultiply(a, b); }
