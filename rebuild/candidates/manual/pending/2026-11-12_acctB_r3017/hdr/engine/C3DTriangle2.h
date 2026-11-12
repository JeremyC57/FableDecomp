#ifndef FABLE_ENGINE_C3DTRIANGLE2_H
#define FABLE_ENGINE_C3DTRIANGLE2_H
// C3DTriangle2 (Ego_r class V): unsigned short VertexIndex[3] at +0, size 6 (0x00AE0710 / 0x00AC2BE0 index the records with
// stride 6, movzx word). GetVertexIndex: FableWin ?GetVertexIndex@C3DTriangle2@@QBEKK@Z 0x02EBE3D0, inline in retail.
class C3DTriangle2 {
public:
    unsigned long GetVertexIndex(unsigned long index) const { return VertexIndex[index]; }
#ifdef FABLE_RETAIL_C3DPRIMITIVE_CORNER_ACCESSORS
    // Retail opt-in (chunk 3017, row 0x00BB1F50): FableWin.pdb ?GetA/B/C@C3DTriangle2@@QBEKXZ, the corner accessors the cloth TU instantiates
    // (debug COMDATs listed after CalculateVertexNormals, 0x02E77990..).
    unsigned long GetA() const { return VertexIndex[0]; }
    unsigned long GetB() const { return VertexIndex[1]; }
    unsigned long GetC() const { return VertexIndex[2]; }
#endif
private:
    unsigned short VertexIndex[3];
};
#endif
