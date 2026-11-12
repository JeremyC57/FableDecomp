#ifndef FABLE_ENGINE_CPARTICLEEMITTERMANAGER_H
#define FABLE_ENGINE_CPARTICLEEMITTERMANAGER_H
#include "engine/CParticleEmitterList.h"
#include "engine/CPerlinNoise.h"
#include "engine/CRightHandedSet.h"
// Layout: both original PDB type records and donor Update02F3F4C0;
// native B0DC20 member accesses use the original retail container sizes.
class CIEngine;
#ifdef FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D
class C2DVector;
#ifdef _MSC_VER
enum EEngine2DLayer;
#else
enum EEngine2DLayer { };
#endif
#endif
namespace NParticleEngine {
class CParticleEmitterManager {
public:
 bool GetEnableDecals()const{return EnableDecals;}
 const NFractal::CPerlinNoise& PeekPerlinNoise()const{return PerlinNoise;}
 CParticleEmitter* CreateDecalEmitter(long,const C3DVector&,bool,const CRightHandedSet&);
#ifdef FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D
 // Opt-in (chunk Q28, row 0x00AEB790): Ego_r.pdb / FableWin.pdb ?Create2DEmitter@CParticleEmitterManager@NParticleEngine@@
 // QAEPAVCParticleEmitter@2@JABVC2DVector@@W4EEngine2DLayer@@_NABVCRightHandedSet@@@Z. Declaration only.
 CParticleEmitter* Create2DEmitter(long handle,const C2DVector& position,EEngine2DLayer layer,bool start,const CRightHandedSet& orientation);
#endif
protected:
 CParticleEmitterList ActiveEmitters;NFractal::CPerlinNoise PerlinNoise;
 long MaxVisibleEmitters,VisibleEmitterCount;bool Cleared:1,EnableDecals:1;CIEngine* PEngine;
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticleEmitterManager)==0x1018);
#endif
