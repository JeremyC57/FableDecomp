// CLibpkObject1D8::NormaliseCopy(CCodaVector* pOut, const CCodaVector* pIn) -- descriptive: for a zero input write the default up vector (0, 0, 1) and return 0; otherwise copy the input, take its length, normalise the copy in place (0x00C402E0) and return the length
// Leaf: ?NormaliseCopy@CLibpkObject1D8@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C418F0, 160 bytes, __fastcall `ret 8` (`this` unused), float in st(0).
// Callees: CCodaVector::Normalize 0x00C402E0. Float pool 0x0122DEDC = 0.0f.
// NF95 (part 2, q11383): the vector is GameCODA's CCodaVector (engine/CCodaVector.h; Normalize 0x00C402E0 =
//   ?Normalize@CCodaVector@@QAEMXZ per CODAApi.lib / CODAApiDbg.lib CodaListener.obj); was the local CPKVector3_00c402e0 stand-in
//   (Normalise / GetLength); the inline length is the row-local GetLength helper (same y, z, x order) and the copy is the SDK
//   CodeView class's memberwise operator= (FABLE_RETAIL_PASS22_CCODA3D_CREATE; the implicit copy emits a esi/edi block move).
// Identity: libpk audio (no PDB/RTTI names exist for libpk: neither Ego_r nor FableWin carries one). Called once, from
//   0x00C422A4 with `this` = the libpk object whose dirty word sits at +0x1D8 (out +0x120, in +0xFC, result stored at
//   +0x12C, then `or [esi+1D8h],0D0000h`) -- the landed CLibpkObject1D8 setters' class (dirty word +0x1D8). The vector
//   is CCodaVector (formerly the landed CPKVector3_00c402e0 stand-in). Descriptive; the manifest's
//   "Math_ComputeVectorMagnitude" is propagated.
// Shape: the copy precedes the length (retail reloads the input after the stores). The length is the double-local rule
//   (GOTCHAS, 0x007986A0 / 0x00BDDA80): `double length = v.GetLength(); float result = (float)length;` keeps the root on
//   the FPU (`fsqrt ; fstp st(3) ; fstp st(0) ; fstp st(0)`) and only then spills it to the dead pOut slot across the
//   Normalise call; a plain float local stores before the pops (the parked 160v160 residue). GetLength loads z, y, x
//   (the const member read through `this`) and sums y*y + z*z + x*x.
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double value);

#define FABLE_RETAIL_CCODAVECTOR_ORTHOGONALRIGHT
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE   // the SDK CodeView CCodaVector (explicit memberwise operator=)
#include "engine/CCodaVector.h"
inline float GetLength(const CCodaVector& v) { return (float)sqrt(v.y * v.y + v.z * v.z + v.x * v.x); }   // descriptive (the inline length; no vendor name)

class CLibpkObject1D8 {
public:
    float NormaliseCopy(CCodaVector* pOut, const CCodaVector* pIn);
};

float CLibpkObject1D8::NormaliseCopy(CCodaVector* pOut, const CCodaVector* pIn)
{
    if (pIn->x == 0.0f && pIn->y == 0.0f && pIn->z == 0.0f) {
        pOut->x = 0.0f;
        pOut->y = 0.0f;
        pOut->z = 1.0f;
        return 0.0f;
    }
    *pOut = *pIn;
    double length = GetLength(*pIn);
    float result = (float)length;
    pOut->Normalize();
    return result;
}