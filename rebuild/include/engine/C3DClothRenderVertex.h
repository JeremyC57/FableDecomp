#ifndef FABLE_ENGINE_C3DCLOTHRENDERVERTEX_H
#define FABLE_ENGINE_C3DCLOTHRENDERVERTEX_H
// NEW (chunk 2008, row 0x00BB1F50 CEngineClothRenderer::CalculateVertexNormals). Ego_r.pdb C3DClothRenderVertex (class, 4 bytes):
// PositionIndex +0, TexCoordIndex +2 (unsigned short each); CalculateVertexNormals maps every triangle / quad corner through
// PositionIndex (`movzx edi,word ptr [ecx+eax*4]`, 0x00BB204B).
class C3DClothRenderVertex {
public:
    unsigned short PositionIndex;                                        // +0x00
    unsigned short TexCoordIndex;                                        // +0x02
};
#endif
