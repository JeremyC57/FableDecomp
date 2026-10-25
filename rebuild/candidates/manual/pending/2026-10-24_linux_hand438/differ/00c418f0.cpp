// CPKListener_00c418f0::NormaliseCopy(CPKVector3_00c418f0* pOut, const CPKVector3_00c418f0* pIn) -- for a zero input write the default up vector (0, 0, 1) and return 0; otherwise copy the input, take its length, normalise the copy in place (0x00C402E0) and return the length
// Grade: DIFFER(160v160) -- only the x87 spill order of the length: retail `fsqrt ; fstp st(3) ; fstp st(0) ; fstp st(0) ; fstp [esp+4]` (pop the dead x/y/z first, then spill), ours stores the length to [esp+4] straight after fsqrt and then pops. 3 grades (implicit copy 168v160, memberwise operator= 160v160, the sqrt written inline) + a QFE-4035 probe, all the same. Lane gap: x87 spill scheduling.
// Leaf: ?NormaliseCopy@CPKListener_00c418f0@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C418F0, 160 bytes, __thiscall `ret 8` (`this` unused), float in st(0).
// Library: libpk audio (no PDB/RTTI); descriptive names. Called once, from 0x00C422A4 (`this` = the listener;
//   out +0x120, in +0xfc; the result stored at +0x12c). Float constant 0x0122DEDC = 0.0f. The manifest's
//   "Math_ComputeVectorMagnitude" is propagated.
// Shape: the copy precedes the length (retail reloads the input after the stores); the length is a float local kept
//   across the Normalise call in the dead pOut argument slot.
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double value);

struct CPKVector3_00c418f0 {
    float x, y, z;
    CPKVector3_00c418f0& operator=(const CPKVector3_00c418f0& v) { x = v.x; y = v.y; z = v.z; return *this; }
    float Normalise();                                  // 0x00C402E0
    float Length() const { return (float)sqrt(y * y + z * z + x * x); }
};

struct CPKListener_00c418f0 {
    float NormaliseCopy(CPKVector3_00c418f0* pOut, const CPKVector3_00c418f0* pIn);
};

float CPKListener_00c418f0::NormaliseCopy(CPKVector3_00c418f0* pOut, const CPKVector3_00c418f0* pIn)
{
    if (pIn->x == 0.0f && pIn->y == 0.0f && pIn->z == 0.0f) {
        pOut->x = 0.0f;
        pOut->y = 0.0f;
        pOut->z = 1.0f;
        return 0.0f;
    }
    *pOut = *pIn;
    float length = pIn->Length();
    pOut->Normalise();
    return length;
}
