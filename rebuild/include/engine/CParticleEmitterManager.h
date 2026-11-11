#ifndef FABLE_ENGINE_CPARTICLEEMITTERMANAGER_H
#define FABLE_ENGINE_CPARTICLEEMITTERMANAGER_H
#include "engine/CParticleEmitterList.h"
#include "engine/CPerlinNoise.h"
#include "engine/CRightHandedSet.h"
// Layout: both original PDB type records and donor Update02F3F4C0;
// native B0DC20 member accesses use the original retail container sizes.
class CIEngine;
namespace NParticleEngine {
class CParticleEmitterManager {
public:
 bool GetEnableDecals()const{return EnableDecals;}
 const NFractal::CPerlinNoise& PeekPerlinNoise()const{return PerlinNoise;}
 CParticleEmitter* CreateDecalEmitter(long,const C3DVector&,bool,const CRightHandedSet&);
protected:
 CParticleEmitterList ActiveEmitters;NFractal::CPerlinNoise PerlinNoise;
 long MaxVisibleEmitters,VisibleEmitterCount;bool Cleared:1,EnableDecals:1;CIEngine* PEngine;
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticleEmitterManager)==0x1018);
#endif
