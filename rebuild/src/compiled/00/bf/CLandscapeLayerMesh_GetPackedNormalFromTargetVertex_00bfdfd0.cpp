// CLandscapeLayerMesh::GetPackedNormalFromTargetVertex(const CTVertexLandscapeForeground& vertex) -- pack the foreground vertex's float normal into the 11:11:10 word (x*1023 bits 0-10, y*1023 bits 11-21, z*511 bits 22-31, each rounded)
// Grade: RELOCATION_MATCH (1 grade) -- rasm1 retail_asm: C++ body + a minimal 2-instruction __asm (no control flow) + the Retail-asm evidence line
// Retail-asm: three expansions of the engine's inline-assembly ROUND helper (each component product stored to the by-value copy in a dead argument slot, reloaded and rounded with a bare `fistp dword`), the same helper as the landed retail_asm PackVertexNormal 0x00BE6790/0x00BFDF60. VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's `__asm`.
// Leaf: ?GetPackedNormalFromTargetVertex@CLandscapeLayerMesh@@AAEKABVCTVertexLandscapeForeground@@@Z
// Retail-asm evidence: 0x00BFDFEE `fstp dword ptr [esp+1Ch] ; fld dword ptr [esp+1Ch] ; fistp dword ptr [esp+4]` (x3), the helper the landed
// CPackedNormalOwner_00bfdf60::PackVertexNormal (grade retail_asm) expands; VC7.1 C++ `(long)x` calls __ftol2 (GOTCHAS wave 25 chunk 145).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFDFD0, 123 bytes. Ego_r.pdb:
// ?GetPackedNormalFromTargetVertex@CLandscapeLayerMesh@@AAEKABVCTVertexLandscapeForeground@@@Z (private __fastcall that never reads
// `this`, vertex on the stack, `ret 4`). Float pool: 0x0129EF90 = 1023.0f, 0x0129EF94 = 511.0f.
// Shape: as the background twin 0x00BE6800: the normal copied into a C3DVector local (X/Y integer moves, Z on the FPU stack),
// PackVertexNormal inline (z, y, x rounded, combined as ((y & 0x7ff) | (z << 11)) << 11 | (x & 0x7ff)); here the products are
// homed in the dead vertex-argument slot. The ROUND helper is the one of the landed retail_asm source 0x00BFDF60.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CTVertexLandscapeForeground {
public:
    unsigned short X, Y;                           // +0x00 / +0x02
    float          Z;                              // +0x04
    float          NormalX, NormalY, NormalZ;      // +0x08 / +0x0c / +0x10
    unsigned char  CliffLookupV, CliffLookupU;     // +0x14 / +0x15
};
// the engine's inline-assembly ROUND helper (round to nearest, the FPU default); expanded in place as a __forceinline
__forceinline long ROUND(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

__forceinline unsigned long PackVertexNormal(const C3DVector& normal)
{
    long z = ROUND(normal.Z * 511.0f);
    long y = ROUND(normal.Y * 1023.0f);
    long x = ROUND(normal.X * 1023.0f);
    return (((y & 0x7ff) | (z << 11)) << 11) | (x & 0x7ff);
}

class CLandscapeLayerMesh {
    unsigned long GetPackedNormalFromTargetVertex(const CTVertexLandscapeForeground& vertex);
};

unsigned long CLandscapeLayerMesh::GetPackedNormalFromTargetVertex(const CTVertexLandscapeForeground& vertex)
{
    C3DVector normal(vertex.NormalX, vertex.NormalY, vertex.NormalZ);
    return PackVertexNormal(normal);
}