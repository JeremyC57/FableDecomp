#ifndef FABLE_ENGINE_CENGINEPRIMITIVEMANAGERPARTICLEGROUP_H
#define FABLE_ENGINE_CENGINEPRIMITIVEMANAGERPARTICLEGROUP_H
// New header (chunk 2045, row 0x00BB8EF0): CEnginePrimitiveManagerParticleGroup : CEnginePrimitiveManager (Fable.exe RTTI
// .?AVCEnginePrimitiveManagerParticleGroup@@, vtable 0x012A5BB8; pdb_class_keys V), Ego_r.pdb sizeof 0x28 (= retail: the base is 0xC in
// both). Own members at their Ego_r.pdb offsets: PointLightsSet +0x0C, PointLightCount (signed char) +0x0D, CurrentWorldPos +0x0E (byte
// packed: the landed SetUpPointLights 0x00BBC0B0 / RenderSubPrimitive 0x00BBC460 store dwords at +0x0E / +0x12 / +0x16), CurrentBSRadius
// +0x1C, ClipEffectToWeatherMask +0x20, ReadZBuffer +0x21, WriteZBuffer +0x22, DisplayBoundingBox +0x23, DisplayBoundingSphere +0x24.
// Member functions: Ego_r.pdb / FableWin.pdb spellings (?SetUpPointLights@CEnginePrimitiveManagerParticleGroup@@QAEJXZ, landed 0x00BBC0B0;
// ?SetupPixelShader@..@@QAEXKABVCPixelShader@@0@Z 0x00BBCC70). Declarations only (the landed rows keep their local models).
#include "rebuild_abi.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/C3DVector.h"
class CPixelShader;
#pragma pack(push, 1)
class CEnginePrimitiveManagerParticleGroup : public CEnginePrimitiveManager {
public:
    long SetUpPointLights();                                             // 0x00BBC0B0
    void SetupPixelShader(unsigned long index, const CPixelShader& stipple, const CPixelShader& normal);   // 0x00BBCC70
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEGROUP_GETREADZBUFFER
    // Opt-in (chunk 2059, row 0x00BC25F0): FableWin.pdb ?GetReadZBuffer@CEnginePrimitiveManagerParticleGroup@@QAE_NXZ (17b/9i, listed right
    // after CEnginePrimitiveManagerParticleMeshGroup::RenderSubPrimitive: first user), inline in retail (0x00BC2681 reads +0x21 in place).
    bool GetReadZBuffer() { return ReadZBuffer; }
#endif
private:
    bool          PointLightsSet;                                        // +0x0c
    signed char   PointLightCount;                                       // +0x0d
    C3DVector     CurrentWorldPos;                                       // +0x0e
    unsigned char _pad_0x1a[2];                                          // +0x1a
    float         CurrentBSRadius;                                       // +0x1c
    bool          ClipEffectToWeatherMask;                               // +0x20
    bool          ReadZBuffer;                                           // +0x21
    bool          WriteZBuffer;                                          // +0x22
    bool          DisplayBoundingBox;                                    // +0x23
    bool          DisplayBoundingSphere;                                 // +0x24
    unsigned char _pad_0x25[3];                                          // +0x25
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManagerParticleGroup) == 0x28);
#endif // FABLE_ENGINE_CENGINEPRIMITIVEMANAGERPARTICLEGROUP_H
