// Grade: DIFFER(441v441) -- one store position: `failed = false` must be the first local defined to get retail slots (flag homed in the mesh slot, failed in the frame), but VC then emits its store at the top of the branch; retail stores it after the prim load (tried six declaration/definition orders, const flag, a named condition)
// CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocks(C3DPrimitive2& primitive, CEngineInternalPrimitiveStaticMesh& mesh, C3DTextureStages& stages, const C3DMeshMaterial* material, CVectorMap<long, CCompositeSurfaceInfo>* surfaces, long variant, bool* alreadySetUp) -- resolve each static block's diffuse and bump textures from its mesh material (blank textures when the stages ask for them), skipping the work when nothing changed since the recorded bank frame; false when a block needed a diffuse texture the stages cannot take
// Leaf: SetupStaticBlocks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BA50A0, length 448 bytes (body 0x1b9), `ret 1Ch`.
// Callees: SetupStaticBlocksBlankTextures 0x00BA4A40, the static `__fastcall` CEnginePrimitiveManagerMeshBase::
//   GetDiffuseTexture 0x00BA9360 / GetBumpTexture 0x00BA95D0 (material, surfaces, variant, bool& failed, bank, flag).
// Layout: C3DPrimitive2 retail as the landed-shape SetupStaticBlocksBlankTextures (StaticBlockCount +0x40, StaticBlocks
//   +0x44, RecordedTextureStages +0x6c, RecordedFrame +0x7c); CStaticBlock (0x20): map flags byte +0xa (1 diffuse,
//   2 bump), DiffuseTexture +0x14, BumpTexture +0x18, MaterialIndex +0x1c. The static mesh: flags +0xc (bit 7 is the
//   texture-lookup flag), the graphic bank through +0x50 -> +4 -> +0x3d4, the C3DMesh data at RenderInfo +0xa4 with its
//   0x30-byte C3DMeshMaterial array at +0x8c. The bank frame stamp +0x194.
// Shape: block 0 uses the `material` argument, the others their own mesh material; the stage comparison against the
//   recorded stages folds UseBlankTextures to the already-tested false.
struct CTexture;
struct CGraphicDataBank {
    unsigned char _pad000[0x194];
    long          FrameStamp;                      // +0x194
};
struct CGraphicsOwner { unsigned char _pad[0x3d4]; CGraphicDataBank* Bank; };   // +0x3d4
struct CMeshResource { unsigned char _pad[4]; CGraphicsOwner* Owner; };          // +0x04

struct C3DMeshMaterial { unsigned char _body[0x30]; };
struct C3DMeshData {
    unsigned char    _pad00[0x8c];
    C3DMeshMaterial* Materials;                    // +0x8c
};
struct CCompositeSurfaceInfo;
struct CSurfaceMap;                                // CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<...> >

struct C3DTextureStages {
    long DiffuseTextureStage;                      // +0x00
    long BumpTextureStage;                         // +0x04
    bool UseBlankTextures;                         // +0x08
    bool operator==(const C3DTextureStages& other) const
    {
        return DiffuseTextureStage == other.DiffuseTextureStage && BumpTextureStage == other.BumpTextureStage
            && UseBlankTextures == other.UseBlankTextures;
    }
};

struct CStaticBlock {                              // 0x20 bytes
    unsigned char   _pad00[0xa];
    unsigned char   MapFlags;                      // +0x0a
    unsigned char   _pad0b[9];
    const CTexture* DiffuseTexture;                // +0x14
    const CTexture* BumpTexture;                   // +0x18
    long            MaterialIndex;                 // +0x1c
};

struct C3DPrimitive2 {
    unsigned char    _pad00[0x40];
    unsigned long    StaticBlockCount;             // +0x40
    CStaticBlock*    StaticBlocks;                 // +0x44
    unsigned char    _pad48[0x6c - 0x48];
    C3DTextureStages RecordedTextureStages;        // +0x6c
    unsigned char    _pad78[4];
    long             RecordedFrame;                // +0x7c
};

struct CEngineInternalPrimitiveStaticMesh {
    unsigned char  _pad00[0xc];
    unsigned long  Flags;                          // +0x0c
    unsigned char  _pad10[0x40];
    CMeshResource* Mesh;                           // +0x50
    unsigned char  _pad54[0xa4 - 0x54];
    C3DMeshData*   MeshData;                       // +0xa4 (RenderInfo +4)
};

struct CEnginePrimitiveManagerMeshBase {
    static const CTexture* __fastcall GetDiffuseTexture(const C3DMeshMaterial* material, CSurfaceMap* surfaces, unsigned long variant,
                                                        bool& failed, const CGraphicDataBank* bank, bool flag);   // 0x00BA9360
    static const CTexture* __fastcall GetBumpTexture(const C3DMeshMaterial* material, CSurfaceMap* surfaces, unsigned long variant,
                                                     bool& failed, const CGraphicDataBank* bank, bool flag);      // 0x00BA95D0
};

struct CEnginePrimitiveManagerStaticMeshesBase : CEnginePrimitiveManagerMeshBase {
    bool SetupStaticBlocksBlankTextures(C3DPrimitive2& primitive, CEngineInternalPrimitiveStaticMesh& mesh, C3DTextureStages& stages,
                                        const C3DMeshMaterial* material, bool* alreadySetUp);   // 0x00BA4A40
    bool SetupStaticBlocks(C3DPrimitive2& primitive, CEngineInternalPrimitiveStaticMesh& mesh, C3DTextureStages& stages,
                           const C3DMeshMaterial* material, CSurfaceMap* surfaces, long variant, bool* alreadySetUp);
};

bool CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocks(C3DPrimitive2& primitive, CEngineInternalPrimitiveStaticMesh& mesh,
                                                                C3DTextureStages& stages, const C3DMeshMaterial* material,
                                                                CSurfaceMap* surfaces, long variant, bool* alreadySetUp)
{
    if (stages.UseBlankTextures) {
        SetupStaticBlocksBlankTextures(primitive, mesh, stages, material, alreadySetUp);
        return true;
    }
    bool failed = false;
    CGraphicDataBank* bank = mesh.Mesh->Owner->Bank;
    C3DMeshData* meshData = mesh.MeshData;
    bool flag = (mesh.Flags >> 7) & 1;
    if (surfaces == 0 && bank->FrameStamp == primitive.RecordedFrame && stages == primitive.RecordedTextureStages) {
        *alreadySetUp = true;
        return true;
    }
    *alreadySetUp = false;
    CStaticBlock* blocks = primitive.StaticBlocks;
    if (stages.DiffuseTextureStage != -1)
        blocks[0].DiffuseTexture = GetDiffuseTexture(material, surfaces, variant, failed, bank, flag);
    if (stages.BumpTextureStage != -1)
        blocks[0].BumpTexture = GetBumpTexture(material, surfaces, variant, failed, bank, flag);
    unsigned long count = primitive.StaticBlockCount;
    for (unsigned long i = 1; i < count; ++i) {
        CStaticBlock& block = primitive.StaticBlocks[i];
        const C3DMeshMaterial* blockMaterial = &meshData->Materials[block.MaterialIndex];
        if (block.MapFlags & 1) {
            if (stages.DiffuseTextureStage != -1)
                block.DiffuseTexture = GetDiffuseTexture(blockMaterial, surfaces, variant, failed, bank, flag);
            else
                failed = true;
        }
        if ((block.MapFlags & 2) && stages.BumpTextureStage != -1)
            block.BumpTexture = GetBumpTexture(blockMaterial, surfaces, variant, failed, bank, flag);
    }
    return !failed;
}
