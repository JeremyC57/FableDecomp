#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_ACCESSORS
#include "engine/CMatrix4x4.h"
extern "C" void Use(const void*);

struct Holder { char pad[0x210]; CMatrix4x4 World; char pad2[0x350-0x250]; CMatrix4x4 TT[4]; long n; void Run(); };
void Holder::Run() { for (long i = 0; i < n; ++i) { __declspec(align(16)) CMatrix4x4 t; t.AlignedSetMultiply(TT[i], World); Use(&t); } }
extern "C" void ProbeMul(Holder* h) { h->Run(); }
