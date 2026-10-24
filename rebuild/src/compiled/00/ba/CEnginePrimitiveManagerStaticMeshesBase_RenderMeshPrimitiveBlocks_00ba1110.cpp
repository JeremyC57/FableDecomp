// CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocks(C3DPrimitive2& primitive, const C3DTextureStages& textureStages, bool usePushBuffer, bool renderDegenerateTriangles) -- draw a static mesh primitive's blocks through the index buffer, then its degenerate-triangle strips when asked
// Leaf: RenderMeshPrimitiveBlocks@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BA1110, 33 bytes, static __fastcall (primitive ecx, textureStages edx, two bool
//   stack args), `ret 8`.
// Identity: Ego_r.pdb ?RenderMeshPrimitiveBlocks@CEnginePrimitiveManagerStaticMeshesBase@@SIXAAVC3DPrimitive2@@ABVC3DTextureStages@@_N2@Z
//   (static, public, __fastcall, two bools); the two callees are the same class's private statics
//   ?RenderMeshPrimitiveBlocksIndexBuffer@...@@CIXAAVC3DPrimitive2@@ABVC3DTextureStages@@@Z 0x00B9E5B0 and
//   ?RenderMeshPrimitiveBlocksDegenerateTriangles@...@@CIX...@Z 0x00B9E750 (manifest labels agree). One caller 0x00B8C041.
//   Catalog name agrees. Parameter names are descriptive (the Ego_r name set has no parameter names).
// Shape: the first bool is never read in retail (the Ego_r name set also has a ...PushBuffer@ sibling, which this body
//   does not call); the second bool selects the degenerate-triangle pass.
class C3DPrimitive2;
class C3DTextureStages;

class CEnginePrimitiveManagerStaticMeshesBase {
public:
    static void __fastcall RenderMeshPrimitiveBlocks(C3DPrimitive2& primitive, const C3DTextureStages& textureStages,
                                                     bool usePushBuffer, bool renderDegenerateTriangles);
private:
    static void __fastcall RenderMeshPrimitiveBlocksIndexBuffer(C3DPrimitive2& primitive, const C3DTextureStages& textureStages);          // 0x00B9E5B0
    static void __fastcall RenderMeshPrimitiveBlocksDegenerateTriangles(C3DPrimitive2& primitive, const C3DTextureStages& textureStages);  // 0x00B9E750
};

void __fastcall CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocks(C3DPrimitive2& primitive,
                                                                                   const C3DTextureStages& textureStages,
                                                                                   bool usePushBuffer,
                                                                                   bool renderDegenerateTriangles)
{
    RenderMeshPrimitiveBlocksIndexBuffer(primitive, textureStages);
    if (renderDegenerateTriangles)
        RenderMeshPrimitiveBlocksDegenerateTriangles(primitive, textureStages);
}