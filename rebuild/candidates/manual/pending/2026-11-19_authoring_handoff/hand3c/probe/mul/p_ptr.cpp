#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS
#include "engine/CMatrix4x4.h"
extern "C" void Use(const void*);

extern "C" void ProbeMul(const CMatrix4x4* a, const CMatrix4x4* b, CMatrix4x4* d) { d->AlignedSetMultiply(*a, *b); }
