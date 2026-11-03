#ifndef FABLE_ENGINE_C3DQUAD2_H
#define FABLE_ENGINE_C3DQUAD2_H
// C3DQuad2 (Ego_r class V, size 8): unsigned short VertexIndex[4] at +0 (FableWin ??0C3DQuad2@@QAE@GGGG@Z; 0x00AE0710 indexes the
// records with stride 8, movzx word). GetVertexIndex (same form as C3DTriangle2's) is inline in retail.
class C3DQuad2 {
public:
    unsigned long GetVertexIndex(unsigned long index) const { return VertexIndex[index]; }
private:
    unsigned short VertexIndex[4];
};
#endif
