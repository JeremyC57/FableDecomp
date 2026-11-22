#ifndef FABLE_ENGINE_CPARTICLESYSTEM_H
#define FABLE_ENGINE_CPARTICLESYSTEM_H
#include "fable_compiler.h"
#include "engine/CBaseClass.h"
#include "engine/CParticleEmitter.h"
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
#include "engine/CParticleUpdateParameters.h"
#include "engine/CComponentInstance.h"
#endif
#ifdef FABLE_RETAIL_CPARTICLESYSTEM
#include "engine/CMatrix3x4.h"
#include "engine/C2DVector.h"
#endif
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_PEEKEMITTERTRANSFORM
#include "engine/CMatrix3x4.h"
#endif
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_SCALE
#include "engine/C3DVector.h"
#endif
class CIEngine;
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_INVERSE_TRANSLATE_PASS26
class CMatrix3x4;
#endif
namespace NParticleEngine {
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_ADDCOMPONENT
class CPSCBase;
#endif
#ifdef FABLE_RETAIL_CPARTICLESYSTEM
class CParticle;
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
class CDecal;
#endif
#endif
// Layout: native ctor00af4e10 and original TPI complete fields; size0x70.
class CParticleSystem:public CBaseClass {
public:
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_INVERSE_TRANSLATE_PASS26
 // Both PDBs, donor02f5e230 caller; native00b201d0.
 void GetInverseSystemTransformWithTranslate(CMatrix3x4&)const;
#endif
public:CParticleSystem();virtual ~CParticleSystem();
 void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);
 void SetParentEmitter(const CParticleEmitter& p){PParentEmitter=&p;}
#ifdef FABLE_RETAIL_CPARTICLESYSTEM
 // Retail opt-in (chunk 737, CPSCLight / CPSCSingleSprite::PreparePrimitives 0x00B19C60 / 0x00B087C0): FableWin.pdb members.
 // Out of line in retail: ?GetSystemTransform@CParticleSystem@NParticleEngine@@QBEXAAVCMatrix3x4@@@Z 0x00AF2250 and
 // ?ScaleParticleDimensions@CParticleSystem@NParticleEngine@@QBEXAAM@Z 0x00AF8F80 (both landed). Inline (no retail copy):
 // ?GetGlobalAlpha@...@QBEMXZ / ?GetFadeAlpha@...@QBEMXZ (+0x54 / +0x58 reads) and ?TransformPosIfWorld@...@QBEXAAVC3DVector@@@Z
 // (debug copy 0x02F384F0 in the CPSCSingleSprite TU; __forceinline: plain inline is called out of line in the 2.5 KB rows): with the system not StayWithEmitter (+0x50 bit 6 clear) the
 // position is moved through GetSystemTransform's matrix (CMatrix3x4::TransformPoint; the retail rows read it with FABLE_RETAIL_CMATRIX3X4) (both PreparePrimitives rows; 0x00B18AF0 AddToBoundingSphere).
 void GetSystemTransform(CMatrix3x4&) const;
 #ifdef FABLE_RETAIL_CPARTICLESYSTEM_SCALEPARTICLEDIMENSIONS_FLOAT_INLINE
 // Opt-in (hand3c, row 0x00B05C60 CPSCSingleSprite::GetComponentCost; needs FABLE_RETAIL_CPARTICLESYSTEM_SCALE): the body is a header
 // inline -- Ego_r.pdb / FableWin.pdb place ?ScaleParticleDimensions@...@QBEXAAM@Z (and the C2DVector& / unsigned long& overloads) in
 // the first psc_*.obj that uses each, never in particle_system.obj (COMDATs), and FableWin /Od 0x02F26DC0 evaluates the inner max
 // twice (a max macro nested in a max macro), as the landed out-of-line copy 0x00AF8F80 spells it. VC7.1 declines to inline this
 // body (retail calls 0x00AF8F80 from 0x00B05D69 / 0x00B05DE0), and seeing it gives retail's frame (the size homed in the dead
 // `system` argument slot; declaration only: one extra frame word).
 void ScaleParticleDimensions(float& size) const
 {
  if (ScaleParticles)
   size *= ((Scale.X > Scale.Y ? Scale.X : Scale.Y) > Scale.Z ? (Scale.X > Scale.Y ? Scale.X : Scale.Y) : Scale.Z);
 }
 #else
 void ScaleParticleDimensions(float&) const;
 #endif
 // CPSCSingleSprite::PreparePrimitives 0x00B087C0: ?ScaleParticleDimensions@...@QBEXAAVC2DVector@@@Z 0x00AF8F00 (the sprite size),
 // ?ScaleParticleDimensions@...@QBEXAAK@Z 0x00B05B80 (the trail length) and ?GetUnscaledWorldTransform@...@QBEXAAVCMatrix3x4@@@Z
 // 0x00B05840, out of line; ?GetNormalisedTime@...@QBEMXZ inline (the +0x50 bit 3 test, then the +0x5C read).
 #ifdef FABLE_RETAIL_CPARTICLESYSTEM_SCALEPARTICLEDIMENSIONS_C2DVECTOR_INLINE
 // Opt-in (hand3c3 row 0x00B075D0 CPSCSingleSprite::AddToBoundingSphere, audit 2026-11-20; needs FABLE_RETAIL_CPARTICLESYSTEM_SCALE and
 // engine/C2DVector.h): the C2DVector& overload as the header inline it is -- Ego_d.pdb lines 243..250 of the particle-system header
 // (244 the test, 246 `scale`, 247 / 248 the two products; parameter `dim`, local `scale`), a COMDAT of the first psc_*.obj that
 // uses it, like the float overload above. VC7.1 declines to inline this body (retail calls 0x00AF8F00 from 0x00B0794B) but must
 // see it: declaration only is 976v977. The emitted COMDAT equals retail 0x00AF8F00 (117 bytes, no relocation).
 void ScaleParticleDimensions(C2DVector& dim) const
 {
  if (ScaleParticles)
  {
   float scale = ((Scale.X > Scale.Y ? Scale.X : Scale.Y) > Scale.Z ? (Scale.X > Scale.Y ? Scale.X : Scale.Y) : Scale.Z);
   dim.X *= scale;
   dim.Y *= scale;
  }
 }
 #else
 void ScaleParticleDimensions(C2DVector&) const;
 #endif
 void ScaleParticleDimensions(unsigned long&) const;
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_SCALEVECTOR
 // Opt-in (wfD6, row 0x00B01900): FableWin ?ScaleParticleDimensions@CParticleSystem@NParticleEngine@@QBEXAAVC3DVector@@@Z, inline in retail
 // (no retail copy; CPSCRenderMesh::AddToBoundingSphere 0x00B01900 scales the mesh size per axis). Needs a typed Scale (FABLE_RETAIL_CPARTICLESYSTEM_SCALE).
 void ScaleParticleDimensions(C3DVector& dim) const { if (ScaleParticles) { dim.X *= Scale.X; dim.Y *= Scale.Y; dim.Z *= Scale.Z; } }
#endif
 void GetUnscaledWorldTransform(CMatrix3x4&) const;
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_PARTICLEWORLDTRANSFORM
 // Opt-in (wfE8, row 0x00B03CA0): FableWin.pdb ?GetParticleWorldTransform@CParticleSystem@NParticleEngine@@QBEXAAVCMatrix3x4@@@Z, out of line
 // (retail 0x00B05980, landed). Declaration only.
 void GetParticleWorldTransform(CMatrix3x4&) const;
#endif
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_GETSCALE
 // Opt-in (wfE8, row 0x00B03CA0): FableWin.pdb ?GetScale@CParticleSystem@NParticleEngine@@QBEABVC3DVector@@XZ (inline; no Ego_r copy).
 // Needs the typed Scale (FABLE_RETAIL_CPARTICLESYSTEM_UPDATE / _SCALE).
 const C3DVector& GetScale() const { return Scale; }
#endif
 float GetNormalisedTime() const { return NormalisedTimeEnabled ? NormalisedTime : 0.0f; }
 float GetGlobalAlpha() const { return GlobalAlpha; }
 float GetFadeAlpha() const { return FadeAlpha; }
 FABLE_FORCEINLINE void TransformPosIfWorld(C3DVector& pos) const { if (!StayWithEmitter) { CMatrix3x4 transform; GetSystemTransform(transform); transform.TransformPoint(pos); } }
 // Chunk 739 (CPSCUpdateNormal::UpdateAddParticle 0x00B0CAD0 / Update 0x00B0DC20): FableWin.pdb
 // ?AllocateParticle@CParticleSystem@NParticleEngine@@QAEPAVCParticle@2@XZ (out of line, landed 0x00AF5010) and
 // ?PeekEmitterVelocity@CParticleSystem@NParticleEngine@@QBEABVC3DVector@@XZ (out of line at 0x00AF3DB0: `mov eax,[ecx+4] ;
 // add eax,52h ; ret`, the parent emitter's Velocity -- Ego_r.pdb +0x5E less the 0xC the STLport members save).
 CParticle* AllocateParticle();
 const C3DVector& PeekEmitterVelocity() const;
 // Chunk 742 (CParticleEmitter::ReadBinary 0x00AEFAA0): FableWin.pdb ?ReadBinary@CParticleSystem@NParticleEngine@@QAEXAAVCDataInputStream@@@Z
 // (out of line, 0x00AF4660) and ?IsEnabled@CParticleSystem@NParticleEngine@@QBE_NXZ (inline: `test byte [ebp+50h],2`; FableWin
 // places its debug COMDAT 0x02F0DB90 right after CParticleEmitter::ReadBinary 0x02F0D3A0).
 void ReadBinary(CDataInputStream&);
 bool IsEnabled() const { return Enabled; }
#endif
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_ADDCOMPONENT
 // Opt-in (chunk 2001, row 0x00AF4900 DatabaseSerialise): Ego_r.pdb / FableWin.pdb
 // ?AddComponent@CParticleSystem@NParticleEngine@@QAEAAVCPSCBase@2@ABVCCharString@@ABK_N2@Z (0x00AF4530; (type name, name CRC, enabled,
 // visible), returns the new component). Declaration only.
 CPSCBase& AddComponent(const CCharString& type, const unsigned long& nameCRC, bool enabled, bool visible);   // 0x00AF4530
#endif
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_INTERPOLATETO
 // Opt-in (chunk 2001, row 0x00AED600 CParticleEmitter::InterpolateTo): Ego_r.pdb / FableWin.pdb
 // ?InterpolateTo@CParticleSystem@NParticleEngine@@QAEXABV12@M@Z (public non-virtual, out of line at 0x00AF4100; its only caller is
 // 0x00AED623). Declaration only: no layout change.
 void InterpolateTo(const CParticleSystem& other, float t);
#endif
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
 // PDB accessors, donor02F3F4C0 and native AF5330/AF5380/AF22C0.
 const CParticleEmitter& GetParentEmitter()const{return *PParentEmitter;}
 typedef std::list<CParticle*>::iterator ParticleIterator;
 void ClearParticle(ParticleIterator&);
 std::list<CParticle*>& GetParticleList(){return Particles;}
 void SetGlobalAlpha(const float alpha){GlobalAlpha=alpha;}
 void SetNormalisedTime(bool enabled,float value){NormalisedTimeEnabled=enabled;NormalisedTime=value;}
 void GetInverseSystemTransform(CMatrix3x4&)const;
 std::list<CParticle*>::iterator DeleteParticle(std::list<CParticle*>::iterator&);
 CDecal* AllocateDecal();
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_CLEARDECAL
 // Opt-in (row 0x00AF5430): Ego_r.pdb + FableWin.pdb ?ClearDecal@CParticleSystem@NParticleEngine@@QAEXAAV?$_List_iterator@..CDecal..@std@@@Z.
 void ClearDecal(std::list<CDecal*>::iterator&);
#endif
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_GETDECALLIST
 // Opt-in (wfD6, row 0x00B1F200): FableWin.pdb ?GetDecalList@CParticleSystem@NParticleEngine@@QAEAAV?$list@PAVCDecal@NParticleEngine@@..@std@@XZ
 // (inline; the row reads the list head at +0x20 in place, as GetParticleList).
 std::list<CDecal*>& GetDecalList(){return Decals;}
#endif
 void TransformVectorIfLocal(C3DVector& v)const{if(StayWithEmitter){CMatrix3x4 t;GetSystemTransform(t);t.TransformVector(v);}}
 
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_BOUNDING_TRANSFORM_PASS24
    // Inline-budget: native 00AFB213, 00AFB381 and 00AFB537 expands the original member in CPSCRenderSprite::AddToBoundingSphere.
    FABLE_FORCEINLINE
#endif
    void TransformPosIfLocal(C3DVector& v)const{if(StayWithEmitter){CMatrix3x4 t;GetSystemTransform(t);t.TransformPoint(v);}}
 void InverseTransformPosIfLocal(C3DVector& v)const{if(StayWithEmitter){CMatrix3x4 t;GetInverseSystemTransform(t);t.TransformPoint(v);}}
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_GENERATEPARTICLE
 // Retail opt-in (wfE8 6th agent, row 0x00B10B10 CPSCEmitterGeneric::GenerateParticle): FableWin.pdb ?TransformVectorIfWorld@ / ?InverseTransformVector
 // IfLocal@CParticleSystem@NParticleEngine@@QBEXAAVC3DVector@@@Z (inline, the siblings TransformVectorIfLocal / InverseTransformPosIfLocal
 // text) and ?GetComponentOfType@..@@QAEPAVCPSCBase@2@W4EPSCType@2@K@Z (0x00AF4360, out of line). No layout change.
 void TransformVectorIfWorld(C3DVector& v)const{if(!StayWithEmitter){CMatrix3x4 t;GetSystemTransform(t);t.TransformVector(v);}}
 void InverseTransformVectorIfLocal(C3DVector& v)const{if(StayWithEmitter){CMatrix3x4 t;GetInverseSystemTransform(t);t.TransformVector(v);}}
 CPSCBase* GetComponentOfType(EPSCType type,unsigned long index);   // 0x00AF4360
#endif
#endif
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_PEEKEMITTERTRANSFORM
 // Opt-in (agentNF22, row 0x00AF3D80): Ego_r.pdb + FableWin.pdb ?PeekEmitterTransform@CParticleSystem@NParticleEngine@@ABEABVCMatrix3x4@@XZ
 // (private const), out of line at 0x00AF3D80: `mov eax,[ecx+4] ; add eax,0B0h ; ret` -- the parent emitter's Transform (+0xb0).
 // Callers: GetSystemTransform 0x00AF2250, GetUnscaledWorldTransform 0x00B05840, GetInverseSystemTransformWithTranslate 0x00B201D0.
private:
 const CMatrix3x4& PeekEmitterTransform() const;
#endif
protected:
 const CParticleEmitter* PParentEmitter;CIEngine* PEngine;
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
 CVectorMap<EPSCType,CComponentInstance,CKeyPairCompareLess<EPSCType,CComponentInstance> > Components;
 std::list<CParticle*> Particles;std::list<CDecal*> Decals;
 CParticleUpdateParameters<unsigned long,unsigned long> UpdateParameters;
#else
 unsigned char Components[16],Particles[4],Decals[4],UpdateParameters[36];
#endif
 long NoActiveParticles,NoActiveDecals;
 bool Initialised:1,Enabled:1,Dead:1,NormalisedTimeEnabled:1,UseAllAttractors:1,Visible:1,StayWithEmitter:1,ScaleParticles:1;
 float GlobalAlpha,FadeAlpha,NormalisedTime;
// FABLE_RETAIL_CPARTICLESYSTEM_SCALE (chunk 2001, row 0x00AF4900 DatabaseSerialise: ScaleX/Y/Z at +0x60/+0x64/+0x68) types the member alone.
#if defined(FABLE_RETAIL_CPARTICLESYSTEM_UPDATE) || defined(FABLE_RETAIL_CPARTICLESYSTEM_SCALE)
 C3DVector Scale;
#else
 unsigned char Scale[12];
#endif
 CCharString Name;
};
}
FABLE_STATIC_ASSERT_AT(126, sizeof(NParticleEngine::CParticleSystem)==0x70);
#endif
