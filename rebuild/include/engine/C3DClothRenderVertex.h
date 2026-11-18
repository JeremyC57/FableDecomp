#ifndef FABLE_ENGINE_C3DCLOTHRENDERVERTEX_H
#define FABLE_ENGINE_C3DCLOTHRENDERVERTEX_H
// NEW (chunk 2008, row 0x00BB1F50 CEngineClothRenderer::CalculateVertexNormals). Ego_r.pdb C3DClothRenderVertex (class, 4 bytes):
// PositionIndex +0, TexCoordIndex +2 (unsigned short each); CalculateVertexNormals maps every triangle / quad corner through
// PositionIndex (`movzx edi,word ptr [ecx+eax*4]`, 0x00BB204B).
class C3DClothRenderVertex {
public:
#ifdef FABLE_RETAIL_C3DCLOTHRENDERVERTEX_DEFAULT_CTOR_PASS24
    // FableWin 033587B0; native 00AE3F32 passes an existing stack word without initializing either index.
    C3DClothRenderVertex() {}
#endif
    unsigned short PositionIndex;                                        // +0x00
    unsigned short TexCoordIndex;                                        // +0x02
};
#endif
