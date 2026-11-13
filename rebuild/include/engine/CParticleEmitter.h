#ifdef FABLE_RETAIL_CPARTICLEEMITTER
#ifndef FABLE_ENGINE_CPARTICLEEMITTER_H
#define FABLE_ENGINE_CPARTICLEEMITTER_H
#include "engine/CBaseObject.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
class CPersistContext;class CSymbolMap;class CDataInputStream;
#ifdef FABLE_RETAIL_CPARTICLEEMITTER_CREATE2D
class C2DVector;class C3DOrientationInterpolator;
#ifdef _MSC_VER
enum EEngine2DLayer;
#else
enum EEngine2DLayer { };
#endif
#endif
namespace NParticleEngine {
class CParticleSystem;
#ifdef FABLE_RETAIL_CPARTICLEEMITTER_GETPRIORITY
#ifdef _MSC_VER
enum EEmitterPriority;
#else
enum EEmitterPriority { };
#endif
#endif
class CEmitterDatabaseSerialiseInfo {public:const CSymbolMap& GraphicSymbols;const CSymbolMap& MeshSymbols;};
float __fastcall GetMaxSpawnDistance();float __fastcall GetMaxDrawDistance();
float __fastcall GetFadeOutStartDistance();float __fastcall GetFadeInStartDistance();float __fastcall GetFadeInEndDistance();
// Layout: Ego_r complete original fields, native vector12/update36; size0x16c.
class CParticleEmitter:public CBaseObject {
public:CParticleEmitter();virtual ~CParticleEmitter();
#ifdef FABLE_RETAIL_CPARTICLESYSTEM_PEEKEMITTERTRANSFORM
 // Opt-in (agentNF22, row 0x00AF3D80): CParticleSystem::PeekEmitterTransform 0x00AF3D80 / PeekEmitterVelocity 0x00AF3DB0 read the
 // protected Transform (+0xb0) / Velocity (+0x52) straight off the parent emitter, and neither PDB names an emitter accessor for them.
 friend class CParticleSystem;
#endif
#ifdef FABLE_RETAIL_CPARTICLEEMITTER_KILL
 // Opt-in (chunk 898, row 0x0079CF30 CTCBulletTime::Pause): Ego_r.pdb ?IsDead@CParticleEmitter@NParticleEngine@@QBE_NXZ (inline: the
 // Dead bit, bit 5 of the +0x40 word: `mov cl,[eax+40h] ; shr cl,5 ; test cl,1` at 0x0079CF6F) and ?Kill@CParticleEmitter@NParticleEngine@@QAEXXZ (0x00AEE8C0).
 bool IsDead() const { return Dead; }
 void Kill();                                                          // 0x00AEE8C0
#endif
#ifdef FABLE_RETAIL_CPARTICLEEMITTER_UPDATE
 bool IsEmitter2D()const{return Emitter2D;}
 void Update();
#endif
 void DatabaseSerialise(CPersistContext&,const CEmitterDatabaseSerialiseInfo&);
#ifdef FABLE_RETAIL_CPARTICLEEMITTER_CREATE2D
 // Opt-in (chunk Q28, row 0x00AEB790 CParticleEmitterManager::Create2DEmitter): Ego_r.pdb / FableWin.pdb
 // ?Is2DEmitter@CParticleEmitter@NParticleEngine@@QBE_NXZ (inline in retail: the Emitter2D bit, `mov dx,[eax+40h] ; shr edx,0Bh ;
 // test dl,1` at 0x00AEB7B4), ?DatabaseClone@..@@QBEPAV12@XZ (0x00AF0650), ?Set2DPosition@..@@QAEXABVC2DVector@@W4EEngine2DLayer@@@Z
 // (0x00AED090) and ?SetOrientation@..@@QAEXABVC3DOrientationInterpolator@@@Z (0x00AEDE60). No layout change.
 bool Is2DEmitter() const { return Emitter2D; }
 CParticleEmitter* DatabaseClone() const;                              // 0x00AF0650
 void Set2DPosition(const C2DVector& position, EEngine2DLayer layer);  // 0x00AED090
 void SetOrientation(const C3DOrientationInterpolator& orientation);   // 0x00AEDE60
#endif
#ifdef FABLE_RETAIL_CPARTICLEEMITTER_GETPRIORITY
 // Opt-in (chunk Q28, the CParticleEmitterList::Insert 0x00AEB060 body in the 0x00AEB790 TU): FableWin.pdb
 // ?GetPriority@CParticleEmitter@NParticleEngine@@QBE?AW4EEmitterPriority@2@XZ (inline in retail: the Priority:3 bits of the +0x40
 // word, `mov cl,[ecx+40h] ; and cl,7` at 0x00AEB113). The enumerators are not on disk. No layout change.
 EEmitterPriority GetPriority() const { return (EEmitterPriority)Priority; }
#endif
#ifdef FABLE_RETAIL_CPARTICLEEMITTER_INTERPOLATE
 // Retail opt-in (chunk Q3, row 0x007CCF50 CTCMultiStrike::ApplyEffectOnWeapon): Ego_r.pdb / FableWin.pdb
 // ?GetApproximateTotalSplineLength@CParticleEmitter@NParticleEngine@@QAEMXZ (out of line, retail 0x00AED050: sums the spline length of
 // every particle system with flag 2), ?InterpolateTo@CParticleEmitter@NParticleEngine@@QAEXABV12@M@Z (0x00AED600), and the inline
 // ?Sleep@..@@QAEXXZ (WaitingToSleep = true, `or byte ptr [edi+41h],2` at 0x007CD24D; the landed 0x006E0690 model spells the same body)
 // and ?SetAlwaysUpdate@..@@QAEX_N@Z (`or al,10h` on +0x44 at 0x007CD2C6). No layout change.
 float GetApproximateTotalSplineLength();                              // 0x00AED050
 void InterpolateTo(const CParticleEmitter& other, float t);           // 0x00AED600
 void Sleep() { WaitingToSleep = true; }
 void SetAlwaysUpdate(bool always) { AlwaysUpdate = always; }
#endif
 // Chunk 742 (0x00AEFAA0): FableWin.pdb / Ego_r.pdb ?ReadBinary@CParticleEmitter@NParticleEngine@@QAEXAAVCDataInputStream@@@Z.
 void ReadBinary(CDataInputStream&);
#ifdef FABLE_RETAIL_CPARTICLEEMITTER_INTERPOLATETO
 // Opt-in (chunk 2001, row 0x00AED600): Ego_r.pdb / FableWin.pdb ?InterpolateTo@CParticleEmitter@NParticleEngine@@QAEXABV12@M@Z
 // (public non-virtual; called by CParticleEmitterManager::CreateInterpolatedEmitter 0x00AEB674). Declaration only.
 void InterpolateTo(const CParticleEmitter& other, float t);
#endif
protected:
 std::vector<CCountedPointer<CParticleSystem> > ParticleSystems;
 unsigned char UpdateParameters[36]; // Original NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long>, unaccessed composite.
 unsigned char PrimitiveGroupHandle[8]; // Original CEnginePrimitiveHandle, unaccessed composite.
 unsigned short Priority:3;
 unsigned short IsScreenDisplacement:1;
 unsigned short ReadZBuffer:1;
 unsigned short Dead:1;
 unsigned short ParticleCreationDisabled:1;
 unsigned short ContinuousEmitter:1;
 unsigned short Initialised:1;
 unsigned short WaitingToSleep:1;
 unsigned short Sleeping:1;
 unsigned short Emitter2D:1;
 unsigned short Emitter2DLayer:7;
 unsigned char PreWaterEffect:1;
 unsigned char WaterEffect:1;
 unsigned char ZBufferWriteable:1;
 unsigned char Visible:1;
 unsigned char AlwaysUpdate:1;
 unsigned char DieIfOffsceen:1;
 unsigned char OffscreenUpdate:1;
 unsigned char ClipEffectToWeatherMask:1;
 unsigned char EnableDithering:1;
 unsigned char CalcBoundingSphereOnceOnly:1;
 unsigned char BoundingSphereValid:1;
 unsigned char ClearedParticles:1;
 unsigned char ClearedDecals:1;
 unsigned char OldPos[12]; // Original C3DVector, unaccessed composite.
 unsigned char Velocity[14]; // Original C3DVector, unaccessed composite.
 unsigned char OrientationInterpolator[52]; // Original C3DOrientationInterpolator, unaccessed composite.
 unsigned char PositionInterpolator[28]; // Original C3DPositionInterpolator, unaccessed composite.
 unsigned char Transform[48]; // Original CMatrix3x4, unaccessed composite.
 unsigned char InverseTransform[48]; // Original CMatrix3x4, unaccessed composite.
 unsigned char EndTransform[48]; // Original CMatrix3x4, unaccessed composite.
 CCharString Name;
 unsigned char BSphere[16]; // Original C3DBoundingSphere, unaccessed composite.
 float DistanceToCam;
 float MaxSpawnDistance;
 float MaxDrawDistance;
 float FadeOutStart;
 float FadeInStart;
 float FadeInEnd;
};
}
FABLE_STATIC_ASSERT(sizeof(NParticleEngine::CParticleEmitter)==0x16c);
#endif
#else
#ifndef FABLE_ENGINE_CPARTICLEEMITTER_H
#define FABLE_ENGINE_CPARTICLEEMITTER_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CParticleEmitter = 0x178.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CParticleEmitter = 0x16c.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CCharStringData;

