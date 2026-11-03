#ifndef FABLE_ENGINE_C3DTRIANGLE2_H
#define FABLE_ENGINE_C3DTRIANGLE2_H
#ifdef FABLE_RETAIL_C3DTRIANGLE2
// Hand-written (chunk 723): C3DTriangle2, three 16-bit vertex indices (Ego_r.pdb: unsigned short VertexIndex[3], 6 bytes). Members
// from Ego_r.pdb / FableWin.pdb publics; all inline in retail (0x00B6EDF0 writes the indices as immediate word stores).
#include "rebuild_abi.h"
class C3DTriangle2 {
public:
    C3DTriangle2() { }                                                   // ??0C3DTriangle2@@QAE@XZ
    C3DTriangle2(unsigned short a, unsigned short b, unsigned short c)   // ??0C3DTriangle2@@QAE@GGG@Z
    {
        VertexIndex[0] = a;
        VertexIndex[1] = b;
        VertexIndex[2] = c;
    }
    unsigned long GetA() const { return VertexIndex[0]; }                // ?GetA@C3DTriangle2@@QBEKXZ
    unsigned long GetB() const { return VertexIndex[1]; }                // ?GetB@C3DTriangle2@@QBEKXZ
    unsigned long GetC() const { return VertexIndex[2]; }                // ?GetC@C3DTriangle2@@QBEKXZ
    unsigned long GetVertexIndex(unsigned long i) const { return VertexIndex[i]; }   // ?GetVertexIndex@C3DTriangle2@@QBEKK@Z
    void SetA(unsigned long v) { VertexIndex[0] = (unsigned short)v; }   // ?SetA@C3DTriangle2@@QAEXK@Z
    void SetB(unsigned long v) { VertexIndex[1] = (unsigned short)v; }   // ?SetB@C3DTriangle2@@QAEXK@Z
    void SetC(unsigned long v) { VertexIndex[2] = (unsigned short)v; }   // ?SetC@C3DTriangle2@@QAEXK@Z
    void SetVertexIndex(unsigned long i, unsigned short v) { VertexIndex[i] = v; }   // ?SetVertexIndex@C3DTriangle2@@QAEXKG@Z
    unsigned short VertexIndex[3];                                       // +0x00
};
FABLE_STATIC_ASSERT(sizeof(C3DTriangle2) == 6);
#else
// C3DTriangle2 (Ego_r class V): unsigned short VertexIndex[3] at +0, size 6 (0x00AE0710 / 0x00AC2BE0 index the records with
// stride 6, movzx word). GetVertexIndex: FableWin ?GetVertexIndex@C3DTriangle2@@QBEKK@Z 0x02EBE3D0, inline in retail.
class C3DTriangle2 {
public:
    unsigned long GetVertexIndex(unsigned long index) const { return VertexIndex[index]; }
private:
    unsigned short VertexIndex[3];
};
#endif // FABLE_RETAIL_C3DTRIANGLE2
#endif
