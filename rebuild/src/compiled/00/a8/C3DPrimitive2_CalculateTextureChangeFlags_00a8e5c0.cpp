// C3DPrimitive2::CalculateTextureChangeFlags(const C3DMesh2*) -- for every static block after the first: mark it degenerate when its material is, else flag a diffuse (1) / bump (2, when the primitive uses bump maps) texture change against the previous block's material
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8E5C0, length 136 bytes.
// Leaf: CalculateTextureChangeFlags
// Caller: C3DMesh2::CalculateTextureChangeFlags (landed 0x00A89380) passes the owning mesh. No callees.
// Layout: the retail C3DPrimitive2 is 0xa0 bytes (PDB 0xac less 4 bytes for each of its three CArrays):
// StaticBlockCount +0x40, StaticBlocks +0x44, InitFlags +0x68. CStaticBlock (PDB, 32 bytes) = CBaseBlock
// {PrimitiveCount, StartIndex, IsStrip, DegenerateTriangles +9, ChangeFlags +0xa} + fix-ups/textures +
// MaterialIndex +0x1c. C3DMesh2's Materials sit at retail +0x8c (PDB +0x94, the landed caller's 8-byte shift);
// C3DMeshMaterial (PDB, 48 bytes): DiffuseMapBankIndex +0x10, BumpMapBankIndex +0x14, DegenerateTriangles +0x2b.
// Shape: the mesh's material array and the previous block's material INDEX are named locals read before the
// block reference (`[eax+ebp-4]` ahead of `lea esi`; a reference to the previous material is CSE'd as
// `[esi-4]`), both materials are scaled (`lea x,[x+x*2] ; shl x,4`) before the degenerate test; the loop index
// lives in the `push ecx` frame slot and the count and block array are re-read every iteration.
struct CTexture;
struct CBaseBlock {                                  // 12 bytes (PDB)
    unsigned long PrimitiveCount;                    // +0x00
    unsigned long StartIndex;                        // +0x04
    bool          IsStrip;                           // +0x08
    bool          DegenerateTriangles;               // +0x09
    unsigned char ChangeFlags;                       // +0x0a
};
struct CStaticBlock : CBaseBlock {                   // 32 bytes (PDB)
    unsigned long DiffuseTextureFixupOffset;         // +0x0c
    unsigned long BumpTextureFixupOffset;            // +0x10
    CTexture*     DiffuseTexture;                    // +0x14
    CTexture*     BumpTexture;                       // +0x18
    long          MaterialIndex;                     // +0x1c
};
struct C3DMeshMaterial {                             // 48 bytes (PDB)
    unsigned char _pad00[0x10];
    unsigned long DiffuseMapBankIndex;               // +0x10
    unsigned long BumpMapBankIndex;                  // +0x14
    unsigned char _pad18[0x13];
    bool          DegenerateTriangles;               // +0x2b
    void*         Filenames;                         // +0x2c
};
struct C3DMesh2 {
    unsigned char    _pad00[0x8c];
    C3DMeshMaterial* Materials;                      // +0x8c (PDB +0x94)
};

struct C3DPrimitive2 {
    unsigned char _pad00[0x40];
    unsigned long StaticBlockCount;                  // +0x40
    CStaticBlock* StaticBlocks;                      // +0x44
    unsigned char _pad48[0x20];
    unsigned long InitFlags;                         // +0x68
    unsigned char _pad6c[0x34];                      // sizeof == 0xa0
    void CalculateTextureChangeFlags(const C3DMesh2* mesh);
};

void C3DPrimitive2::CalculateTextureChangeFlags(const C3DMesh2* mesh)
{
    for (unsigned long i = 1; i < StaticBlockCount; ++i) {
        const C3DMeshMaterial* materials = mesh->Materials;
        long previousIndex = StaticBlocks[i - 1].MaterialIndex;
        CStaticBlock& block = StaticBlocks[i];
        const C3DMeshMaterial& material = materials[block.MaterialIndex];
        const C3DMeshMaterial& previous = materials[previousIndex];
        if (material.DegenerateTriangles)
            block.DegenerateTriangles = true;
        else {
            if (material.DiffuseMapBankIndex != previous.DiffuseMapBankIndex)
                block.ChangeFlags |= 1;
            if (InitFlags & 2) {
                if (material.BumpMapBankIndex != previous.BumpMapBankIndex)
                    block.ChangeFlags |= 2;
            }
        }
    }
}