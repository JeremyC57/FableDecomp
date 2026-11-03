#ifndef FABLE_ENGINE_C3DTRIANGLE2_H
#define FABLE_ENGINE_C3DTRIANGLE2_H
// Retail model (Codex pass 12): original Ego_r class, VertexIndex ushort[3]
// at zero; native 00ae0710 indexes these records with stride 6.
class C3DTriangle2 {
public:
 unsigned long GetVertexIndex(unsigned long index)const{return VertexIndex[index];}
private:
 unsigned short VertexIndex[3];
};
#endif
