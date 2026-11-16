// C3DVector::Copy() const -- returns the vector as GameCODA _CVECTOR storage (x, y, z) through the hidden result pointer, assembled field by field in a local
// Leaf: ?Copy@C3DVector@@QBE?AU_CVECTOR@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BFF0, 53 bytes (+11 int3 padding). No callees.
//
// The manifest label `CPhysicalPrimitiveMesh::GetClosestPointToPos2D` is propagated and wrong:
// `this` is a 12-byte vector (three floats at +0/+4/+8), the result goes through the hidden pointer
// (`mov eax,[esp+10h]` after the 12-byte frame, `ret 4`, eax returned). Callers in the render module
// (0x00C1C5B2) use it beside the Y/Z swizzle 0x00C1C070 (identical body with +4/+8 exchanged).
// Shape: the vector is a POD (no user constructors). The copy is assembled field by field
// in a NAMED local (frame slots x/y/z) and that local is copied out to the hidden result
// (`mov ecx,eax` then three load/store pairs): a POD return copies through the frame. A user
// (memberwise) copy constructor instead folds everything into three x87 stores straight into the
// result (25 bytes), and dword members with constructors elide the frame.
// Port (NF40, round 4): descriptive name (no PDB spelling; the queue claim keeps "Copy" and proves the _CVECTOR return: the GameCODA listener/source vector the libpk callers pass to the
//   CODA API). Now on engine/C3DVector.h (FABLE_RETAIL_C3DVECTOR_LIBPK_OUTOFLINE + _LIBPK_CODA_COPY_PASS23 declare the _CVECTOR
//   return) and engine/CodaEnvironment.h (_CVECTOR); the row defines the out-of-line body (same text as the header's
//   FABLE_RETAIL_C3DVECTOR_LIBPK_CODA_COPY_BODY_PASS23 inline body, not enabled here so the definition is emitted).
#define FABLE_RETAIL_C3DVECTOR_LIBPK_OUTOFLINE
#define FABLE_RETAIL_C3DVECTOR_LIBPK_CODA_COPY_PASS23
#include "engine/C3DVector.h"
#include "engine/CodaEnvironment.h"

_CVECTOR C3DVector::Copy() const
{
    _CVECTOR result;
    result.x = X;
    result.y = Y;
    result.z = Z;
    return result;
}
