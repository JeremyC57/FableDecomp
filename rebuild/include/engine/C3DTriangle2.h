#ifndef FABLE_ENGINE_C3DTRIANGLE2_H
#define FABLE_ENGINE_C3DTRIANGLE2_H
// C3DTriangle2 (Ego_r class V): unsigned short VertexIndex[3] at +0, size 6 (0x00AE0710 / 0x00AC2BE0 index the records with
// stride 6, movzx word). GetVertexIndex: FableWin ?GetVertexIndex@C3DTriangle2@@QBEKK@Z 0x02EBE3D0, inline in retail.
class C3DTriangle2 {
public:
#ifdef FABLE_RETAIL_PHYSICS_MESH_READER_PASS30
    // Original FableWin setters take unsigned long and store a 16-bit index.
    void SetA(unsigned long value) { VertexIndex[0] = static_cast<unsigned short>(value); }
    void SetB(unsigned long value) { VertexIndex[1] = static_cast<unsigned short>(value); }
    void SetC(unsigned long value) { VertexIndex[2] = static_cast<unsigned short>(value); }
#endif
    unsigned long GetVertexIndex(unsigned long index) const { return VertexIndex[index]; }
#ifdef FABLE_RETAIL_QUADRIC_COPY_TO_PASS30
    C3DTriangle2(unsigned short a,unsigned short b,unsigned short c) { VertexIndex[0]=a; VertexIndex[1]=b; VertexIndex[2]=c; }
#endif
#ifdef FABLE_RETAIL_C3DPRIMITIVE_CORNER_ACCESSORS
    // Retail opt-in (chunk 3017, row 0x00BB1F50): FableWin.pdb ?GetA/B/C@C3DTriangle2@@QBEKXZ, the corner accessors the cloth TU instantiates
    // (debug COMDATs listed after CalculateVertexNormals, 0x02E77990..).
    unsigned long GetA() const { return VertexIndex[0]; }
    unsigned long GetB() const { return VertexIndex[1]; }
    unsigned long GetC() const { return VertexIndex[2]; }
#endif
#ifdef FABLE_RETAIL_C3DTRIANGLE2_SETVERTEXINDEX
    // Retail opt-in (wfE6, row 0x00B6EDF0 BuildConstantBuffers): FableWin.pdb ?SetVertexIndex@C3DTriangle2@@QAEXKG@Z (0x0333C310), inline
    // in retail: the row writes the ten triangles' indices as in-order immediate word stores.
    void SetVertexIndex(unsigned long index, unsigned short value) { VertexIndex[index] = value; }
#endif
private:
    unsigned short VertexIndex[3];
};
#endif
