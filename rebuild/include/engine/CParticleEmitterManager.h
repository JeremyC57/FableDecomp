#if defined(FABLE_RETAIL_CPARTICLEEMITTERMANAGER_INTERPOLATED_ONLY) && !defined(FABLE_ENGINE_CPARTICLEEMITTERMANAGER_H)
// NF158 partial mode (row 0x006E11F0 CTCDParticleEmitter::Create, which models its vectors / counted pointer inside a stand-in
// namespace and so cannot take the full header): declarations only, no includes. Ego_r.pdb 0x00AAFFF0 / FableWin.pdb 0x02EF5E50
// ?CreateInterpolatedEmitter@CParticleEmitterManager@NParticleEngine@@QAEPAVCParticleEmitter@2@JJMABVC3DVector@@ABVCRightHandedSet@@_N@Z
// (0x00AEB8C0) and the free accessor 0x00AEAAF0 (the PASS23 declaration).
#define FABLE_ENGINE_CPARTICLEEMITTERMANAGER_H
#include "rebuild_abi.h"
class C3DVector;
class CRightHandedSet;
namespace NParticleEngine {
class CParticleEmitter;
class CParticleEmitterManager {
public:
 CParticleEmitter* CreateInterpolatedEmitter(long fromHandle,long toHandle,float interpolate,const C3DVector& position,const CRightHandedSet& orientation,bool force);
};
CParticleEmitterManager& FABLE_FASTCALL GetParticleEmitterManager();
}
#endif
#ifndef FABLE_ENGINE_CPARTICLEEMITTERMANAGER_H
#define FABLE_ENGINE_CPARTICLEEMITTERMANAGER_H
#include "engine/CParticleEmitterList.h"
#include "engine/CPerlinNoise.h"
#include "engine/CRightHandedSet.h"
// Layout: both original PDB type records and donor Update02F3F4C0;
// native B0DC20 member accesses use the original retail container sizes.
class CIEngine;
#ifdef FABLE_RETAIL_CPARTICLEEMITTERMANAGER_PREPAREPRIMITIVES_PASS24
class CEnginePrimitivePrepareInfo;
#endif
#ifdef FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D
class C2DVector;
#ifdef _MSC_VER
enum EEngine2DLayer;
#else
enum EEngine2DLayer { };
#endif
#endif
namespace NParticleEngine {
#ifdef FABLE_RETAIL_CPARTICLEEMITTERMANAGER_PREPAREPRIMITIVES_PASS24
extern bool RenderingEnabled;
inline bool FABLE_FASTCALL IsRenderingEnabled() { return RenderingEnabled; }
#endif
#ifdef FABLE_RETAIL_CPARTICLEEMITTERMANAGER_BULLETTIME_PASS23
// Exact original namespace/free fastcall/reference singleton; native00aeaaf0.
class CParticleEmitterManager;
CParticleEmitterManager& FABLE_FASTCALL GetParticleEmitterManager();
#endif
class CParticleEmitterManager {
public:
#ifdef FABLE_RETAIL_CPARTICLEEMITTERMANAGER_PREPAREPRIMITIVES_PASS24
 void PreparePrimitives(const ::CEnginePrimitivePrepareInfo& info);
#endif
 bool GetEnableDecals()const{return EnableDecals;}
#ifdef FABLE_RETAIL_CPARTICLEEMITTERMANAGER_SETENABLEDECALS
 // NF158 opt-in (row 0x0088F4A0 CGameScriptInterface::EnableDecals): FableWin ?SetEnableDecals@CParticleEmitterManager@NParticleEngine@@QAEX_N@Z,
 // inline in retail (the bit-field store at +0x1010 expanded in the caller).
 void SetEnableDecals(bool enable){EnableDecals=enable;}
#endif
 const NFractal::CPerlinNoise& PeekPerlinNoise()const{return PerlinNoise;}
 CParticleEmitter* CreateDecalEmitter(long,const C3DVector&,bool,const CRightHandedSet&);
#ifdef FABLE_RETAIL_CPARTICLEEMITTERMANAGER_DEFAULTBLEND
 // Opt-in (wfD6, row 0x00B1E380): Ego_r.pdb ?GetDefaultBlendMode@CParticleEmitterManager@NParticleEngine@@QBE?BJXZ /
 // ?GetDefaultBlendOp@...@@QBE?BJXZ; inline in retail (the BlendMode / BlendOp Transfer defaults are the constants 2 and 0, as the
 // landed CPSCRenderSprite::DatabaseSerialise 0x00AFBCE0 / CPSCRenderMesh::DatabaseSerialise 0x00B027D0 bodies spell them).
 const long GetDefaultBlendMode()const{return 2;}
 const long GetDefaultBlendOp()const{return 0;}
#endif
#ifdef FABLE_RETAIL_CPARTICLEEMITTERMANAGER_DEFAULTALIGNMENT
 // Opt-in (hand3c, row 0x00B1DFD0 CPSCDecalRenderer ctor): FableWin.pdb ?GetDefaultAlignmentMode@CParticleEmitterManager@NParticleEngine@@
 // QBE?BJXZ (0x02F26960), inline in retail: the ctor's Flags default folds to 0 (0x00B1E34A `and dword ptr [esi+20h],0FFFF003Fh`,
 // as the landed CPSCRenderSprite ctor 0x00AFB7C0 spells it). No layout change.
 const long GetDefaultAlignmentMode()const{return 0;}
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
