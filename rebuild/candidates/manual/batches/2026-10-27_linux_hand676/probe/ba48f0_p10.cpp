// CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksNoMorph(...) -- probe
// Leaf: ?SetupStaticBlocksNoMorph@CEnginePrimitiveManagerStaticMeshesBase@@SI_NAAVC3DPrimitive2@@ABVC3DMesh2@@AAVC3DTextureStages@@JPBVCGraphicDataBank@@_N@Z
class CTexture;
class CGraphicDataBank;
class CCompositeSurfaceInfo;
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
class C3DMeshMaterial {                                  // Ego_r.pdb, 0x30 bytes (restated: no engine header)
public:
    unsigned char _pad_0x00[0x30];
};
class C3DTextureStages {                                 // Ego_r.pdb, 0xC bytes
public:
    long DiffuseTextureStage;                            // +0x00
    long BumpTextureStage;                               // +0x04
    bool UseBlankTextures;                               // +0x08
};
namespace N3DPrimitive {
class CBaseBlock {                                       // Ego_r.pdb, 0xC bytes
public:
    unsigned long PrimitiveCount;                        // +0x00
    unsigned long StartIndex;                            // +0x04
    bool IsStrip;                                        // +0x08
    bool DegenerateTriangles;                            // +0x09
    unsigned char ChangeFlags;                           // +0x0A
};
class CStaticBlock : public CBaseBlock {                 // Ego_r.pdb, 0x20 bytes
public:
    unsigned long DiffuseTextureFixupOffset;             // +0x0C
    unsigned long BumpTextureFixupOffset;                // +0x10
    CTexture* DiffuseTexture;                            // +0x14
    CTexture* BumpTexture;                               // +0x18
    long MaterialIndex;                                  // +0x1C
};
}
class C3DPrimitive2 {
public:
    unsigned char _pad_0x00[0x40];
    unsigned long StaticBlockCount;                      // +0x40
    N3DPrimitive::CStaticBlock* StaticBlocks;            // +0x44
    unsigned char _pad_0x48[0x7c - 0x48];
    long RecordedFrame;                                  // +0x7C
    N3DPrimitive::CStaticBlock* GetStaticBlock(unsigned long index) { return &StaticBlocks[index]; }
    unsigned long GetStaticBlockCount() const { return StaticBlockCount; }
};
class C3DMesh2 {
public:
    unsigned char _pad_0x00[0x8c];
    C3DMeshMaterial* Materials;                          // +0x8C
};
class CEnginePrimitiveManagerMeshBase {
public:
    static const CTexture* __fastcall GetDiffuseTexture(const C3DMeshMaterial* material,
        CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo> >* surfaces, unsigned long level,
        bool& failed, const CGraphicDataBank* bank, bool load);                      // 0x00BA9360
    static const CTexture* __fastcall GetBumpTexture(const C3DMeshMaterial* material,
        CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo> >* surfaces, unsigned long level,
        bool& failed, const CGraphicDataBank* bank, bool load);                      // 0x00BA95D0
};
class CEnginePrimitiveManagerStaticMeshesBase : public CEnginePrimitiveManagerMeshBase {
public:
    static bool __fastcall SetupStaticBlocksNoMorph(C3DPrimitive2& primitive, const C3DMesh2& mesh, C3DTextureStages& stages,
        long level, const CGraphicDataBank* bank, bool load);
};

bool __fastcall CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksNoMorph(C3DPrimitive2& primitive, const C3DMesh2& mesh,
    C3DTextureStages& stages, long level, const CGraphicDataBank* bank, bool load)
{
    N3DPrimitive::CStaticBlock* first = primitive.GetStaticBlock(0);
    const C3DMeshMaterial* firstMaterial = &mesh.Materials[primitive.GetStaticBlock(0)->MaterialIndex];
    bool failed = false;
    if (stages.DiffuseTextureStage != -1)
        primitive.GetStaticBlock(0)->DiffuseTexture = const_cast<CTexture*>(GetDiffuseTexture(firstMaterial, 0, level, failed, bank, load));
    if (stages.BumpTextureStage != -1)
        first->BumpTexture = const_cast<CTexture*>(GetBumpTexture(firstMaterial, 0, level, failed, bank, load));
    unsigned long count = primitive.GetStaticBlockCount();
    for (unsigned long i = 1; i < count; ++i) {
        N3DPrimitive::CStaticBlock* block = primitive.GetStaticBlock(i);
        const C3DMeshMaterial* material = &mesh.Materials[block->MaterialIndex];
        if (block->ChangeFlags & 1) {
            if (stages.DiffuseTextureStage != -1)
                block->DiffuseTexture = const_cast<CTexture*>(GetDiffuseTexture(material, 0, level, failed, bank, load));
            else
                failed = true;
        }
        if ((block->ChangeFlags & 2) && stages.BumpTextureStage != -1)
            block->BumpTexture = const_cast<CTexture*>(GetBumpTexture(material, 0, level, failed, bank, load));
    }
    primitive.RecordedFrame = -1;
    return !failed;
}
