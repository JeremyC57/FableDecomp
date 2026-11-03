#ifndef FABLE_ENGINE_C3DQUAD2_H
#define FABLE_ENGINE_C3DQUAD2_H
// Retail model (Codex pass 12): original Ego_r class, VertexIndex ushort[4]
// at zero; native 00ae0710 indexes these records with stride 8.
class C3DQuad2 {
public:
 unsigned long GetVertexIndex(unsigned long index)const{return VertexIndex[index];}
private:
 unsigned short VertexIndex[4];
};
#endif
