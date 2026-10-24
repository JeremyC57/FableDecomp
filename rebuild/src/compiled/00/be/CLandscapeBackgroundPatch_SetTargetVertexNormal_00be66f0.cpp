// CLandscapeBackgroundPatch::SetTargetVertexNormal(CTVertexLandscapeBackground& vertex, unsigned long packedNormal) -- unpack an 11:11:10 signed packed normal into the vertex's float normal (x, y scaled by 1/1023, z by 1/511)
// Leaf: ?SetTargetVertexNormal@CLandscapeBackgroundPatch@@SIXAAVCTVertexLandscapeBackground@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE66F0, 150 bytes (+ int3 padding). Ego_r.pdb:
// ?SetTargetVertexNormal@CLandscapeBackgroundPatch@@SIXAAVCTVertexLandscapeBackground@@K@Z (static __fastcall: the vertex in
// ecx, the packed word in edx, `ret`). Nine callers in the patch tesselator / BuildDefaultSection / Load. The inverse of the
// landed PackVertexNormal 0x00BE6790 (x bits 0-10, y bits 11-21, z bits 22-31). Float pool: 0x0129EF84 = 1/1023,
// 0x0129EF80 = 1/511.
// Layout: CTVertexLandscapeBackground (Ego_r.pdb CTVertexLandscapeBackgroundBase, 24 bytes): X/Y words +0/+2, Z +4,
// NormalX/NormalY/NormalZ +8/+0xc/+0x10, UVPad1 +0x14. The engine header for CLandscapeBackgroundPatch has no methods, so the
// static member's class is declared here only to scope it.
// Shape: each component's sign extension is `v | ((v & signBit) ? highMask : 0)` (`neg ; sbb ; and ; or`), each converted with
// `fild` through one frame word. The three products build a NAMED C3DVector local through its 3-float constructor and are
// then copied member by member into the vertex: Z goes through the local's frame word as an integer copy (`fstp [esp+0Ch] ;
// mov edx`) and is stored first, X and Y are stored straight from the FPU stack. Direct member assignments keep a 4-byte frame
// with three fstp; a field-by-field named local stores X before Z. Grades: 4.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CTVertexLandscapeBackground {
public:
    unsigned short X;                              // +0x00
    unsigned short Y;                              // +0x02
    float          Z;                              // +0x04
    float          NormalX;                        // +0x08
    float          NormalY;                        // +0x0c
    float          NormalZ;                        // +0x10
    unsigned char  UVPad1;                         // +0x14
};

class CLandscapeBackgroundPatch {
public:
    static void __fastcall SetTargetVertexNormal(CTVertexLandscapeBackground& vertex, unsigned long packedNormal);
};

void __fastcall CLandscapeBackgroundPatch::SetTargetVertexNormal(CTVertexLandscapeBackground& vertex, unsigned long packedNormal)
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