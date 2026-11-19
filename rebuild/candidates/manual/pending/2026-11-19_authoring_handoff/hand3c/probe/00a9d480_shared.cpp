// diag: shared header bodies out of line
// Leaf: ?AlignedPreScale@CMatrix4x4@@QAEXABVC3DVector@@@Z
// Flags: none
#include "fable_compiler.h"
#define FABLE_RETAIL_CMATRIX4X4
#define FABLE_RETAIL_CMATRIX4X4_ELEMENT_ACCESS
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDPRESCALE_BODIES
#include "engine/CMatrix4x4.h"
#include "engine/CMatrix4x4AlignedSSE.h"
