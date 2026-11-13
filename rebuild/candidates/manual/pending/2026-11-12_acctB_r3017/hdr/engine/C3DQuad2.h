#ifndef FABLE_ENGINE_C3DQUAD2_H
#define FABLE_ENGINE_C3DQUAD2_H
// C3DQuad2 (Ego_r class V, size 8): unsigned short VertexIndex[4] at +0 (FableWin ??0C3DQuad2@@QAE@GGGG@Z; 0x00AE0710 indexes the
// records with stride 8, movzx word). GetVertexIndex (same form as C3DTriangle2's) is inline in retail.
class C3DQuad2 {
public:
    unsigned long GetVertexIndex(unsigned long index) const { return VertexIndex[index]; }
#ifdef FABLE_RETAIL_C3DPRIMITIVE_CORNER_ACCESSORS
    // Retail opt-in (chunk 3017, row 0x00BB1F50): FableWin.pdb ?GetA/B/C/D@C3DQuad2@@QBEKXZ, the corner accessors the cloth TU instantiates
    // (debug COMDATs listed after CalculateVertexNormals, 0x02E77990..).
    unsigned long GetA() const { return VertexIndex[0]; }
    unsigned long GetB() const { return VertexIndex[1]; }
    unsigned long GetC() const { return VertexIndex[2]; }
    unsigned long GetD() const { return VertexIndex[3]; }
#endif
private:
    unsigned short VertexIndex[4];
};
#endif
