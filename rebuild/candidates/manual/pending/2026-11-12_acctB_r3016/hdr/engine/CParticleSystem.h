#ifndef FABLE_ENGINE_CPARTICLESYSTEM_H
#define FABLE_ENGINE_CPARTICLESYSTEM_H
#include "fable_compiler.h"
#include "engine/CBaseClass.h"
#include "engine/CParticleEmitter.h"
#ifdef FABLE_RETAIL_CPARTICLESYSTEM
#include "engine/CMatrix3x4.h"
#include "engine/C2DVector.h"
#endif
class CIEngine;
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_SCALE
#include "engine/C3DVector.h"
#endif
namespace NParticleEngine {
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_ADDCOMPONENT
class CPSCBase;
#endif
#ifdef FABLE_RETAIL_CPARTICLESYSTEM
class CParticle;
#endif
// Layout: native ctor00af4e10 and original TPI complete fields; size0x70.
class CParticleSystem:public CBaseClass {
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
 void ScaleParticleDimensions(float&) const;
 // CPSCSingleSprite::PreparePrimitives 0x00B087C0: ?ScaleParticleDimensions@...@QBEXAAVC2DVector@@@Z 0x00AF8F00 (the sprite size),
 // ?ScaleParticleDimensions@...@QBEXAAK@Z 0x00B05B80 (the trail length) and ?GetUnscaledWorldTransform@...@QBEXAAVCMatrix3x4@@@Z
 // 0x00B05840, out of line; ?GetNormalisedTime@...@QBEMXZ inline (the +0x50 bit 3 test, then the +0x5C read).
 void ScaleParticleDimensions(C2DVector&) const;
 void ScaleParticleDimensions(unsigned long&) const;
 void GetUnscaledWorldTransform(CMatrix3x4&) const;
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
protected:
 const CParticleEmitter* PParentEmitter;CIEngine* PEngine;
 unsigned char Components[16],Particles[4],Decals[4],UpdateParameters[36];
 long NoActiveParticles,NoActiveDecals;
 bool Initialised:1,Enabled:1,Dead:1,NormalisedTimeEnabled:1,UseAllAttractors:1,Visible:1,StayWithEmitter:1,ScaleParticles:1;
 float GlobalAlpha,FadeAlpha,NormalisedTime;
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_SCALE
 C3DVector Scale;CCharString Name;              // +0x60 (DatabaseSerialise 0x00AF4990 / 0x00AF49AD / 0x00AF49CA: ScaleX/Y/Z at +0x60/+0x64/+0x68)
#else
 unsigned char Scale[12];CCharString Name;
#endif
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticleSystem)==0x70);
#endif
