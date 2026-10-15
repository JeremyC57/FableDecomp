// C3DMesh2::GetVertexCount() -- sums VertexCount over every primitive of the mesh
// Leaf: GetVertexCount
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A89320, length 33 bytes (+ int3 padding).
// Callees: none.
// Catalog label "CTCHeroStats::GetNumberOfAbilityCasts" is a propagated name: the body sits between
// C3DMesh2::InitialiseBoneNamePointers (0x00A892E0) and C3DMesh2::CalculateTextureChangeFlags
// (0x00A89380) and walks the same primitive array (PrimitiveCount +0x90, Primitives +0x94, 0xa0-byte
// C3DPrimitive2 records). Retail C3DMesh2 sits 8 bytes below the Ego_r PDB layout; retail C3DPrimitive2
// is 12 bytes narrower than the PDB's 0xac (its CArray members are 0x0c bytes in retail), so the PDB's
// VertexCount (+0x5c) is at +0x58 and TriangleCount (+0x60) at +0x5c. Its twin 0x00A89350 sums
// TriangleCount.
// Shape: VC7.1 strength-reduces the loop into a pointer to the summed field (`add ecx,58h` then
// `add ecx,0A0h`) and a down-counting `dec edx ; jne` on the unsigned count (the `jbe` entry test).
struct C3DPrimitive2 {
    unsigned char _pad_0x00[0x58];
    unsigned long VertexCount;             // +0x58 (PDB +0x5c)
    unsigned long TriangleCount;           // +0x5c (PDB +0x60)
    unsigned char _pad_0x60[0x40];         // retail record is 0xa0 bytes
};

struct C3DMesh2 {
    unsigned char  _pad_0x00[0x90];
    unsigned long  PrimitiveCount;         // +0x90 (PDB +0x98)
    C3DPrimitive2* Primitives;             // +0x94 (PDB +0x9c)
    unsigned long GetVertexCount();
};

unsigned long C3DMesh2::GetVertexCount()
{
    unsigned long total = 0;
    for (unsigned long i = 0; i < PrimitiveCount; ++i)
        total += Primitives[i].VertexCount;
    return total;
}
