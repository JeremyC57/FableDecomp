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
#ifdef FABLE_RETAIL_CPARTICLEEMITTERMANAGER_BULLETTIME_PASS23
// Exact original namespace/free fastcall/reference singleton; native00aeaaf0.
class CParticleEmitterManager;
CParticleEmitterManager& FABLE_FASTCALL GetParticleEmitterManager();
#endif
class CParticleEmitterManager {
public:
 bool GetEnableDecals()const{return EnableDecals;}
 const NFractal::CPerlinNoise& PeekPerlinNoise()const{return PerlinNoise;}
 CParticleEmitter* CreateDecalEmitter(long,const C3DVector&,bool,const CRightHandedSet&);
#ifdef FABLE_RETAIL_CPARTICLEEMITTERMANAGER_DEFAULTBLEND
 // Opt-in (wfD6, row 0x00B1E380): Ego_r.pdb ?GetDefaultBlendMode@CParticleEmitterManager@NParticleEngine@@QBE?BJXZ /
 // ?GetDefaultBlendOp@...@@QBE?BJXZ; inline in retail (the BlendMode / BlendOp Transfer defaults are the constants 2 and 0, as the
 // landed CPSCRenderSprite::DatabaseSerialise 0x00AFBCE0 / CPSCRenderMesh::DatabaseSerialise 0x00B027D0 bodies spell them).
 const long GetDefaultBlendMode()const{return 2;}
 const long GetDefaultBlendOp()const{return 0;}
#endif
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
FABLE_STATIC_ASSERT_AT(33, sizeof(NParticleEngine::CParticleEmitterManager)==0x1018);
#endif
