// CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksNoMorph(C3DPrimitive2& primitive, const C3DMesh2& mesh, C3DTextureStages& stages, long level, const CGraphicDataBank* bank, bool load) -- resolve the diffuse / bump textures of a static mesh primitive's blocks: block 0 always (per active texture stage), later blocks only where their change flags say the texture changes (a changed diffuse with no diffuse stage marks failure); then invalidate the recorded frame and return true when nothing failed
// Grade: DIFFER(327v324) -- one extra `mov edi,[ebp+44h]` (StaticBlocks reload) before the first bump store; everything else matches
// Leaf: ?SetupStaticBlocksNoMorph@CEnginePrimitiveManagerStaticMeshesBase@@SI_NAAVC3DPrimitive2@@ABVC3DMesh2@@AAVC3DTextureStages@@JPBVCGraphicDataBank@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BA48F0, 324 bytes, static __fastcall (primitive in ecx, mesh in edx), `ret 10h`.
// Identity (proven): Ego_r.pdb ?SetupStaticBlocksNoMorph@CEnginePrimitiveManagerStaticMeshesBase@@SI_NAAVC3DPrimitive2@@ABVC3DMesh2@@
//   AAVC3DTextureStages@@JPBVCGraphicDataBank@@_N@Z (public static fastcall; the catalog label is the bare member name); the callees
//   are Ego_r ?GetDiffuseTexture@CEnginePrimitiveManagerMeshBase@@SIPBVCTexture@@PBVC3DMeshMaterial@@PAV?$CVectorMap@..@@KAA_N
//   PBVCGraphicDataBank@@_N@Z 0x00BA9360 and ?GetBumpTexture@.. 0x00BA95D0 (same signature), called with no surface map (edx = 0).
//   The members walked: C3DPrimitive2 StaticBlockCount +0x40 / StaticBlocks +0x44 / RecordedFrame +0x7C (Ego_r +0x44/+0x48/+0x80,
//   less 4 for the STLport CArray ClothPrimitives), N3DPrimitive::CStaticBlock (0x20: CBaseBlock::ChangeFlags +0x0A, DiffuseTexture
//   +0x14, BumpTexture +0x18, MaterialIndex +0x1C), C3DMesh2::Materials +0x8C (0x30-byte C3DMeshMaterial), C3DTextureStages
//   DiffuseTextureStage / BumpTextureStage.
// Next: the register choice flips (primitive spilled, level/bank in ebx/ebp) as soon as block 0 is held in a pointer local,
//   which is the only way to avoid the reload; 22 probes logged in probe/00ba48f0.log. Try the real C3DPrimitive2 / C3DMesh2
//   class bodies (base classes, CArray members) once engine headers exist, or a cl build with a different allocator tie-break.
// Layout: no engine headers for C3DPrimitive2 / C3DMesh2 / CStaticBlock / C3DTextureStages / C3DMeshMaterial (Ego_r members
//   restated at retail offsets; the leading bytes of C3DPrimitive2 / C3DMesh2 are their unmodelled members and bases).
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
    const C3DMeshMaterial* firstMaterial = &mesh.Materials[primitive.GetStaticBlock(0)->MaterialIndex];
    bool failed = false;
    if (stages.DiffuseTextureStage != -1)
        primitive.GetStaticBlock(0)->DiffuseTexture = const_cast<CTexture*>(GetDiffuseTexture(firstMaterial, 0, level, failed, bank, load));
    if (stages.BumpTextureStage != -1)
        primitive.GetStaticBlock(0)->BumpTexture = const_cast<CTexture*>(GetBumpTexture(firstMaterial, 0, level, failed, bank, load));
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
