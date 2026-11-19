// CCodaVector::OrthogonalRight(const CCodaVector& vFront, const CCodaVector& vTop) -- this = vTop x vFront, each component stored as soon as it is computed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40370, length 57 bytes.
// Leaf: ?OrthogonalRight@CCodaVector@@QAEXABV1@0@Z
// Retail bytes: mov edx,[esp+8] ; mov eax,[esp+4] ; fld [edx+4] ; fmul [eax+8] ; fld [eax+4] ; fmul [edx+8] ; fsubp ; fstp [ecx] ; (Y) ; fstp [ecx+4] ; (Z) ; fstp [ecx+8] ; ret 8
// NF95 (part 2, q11384): was the descriptive stand-in C3DVector::SetCrossProduct (manifest label CRightHandedSet::GetRight, propagated).
//   GameCODA 2.10 CODAApi.lib member .\Release\CodaListener.obj holds this 57-byte COMDAT (the header inline OrthogonalRight that
//   CCodaListener::Commit 0x00C40590 calls at its second site; Commit reloc +0x74) and CODAApiDbg.lib names it the same:
//   ?OrthogonalRight@CCodaVector@@QAEXABV1@0@Z, params vFront, vTop (the SDK Toolkit CtkVector.h twin). The body is the
//   engine/CCodaVector.h inline; opt-in FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT_OUTOFLINE turns the in-class body into a declaration
//   so this row emits the out-of-line copy.
#define FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT
#define FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT_OUTOFLINE
#include "engine/CCodaVector.h"

void CCodaVector::OrthogonalRight(const CCodaVector& vFront, const CCodaVector& vTop)
{
    x = vTop.y * vFront.z - vFront.y * vTop.z;
    y = vTop.z * vFront.x - vFront.z * vTop.x;
    z = vTop.x * vFront.y - vFront.x * vTop.y;
}
