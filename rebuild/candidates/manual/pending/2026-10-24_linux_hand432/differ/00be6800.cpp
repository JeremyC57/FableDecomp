// CLandscapeBackgroundPatch::GetPackedNormalFromTargetVertex(const CTVertexLandscapeBackground& vertex) -- pack the vertex's float normal into the 11:11:10 word (x*1023 bits 0-10, y*1023 bits 11-21, z*511 bits 22-31, each rounded)
// Grade: DIFFER(82v113) -- retail_asm candidate: each component is rounded by the engine's inline-assembly ROUND helper (bare dword fistp after a float store/reload); this C++ reference converts with (long), which VC7.1 compiles to __ftol2 calls
// Leaf: ?GetPackedNormalFromTargetVertex@CLandscapeBackgroundPatch@@SIKABVCTVertexLandscapeBackground@@@Z
// Retail-asm evidence: 0x00BE681A `fstp dword ptr [esp] ; fld dword ptr [esp] ; fistp dword ptr [esp+8]` (x3, z then y then x), the same
// helper the landed CPackedNormalOwner_00be6790::PackVertexNormal (grade retail_asm) expands; VC7.1 C++ `(long)x` calls __ftol2 and
// /QIfist gives `fistp qword` without the reload (GOTCHAS wave 25 chunk 145).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE6800, 113 bytes. Ego_r.pdb:
// ?GetPackedNormalFromTargetVertex@CLandscapeBackgroundPatch@@SIKABVCTVertexLandscapeBackground@@@Z (static __fastcall, vertex in ecx).
// Float pool: 0x0129EF90 = 1023.0f, 0x0129EF94 = 511.0f. No callers by direct call (reached through an inlined/indirect path).
// Shape: the normal is first copied into a C3DVector local (X/Y as integer moves into the frame, Z kept on the FPU stack), then
// PackVertexNormal's body runs inline: z, y, x rounded into a 3-word frame and combined as ((y & 0x7ff) | (z << 11)) << 11 | (x & 0x7ff).
// Landing route: the ROUND helper exactly as in the landed 0x00BE6790 source (retail_asm exception) around this body.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CTVertexLandscapeBackground {
public:
    unsigned short X, Y;                           // +0x00 / +0x02
    float          Z;                              // +0x04
    float          NormalX, NormalY, NormalZ;      // +0x08 / +0x0c / +0x10
    unsigned char  UVPad1;                         // +0x14
};
__forceinline long ROUND(float value) { return (long)value; }   // retail: the inline-assembly fld/fistp helper

__forceinline unsigned long PackVertexNormal(const C3DVector& normal)
{
    long z = ROUND(normal.Z * 511.0f);
    long y = ROUND(normal.Y * 1023.0f);
    long x = ROUND(normal.X * 1023.0f);
    return (((y & 0x7ff) | (z << 11)) << 11) | (x & 0x7ff);
}

class CLandscapeBackgroundPatch {
public:
    static unsigned long __fastcall GetPackedNormalFromTargetVertex(const CTVertexLandscapeBackground& vertex);
};

unsigned long __fastcall CLandscapeBackgroundPatch::GetPackedNormalFromTargetVertex(const CTVertexLandscapeBackground& vertex)
{
    C3DVector normal(vertex.NormalX, vertex.NormalY, vertex.NormalZ);
    return PackVertexNormal(normal);
}
