#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS
#include "engine/CMatrix4x4.h"
extern "C" void Use(const void*);

static __forceinline void X87Mul(CMatrix4x4& d, const CMatrix4x4& a, const CMatrix4x4& b) {
 for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) d(r, c) = a(r, 0) * b(0, c) + a(r, 1) * b(1, c) + a(r, 2) * b(2, c) + a(r, 3) * b(3, c); }

extern "C" void ProbeMul(const CMatrix4x4& a, const CMatrix4x4& b) { __declspec(align(16)) CMatrix4x4 t; X87Mul(t, a, b); Use(&t); }
