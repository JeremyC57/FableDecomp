#ifndef FABLE_ENGINE_NPARTICLEENGINE_CPARTICLEEMITTERMANAGER_H
#define FABLE_ENGINE_NPARTICLEENGINE_CPARTICLEEMITTERMANAGER_H
// New header (chunk 724 round 3): NParticleEngine::CParticleEmitterManager (Ego_r TPI size 0x1020: ActiveEmitters +0,
//   PerlinNoise +0x10, MaxVisibleEmitters +0x1010, VisibleEmitterCount +0x1014, Cleared / EnableDecals bits +0x1018,
//   PEngine +0x101C; kept opaque, no row reads them yet) and its accessor NParticleEngine::GetParticleEmitterManager()
//   (Ego_r / FableWin ?GetParticleEmitterManager@NParticleEngine@@YIAAVCParticleEmitterManager@1@XZ, out of line at 0x00AEAAF0).
// GetDefaultBlendMode / GetDefaultBlendOp (FableWin ?GetDefaultBlendMode@CParticleEmitterManager@NParticleEngine@@QBE?BJXZ /
//   ?GetDefaultBlendOp@...@QBE?BJXZ) are inline constants in retail: CPSCDecalRenderer::DatabaseSerialise 0x00B1E816..0x00B1E82C
//   calls the accessor and then stores the default 2 (BlendMode) / 0 (BlendOp) without reading the manager.
#include "rebuild_abi.h"
namespace NParticleEngine {
class CParticleEmitterManager {
public:
    const long GetDefaultBlendMode() const { return 2; }
    const long GetDefaultBlendOp() const { return 0; }
private:
    unsigned long Storage[0x1020 / 4];
};
CParticleEmitterManager& __fastcall GetParticleEmitterManager();   // 0x00AEAAF0
} // namespace NParticleEngine
#endif
