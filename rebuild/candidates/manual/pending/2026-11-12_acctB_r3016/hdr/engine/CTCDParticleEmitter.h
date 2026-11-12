#ifndef FABLE_ENGINE_CTCDPARTICLEEMITTER_H
#define FABLE_ENGINE_CTCDPARTICLEEMITTER_H
#ifdef FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
// Opt-in (Codex pass 20, row 0x008DAA80 NCreatureMode::CModeThunderLightningStorm::CreateSkyBeam): the complete Ego_r.pdb / FableWin.pdb class (members, accessors and layout as the
// pass 20 hand-back modelled it). Without the macro the earlier narrower model below is unchanged.
// Complete original owner from both original PDBs and native offsets.
#include "engine/CTCDriverBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
namespace NParticleEngine{class CParticleEmitter;template<class A,class B>class CParticleUpdateParameters;}
class CRightHandedSet;class C3DVector;
class CTCDParticleEmitter:public CTCDriverBase{
private:class CParticleCreationParameters;
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_PARTICLE_EMITTER;}
 virtual ETCInterfaceType GetInterfaceType() const;
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName() const;
 static unsigned long FABLE_FASTCALL StatGetDriverType();
 virtual bool IsDriverType(long p0) const;
 CTCDParticleEmitter(const CTCDParticleEmitter & p0);
 CTCDParticleEmitter(CThing & p0);
 virtual void FrameUpdate();
 virtual bool HasFrameUpdate() const;
 virtual void OnCreate();
 virtual void OnKill();
 virtual void OnDie();
 virtual void CreateAdditionalTCs();
 virtual void OnSerialise(CPersistContext & p0, bool p1);
 virtual bool IsComponentSerialisable() const;
 virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo & p0);
 virtual void OnSetInLimbo(bool p0);
 static CThing * FABLE_FASTCALL CreateIndependant(long p0, const C3DVector & p1, bool p2);
 static CThing * FABLE_FASTCALL Create(long p0, long p1, float p2, const C3DVector & p3, bool p4);
 static CThing * FABLE_FASTCALL Create(const CCharString & p0, const C3DVector & p1, bool p2);
 static CThing * FABLE_FASTCALL Create(long p0, const C3DVector & p1, bool p2);
 void AttachToThing(CThing & p0, long p1, const CCharString & p2, long p3, float p4);
 void UpdateEmitterThingAttachment(bool p0, float p1);
 void AttachToCamera(long p0, float p1);
 void Detach();
 void SetAsDrawable(bool p0);
 void SetToSleep();
 void SetAwake();
 void SetDieOffTimer(long p0);
 void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long> & p0);
 void SetEmitterOrientation(const CRightHandedSet & p0);
 void SetEmitterPosition(const C3DVector & p0);
 void SetAlwaysUpdate(bool p0);
 C3DVector GetEmitterPosition() const;
 void EnterTimelineEnd();
 bool IsAlwaysUpdating() const;
 NParticleEngine::CParticleEmitter * GetPEmitter() const;
 const CThing * PeekPThingAttachedTo() const;
protected:
 void ClearAttachments();
 bool IsParticleCreationDisabled() const;
 void SetParticleCreationDisabled(bool p0);
 void SetParticleID(long p0);
private:
 void SetPEmitter(NParticleEngine::CParticleEmitter * p0);
 void CreateParticleWaitingFor();
 void Construct(long p0, const C3DVector & p1, bool p2, bool p3);
 void UpdateEmitterCameraAttachment(bool p0);
 static void FABLE_FASTCALL SendCreationCriteriaSound(CThing & p0);
 static void FABLE_FASTCALL SendDestructionCriteriaSound(CThing & p0);
 CCharString EmitterName;
 long ParticleTypeID;
 CIntelligentPointer<NParticleEngine::CParticleEmitter> PEmitter;
 CIntelligentPointer<CThing> PAttachedToThing;
 unsigned long AttachmentFlags;
 CCharString AttachmentPosName;
 long AttachmentPosIndex;
 float FloatOffset;
 long DieOffTimer;
 long FramesLeftToWaitForParticleToCreate;
 CCountedPointer<CTCDParticleEmitter::CParticleCreationParameters> PCreationParameters;
private:
 bool WaitingForParticleToLoad:1;
 bool ForceParticleToLoad:1;
 bool IndependantObject:1;
 bool ParticleCreationDisabled:1;
 bool ResetEmitterPosition:1;
 bool ResetEmitterOrientation:1;
 bool AttachedToCamera:1;
 bool Moving:1;
 bool AttachedToThing:1;
 bool AlwaysUpdate:1;
 bool Asleep:1;
public:
 virtual ~CTCDParticleEmitter();
 CTCDParticleEmitter & operator=(const CTCDParticleEmitter & p0);
};
FABLE_STATIC_ASSERT(sizeof(CTCDParticleEmitter)==72);
#else
// New shared header (chunk 1024, row 0x007CD390 CTCMultiStrike::CTCMultiStrike): the particle-emitter component, interface
// TCI_PARTICLE_EMITTER (75; the row's inline GetTC looks up 0x4B). Declarations only, Ego_r.pdb / FableWin.pdb spellings:
// ?SetAlwaysUpdate@CTCDParticleEmitter@@QAEX_N@Z (retail 0x006E0590) and
// ?AttachToThing@CTCDParticleEmitter@@QAEXAAVCThing@@JABVCCharString@@JM@Z (retail 0x006E0BE0). Layout not modelled. Needs FABLE_RETAIL_CTCBASE.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CThing;
class CCharString;
class C3DVector;
class CTCDParticleEmitter : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PARTICLE_EMITTER; }
    void SetAlwaysUpdate(bool always_update);                                                          // 0x006E0590
    void AttachToThing(CThing& thing, long attach_type, const CCharString& helper_name, long helper_index, float offset);   // 0x006E0BE0
#ifdef FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
    // Nested opt-in (w22 agent2022, row 0x007F26B0 CTCLightningOrb::Initialise): Ego_r.pdb / FableWin.pdb
    // ?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z (public static __fastcall, retail 0x006E0880, the bound spelling: the orb passes
    // the effect index in ecx, its position in edx and pushes false, 0x007F2D12..0x007F2D1F). Declaration only.
    static CThing* __fastcall Create(long effect, const C3DVector& position, bool independent);   // 0x006E0880 (parameter names descriptive)
#endif
};
#endif
#endif
