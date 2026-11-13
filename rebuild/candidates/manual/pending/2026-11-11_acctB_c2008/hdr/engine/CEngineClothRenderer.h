#ifndef FABLE_ENGINE_CENGINECLOTHRENDERER_H
#define FABLE_ENGINE_CENGINECLOTHRENDERER_H
// NEW (chunk 2008, row 0x00BB1F50 CalculateVertexNormals). Ego_r.pdb CEngineClothRenderer (class, polymorphic: ??1 / ??_G UAE, FrameStart
// UAE) at retail offsets: the six CArray<CVertexShader> members are 12-byte STLport vectors (Ego_r 16), so every member from the second
// one on is 4 bytes lower per array: VertexNormals +0x84 (Ego_r +0x9C), PositionBuffer +0xC4 (+0xDC), NormalBufferPool +0xCC (+0xE4),
// NormalBuffer +0xE8 (+0x100), NormalBufferSize +0xEC (+0x104) -- GetNormalBuffer 0x00BB1E90 compares the size at +0xEC, allocates from the
// pool at +0xCC and returns +0xE8; CalculateVertexNormals stores VertexNormals +0x84 and reads PositionBuffer +0xC4. The members before
// +0x84 (shaders, CVertexShader arrays) stay opaque here. Methods: Ego_r.pdb / FableWin.pdb spellings (private AAE); declarations only.
#include "rebuild_abi.h"
class C4DVector;
class CParticleSimulation;
class CParticleProgram;
class CEngineClothRenderer {
public:
    virtual ~CEngineClothRenderer();
    virtual void FrameStart();
private:
    C4DVector* GetNormalBuffer(unsigned long count);                    // 0x00BB1E90 ?GetNormalBuffer@CEngineClothRenderer@@AAEPAVC4DVector@@K@Z
    void CalculateVertexNormals(const CParticleSimulation& simulation, const CParticleProgram& program);   // 0x00BB1F50
    unsigned char _shaders_0x04[0x84 - 0x04];                            // +0x04 TriVertexShaders .. DebugVertexShader (not modelled)
    C4DVector*    VertexNormals;                                         // +0x84
    bool          ClothDebug;                                            // +0x88
    unsigned char _pad_0x8c[0xc4 - 0x8c];                                // +0x8c IndexBuffer, IndexBufferSize, EnableCloth, PositionBufferPool
    C4DVector*    PositionBuffer;                                        // +0xc4
    unsigned long PositionBufferSize;                                    // +0xc8
    unsigned char NormalBufferPool[0x1c];                                // +0xcc CSimpleMemoryPool
    C4DVector*    NormalBuffer;                                          // +0xe8
    unsigned long NormalBufferSize;                                      // +0xec
};
FABLE_STATIC_ASSERT(sizeof(CEngineClothRenderer) == 0xf0);
#endif
