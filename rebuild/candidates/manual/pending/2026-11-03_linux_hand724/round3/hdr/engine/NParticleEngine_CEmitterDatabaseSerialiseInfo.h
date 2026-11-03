#ifndef FABLE_ENGINE_NPARTICLEENGINE_CEMITTERDATABASESERIALISEINFO_H
#define FABLE_ENGINE_NPARTICLEENGINE_CEMITTERDATABASESERIALISEINFO_H
// New header (chunk 724 round 3): NParticleEngine::CEmitterDatabaseSerialiseInfo (Ego_r TPI size 8: GraphicSymbols +0,
//   MeshSymbols +4; FableWin ??0CEmitterDatabaseSerialiseInfo@NParticleEngine@@QAE@ABVCSymbolMap@@0@Z). CPSCDecalRenderer::
//   DatabaseSerialise 0x00B1E380 reads GraphicSymbols (`mov ecx,[info] ; mov ecx,[ecx]`) for its const symbol lookups.
#include "rebuild_abi.h"
class CSymbolMap;
namespace NParticleEngine {
class CEmitterDatabaseSerialiseInfo {
public:
    CEmitterDatabaseSerialiseInfo(const CSymbolMap&, const CSymbolMap&);
    const CSymbolMap* GraphicSymbols;                   // +0x00
    const CSymbolMap* MeshSymbols;                      // +0x04
};
} // namespace NParticleEngine
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CEmitterDatabaseSerialiseInfo) == 8);
#endif
