// C3DMesh2::CalculateTextureChangeFlags() -- asks every primitive (0xa0-byte C3DPrimitive2 entries at +0x94, PrimitiveCount at +0x90) to compute its texture-change flags against this mesh
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A89380, length 54 bytes.
// Callees: C3DPrimitive2::CalculateTextureChangeFlags(C3DMesh2*) @ 0x00A8E5C0.
// Retail C3DMesh2 sits 8 bytes below the Ego_r PDB layout here (PrimitiveCount +0x90, Primitives
// +0x94); the retail C3DPrimitive2 is 0xa0 bytes. The loop keeps an index and a byte offset
// and re-reads the array base and count each iteration (the callee may reallocate); unsigned
// compare (`jb`).
struct C3DMesh2;

struct C3DPrimitive2 {
    unsigned char _body[0xa0];
    void CalculateTextureChangeFlags(C3DMesh2* mesh);   // 0x00A8E5C0
};

struct C3DMesh2 {
    unsigned char _pad_0x00[0x90];
    unsigned long PrimitiveCount;          // +0x90
    C3DPrimitive2* Primitives;             // +0x94
    void CalculateTextureChangeFlags();
};

void C3DMesh2::CalculateTextureChangeFlags()
{
    for (unsigned long i = 0; i < PrimitiveCount; ++i)
        Primitives[i].CalculateTextureChangeFlags(this);
}
