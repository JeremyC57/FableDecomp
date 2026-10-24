// CLandscapeLayerMesh::SetTargetVertexNormal(CTVertexLandscapeForeground& vertex, unsigned long packedNormal) -- unpack an 11:11:10 signed packed normal into the foreground vertex's float normal (x, y scaled by 1/1023, z by 1/511)
// Leaf: ?SetTargetVertexNormal@CLandscapeLayerMesh@@AAEXAAVCTVertexLandscapeForeground@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFDEC0, 145 bytes (+ int3 padding). Ego_r.pdb:
// ?SetTargetVertexNormal@CLandscapeLayerMesh@@AAEXAAVCTVertexLandscapeForeground@@K@Z (private __fastcall member that never reads
// `this`: both arguments on the stack, `ret 8`). The foreground twin of CLandscapeBackgroundPatch::SetTargetVertexNormal
// 0x00BE66F0 and the inverse of the landed PackVertexNormal 0x00BFDF60. Float pool: 0x0129EF84 = 1/1023, 0x0129EF80 = 1/511.
// Layout: CTVertexLandscapeForeground (Ego_r.pdb CTVertexLandscapeForegroundBase, 24 bytes): X/Y words +0/+2, Z +4,
// NormalX/NormalY/NormalZ +8/+0xc/+0x10, CliffLookupV/U +0x14/+0x15. The engine header for CLandscapeLayerMesh is not used:
// the member never touches `this`, so the class is declared here only to scope it.
// Shape: the sign extensions are `v | ((v & signBit) ? highMask : 0)` (`neg ; sbb ; and ; or`), the `fild` conversions go through
// the dead packed-normal argument slot, and the three products are stored from the FPU stack (`fxch st(2)` then three `fstp`).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CTVertexLandscapeForeground {
public:
    unsigned short X;                              // +0x00
    unsigned short Y;                              // +0x02
    float          Z;                              // +0x04
    float          NormalX;                        // +0x08
    float          NormalY;                        // +0x0c
    float          NormalZ;                        // +0x10
    unsigned char  CliffLookupV;                   // +0x14
    unsigned char  CliffLookupU;                   // +0x15
};

class CLandscapeLayerMesh {
    void SetTargetVertexNormal(CTVertexLandscapeForeground& vertex, unsigned long packedNormal);
};

void CLandscapeLayerMesh::SetTargetVertexNormal(CTVertexLandscapeForeground& vertex, unsigned long packedNormal)
{
    long x = packedNormal & 0x7ff;
    long y = (packedNormal >> 11) & 0x7ff;
    long z = packedNormal >> 22;
    x |= (x & 0x400) ? 0xfffff800 : 0;
    y |= (y & 0x400) ? 0xfffff800 : 0;
    z |= (z & 0x200) ? 0xfffffc00 : 0;
    C3DVector normal(x * (1.0f / 1023.0f), y * (1.0f / 1023.0f), z * (1.0f / 511.0f));
    vertex.NormalX = normal.X;
    vertex.NormalY = normal.Y;
    vertex.NormalZ = normal.Z;
}