#ifndef FABLE_ENGINE_C3DTRIANGLE2_H
#define FABLE_ENGINE_C3DTRIANGLE2_H
// C3DTriangle2 (Ego_r class V): unsigned short VertexIndex[3] at +0, size 6 (0x00AE0710 / 0x00AC2BE0 index the records with
// stride 6, movzx word). GetVertexIndex: FableWin ?GetVertexIndex@C3DTriangle2@@QBEKK@Z 0x02EBE3D0, inline in retail.
class C3DTriangle2 {
public:
    unsigned long GetVertexIndex(unsigned long index) const { return VertexIndex[index]; }
private:
    unsigned short VertexIndex[3];
};
#endif
