#ifndef FABLE_ENGINE_CTVERTEXWATERBACKGROUND_H
#define FABLE_ENGINE_CTVERTEXWATERBACKGROUND_H
// Retail layout (hand-written, chunk 785): Ego_r.pdb CTVertexWaterBackground = CGenericVertex<CTVertexWaterBackgroundBase>, 56 bytes
// (short X +0x00, short Y +0x02, float Z +0x04, float ShoreLookUpArray[12] +0x08), class key V, no constructor in either PDB name
// set (a POD). Retail agrees: CArray<CTVertexWaterBackground>::push_back 0x00BE4F90 copies 0x0E dwords, and the water tesselator
// 0x00BE4660 indexes the bridge buffer with `imul 38h` and fills X/Y (16-bit), Z and the 12-float array. The CGenericVertex
// wrapper is flattened (it adds no members).
class CTVertexWaterBackground {
public:
    short X;                                                     // +0x00
    short Y;                                                     // +0x02
    float Z;                                                     // +0x04
    float ShoreLookUpArray[12];                                  // +0x08
};

#endif // FABLE_ENGINE_CTVERTEXWATERBACKGROUND_H
