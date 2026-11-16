// 00aeeba0 NParticleEngine::CParticleEmitter::StartFrame
// Leaf: ?StartFrame@CParticleEmitter@NParticleEngine@@QAEXMABVCEnginePrimitivePrepareInfo@@@Z
// Flags: /d2inlT262
// Inline-budget: native expands the inverse; ordinary helper at T261 gives462B, T262 gives890B. Existing grouped cross preserves the native scheduling region.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CPARTICLEEMITTER_STARTFRAME_PASS23
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CPARTICLE_SPRITE
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_CRIGHTHANDEDSET_INTERPOLATE
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_GETINTERPOLATED
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_BASE
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_PASS22_GETINTERPOLATED
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_PLAIN_INLINE_PASS23
#define FABLE_RETAIL_CMATRIX3X4_PARTICLE_ADD_SET
#define FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_PASS23
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#include "engine/CParticleEmitter.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CAEnvironmentInteraction.h"
namespace NParticleEngine {
CAEnvironmentInteraction& __fastcall GetEnvironmentInteraction();
void CParticleEmitter::StartFrame(const float interpolate,const CEnginePrimitivePrepareInfo& info){
 if(IsSleeping())return;
 if(IsWaitingToSleep()&&GetNoParticles()==0&&GetNoDecals()==0){
  ClearAllPrimitives(&info.Engine);Sleeping=true;return;
 }
 C3DVector current_pos=PositionInterpolator.GetInterpolated(interpolate);
 Transform.Set(OrientationInterpolator.GetInterpolated(interpolate),current_pos);
 InverseTransform.SetInverseNoTranslate(Transform);
 if(Is2DEmitter())DistanceToCam=0.0f;
 else{
  C3DVector camera_pos;
  GetEnvironmentInteraction().GetCameraPos(camera_pos);
  DistanceToCam=(camera_pos-current_pos).GetAccurateMagnitude();
 }
}
}