#pragma pack(push, 1)
struct CParticleEmitter {
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _base_0x04[0x4];                        // +0x04 flattened base-class subobject(s)
    unsigned char    ParticleSystems[0xc];                   // +0x08 vector<CCountedPointer<NParticleEngine::CParticleSystem>,std::allocator<CCountedPointer<NParticleEngine::CParticleSystem>_>_>
    unsigned char    UpdateParameters[0x24];                 // +0x14 CParticleUpdateParameters<unsigned_long,unsigned_long>
    unsigned char    PrimitiveGroupHandle[0x8];              // +0x38 CEnginePrimitiveHandle
    unsigned char    ContinuousEmitter[0x1];                 // +0x40 bitfield group: ushort:1
    unsigned char    Emitter2D[0x1];                         // +0x41 bitfield group: ushort:1
    unsigned char    Emitter2DLayer[0x2];                    // +0x42 bitfield group: ushort:7
    unsigned char    ClipEffectToWeatherMask[0x1];           // +0x44 bitfield group: uchar:1
    unsigned char    ClearedDecals[0x1];                     // +0x45 bitfield group: uchar:1
    float            OldPos_X;                               // +0x46 C3DVector::X
    float            OldPos_Y;                               // +0x4a C3DVector::Y
    float            OldPos_Z;                               // +0x4e C3DVector::Z
    unsigned char    Velocity[0xe];                          // +0x52 C3DVector
    unsigned char    OrientationInterpolator[0x34];          // +0x60 C3DOrientationInterpolator
    unsigned char    PositionInterpolator[0x1c];             // +0x94 C3DPositionInterpolator
    unsigned char    Transform[0x30];                        // +0xb0 CMatrix3x4
    unsigned char    InverseTransform[0x30];                 // +0xe0 CMatrix3x4
    unsigned char    EndTransform[0x30];                     // +0x110 CMatrix3x4
    CCharStringData* Name_PStringData;                       // +0x140 CCharString::PStringData
    unsigned char    BSphere[0x10];                          // +0x144 C3DBoundingSphere
    float            DistanceToCam;                          // +0x154
    float            MaxSpawnDistance;                       // +0x158
    float            MaxDrawDistance;                        // +0x15c
    float            FadeOutStart;                           // +0x160
    float            FadeInStart;                            // +0x164
    float            FadeInEnd;                              // +0x168
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CParticleEmitter) == 0x16c);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, ParticleSystems) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, UpdateParameters) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, PrimitiveGroupHandle) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, ContinuousEmitter) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, Emitter2D) == 0x41);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, Emitter2DLayer) == 0x42);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, ClipEffectToWeatherMask) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, ClearedDecals) == 0x45);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, OldPos_X) == 0x46);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, OldPos_Y) == 0x4a);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, OldPos_Z) == 0x4e);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, Velocity) == 0x52);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, OrientationInterpolator) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, PositionInterpolator) == 0x94);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, Transform) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, InverseTransform) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, EndTransform) == 0x110);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, Name_PStringData) == 0x140);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, BSphere) == 0x144);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, DistanceToCam) == 0x154);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, MaxSpawnDistance) == 0x158);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, MaxDrawDistance) == 0x15c);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, FadeOutStart) == 0x160);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, FadeInStart) == 0x164);
FABLE_STATIC_ASSERT(offsetof(CParticleEmitter, FadeInEnd) == 0x168);

#endif // FABLE_ENGINE_CPARTICLEEMITTER_H

#endif
