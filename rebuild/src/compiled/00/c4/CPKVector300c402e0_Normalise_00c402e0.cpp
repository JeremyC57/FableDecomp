// CPKVector3_00c402e0::Normalise() -- return 0 for the zero vector; otherwise compute the length and, unless it is already 1, scale the three components by its reciprocal; returns the length
// Leaf: ?Normalise@CPKVector3_00c402e0@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C402E0, 138 bytes (+ int3 padding), __fastcall, float in st(0).
// Library: libpk audio (no PDB/RTTI); descriptive names. Float constants: 0x0122DEDC = 0.0f, 0x0122DED8 = 1.0f.
//   Callers 0x00C405BD / 0x00C405C9 (listener orientation, result dropped), 0x00C41982 (NormaliseCopy), 0x00C42053,
//   0x00C4222E. The manifest's "Vector3_Normalize" is close.
// Shape: the squared sum is written z*z + y*y + x*x (retail x87 order: z, y, x loaded, z*z first); the unit test is
//   `1.0f != length` (`fld st(0) ; fld [one] ; fucompp`; `length != 1.0f` loads the constant first).
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double value);

struct CPKVector3_00c402e0 {
    float x, y, z;
    float Normalise();
};

float CPKVector3_00c402e0::Normalise()
{
    if (x == 0.0f && y == 0.0f && z == 0.0f)
        return 0.0f;
    float length = (float)sqrt(z * z + y * y + x * x);
    if (1.0f != length) {
        float scale = 1.0f / length;
        x *= scale;
        y *= scale;
        z *= scale;
    }
    return length;
}