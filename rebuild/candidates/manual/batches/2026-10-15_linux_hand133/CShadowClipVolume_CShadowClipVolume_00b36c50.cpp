// CShadowClipVolume::CShadowClipVolume(const CShadowClipVolume& other) -- copy constructor: the eight clip planes, the plane count, the bounding sphere and the clip-to-sphere flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B36C50, length 133 bytes (+ int3 padding).
// Callees: none. Label correction: the manifest calls this a CEngineCameraDesc constructor (a propagated name); the 0x98-byte object with an
// 8 x 16-byte array, a dword at +0x80, four dwords at +0x84 and a byte at +0x94 is the PDB CShadowClipVolume (ClipPlanes CPlane[8] +0x00,
// ClipPlaneCount +0x80, BoundingSphere C3DBoundingSphere +0x84, ClipToSphere +0x94).
// Shape: the `mov ebx,8 ... dec ebx ; jne` loop is the compiler's element-wise copy of the CPlane array (CPlane's copy is non-trivial because
// C3DVector has a user member-wise copy constructor; VC strength-reduces the third dword into `[ecx+edx]` with ecx = this - other), followed
// by the scalar members. That loop only comes out of the IMPLICIT copy constructor: a hand-written body loop is unrolled by the lane compiler,
// and an array cannot be named in an initialiser list. VC7.1 emits the implicit constructor out of line for the copy site below.
// Leaf: ??0CShadowClipVolume
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};

struct CPlane {
    C3DVector Normal;                // +0x00
    float     Distance;              // +0x0c
};

struct C3DBoundingSphere {
    C3DVector Center;                // +0x00
    float     Radius;                // +0x0c
};

struct CShadowClipVolume {             // no user copy constructor: retail's body is the compiler-generated one
    CPlane            ClipPlanes[8];   // +0x00
    long              ClipPlaneCount;  // +0x80
    C3DBoundingSphere BoundingSphere;  // +0x84
    bool              ClipToSphere;    // +0x94
};

inline void* operator new(unsigned int, void* where) { return where; }

// A copy site in the TU: VC7.1 does not inline the implicit copy constructor (an element loop over a class with a
// non-trivial copy), so it emits ??0CShadowClipVolume@@QAE@ABU0@@Z out of line -- the retail function.
void CopyShadowClipVolume(void* where, const CShadowClipVolume& other)
{
    new (where) CShadowClipVolume(other);
}
