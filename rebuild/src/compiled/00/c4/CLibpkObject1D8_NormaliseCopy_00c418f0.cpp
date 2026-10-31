// CLibpkObject1D8::NormaliseCopy(CPKVector3_00c402e0* pOut, const CPKVector3_00c402e0* pIn) -- descriptive: for a zero input write the default up vector (0, 0, 1) and return 0; otherwise copy the input, take its length, normalise the copy in place (0x00C402E0) and return the length
// Leaf: ?NormaliseCopy@CLibpkObject1D8@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C418F0, 160 bytes, __fastcall `ret 8` (`this` unused), float in st(0).
// Callees: CPKVector3_00c402e0::Normalise 0x00C402E0 (landed spelling). Float pool 0x0122DEDC = 0.0f.
// Identity: libpk audio (no PDB/RTTI names exist for libpk: neither Ego_r nor FableWin carries one). Called once, from
//   0x00C422A4 with `this` = the libpk object whose dirty word sits at +0x1D8 (out +0x120, in +0xFC, result stored at
//   +0x12C, then `or [esi+1D8h],0D0000h`) -- the landed CLibpkObject1D8 setters' class (dirty word +0x1D8). The vector
//   is the landed CPKVector3_00c402e0 (one class with its Normalise). Descriptive; the manifest's
//   "Math_ComputeVectorMagnitude" is propagated.
// Shape: the copy precedes the length (retail reloads the input after the stores). The length is the double-local rule
//   (GOTCHAS, 0x007986A0 / 0x00BDDA80): `double length = v.GetLength(); float result = (float)length;` keeps the root on
//   the FPU (`fsqrt ; fstp st(3) ; fstp st(0) ; fstp st(0)`) and only then spills it to the dead pOut slot across the
//   Normalise call; a plain float local stores before the pops (the parked 160v160 residue). GetLength loads z, y, x
//   (the const member read through `this`) and sums y*y + z*z + x*x.
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double value);

struct CPKVector3_00c402e0 {
    float x, y, z;
    CPKVector3_00c402e0& operator=(const CPKVector3_00c402e0& v) { x = v.x; y = v.y; z = v.z; return *this; }
    float Normalise();                                  // 0x00C402E0
    float GetLength() const { return (float)sqrt(y * y + z * z + x * x); }
};

class CLibpkObject1D8 {
public:
    float NormaliseCopy(CPKVector3_00c402e0* pOut, const CPKVector3_00c402e0* pIn);
};

float CLibpkObject1D8::NormaliseCopy(CPKVector3_00c402e0* pOut, const CPKVector3_00c402e0* pIn)
{
    if (pIn->x == 0.0f && pIn->y == 0.0f && pIn->z == 0.0f) {
        pOut->x = 0.0f;
        pOut->y = 0.0f;
        pOut->z = 1.0f;
        return 0.0f;
    }
    *pOut = *pIn;
    double length = pIn->GetLength();
    float result = (float)length;
    pOut->Normalise();
    return result;
}