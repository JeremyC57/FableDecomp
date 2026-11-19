#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS
#include "engine/CMatrix4x4.h"
extern "C" void Use(const void*);

extern "C" void ProbeMul(const CMatrix4x4& a, const CMatrix4x4& b) { __declspec(align(16)) CMatrix4x4 t; t.AlignedSetMultiply(a, b); Use(&t); }
