#ifndef FABLE_ENGINE_CTHINGSOUNDEMITTER_H
#define FABLE_ENGINE_CTHINGSOUNDEMITTER_H
#include "engine/CThing.h"
#ifdef FABLE_RETAIL_CTHINGSOUNDEMITTER_SOUNDEMITTER
#include "engine/CASoundEmitter.h"
class CASoundBank;
class CSoundInit;
class CCharString;
#endif
#ifdef FABLE_RETAIL_CTHINGSOUNDEMITTER_MEMBERS
// Nested retail opt-in (agentSND2, 2026-11-10; needs FABLE_RETAIL_CTHINGSOUNDEMITTER_SOUNDEMITTER): the tail members typed as
// Ego_r.pdb types them (PAttachedThing CIntelligentPointer<const CThing>, Pos C3DVector); the class is byte-packed from +0x98
// (PAttachedThing +0x9D and Pos +0xA6 are unaligned: landed ctor 0x008367B0 / dtor 0x00836A50 / ConstructFromParams 0x00836910).
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
#pragma pack(push, 1)
#endif
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// AttachedToThing, PAttachedThing, ContinuePlayingAfterAttachedThingDeath, Pos; complete tail through+b8, native allocation527340.
class CThingSoundEmitter:public CThing {
protected:
#ifdef FABLE_RETAIL_CTHINGSOUNDEMITTER_SOUNDEMITTER
    // Retail opt-in (agentSND2, 2026-11-10): +0x98 is the thing's sound emitter -- OnCreate 0x00836740 stores
    // CASoundManager::Create3DEmitter(Pos) (PSoundManager slot 4) here, ~CThingSoundEmitter 0x00836A50 hands it back through
    // DestroyPEmitter (slot 5), and the Ego_r.pdb forwarders PlaySample / PlaySoundEvent / IsSampleLoadedAndPlaying /
    // IsSamplePlaying / StopSpecificSample / ReleaseLoop / SetSpecificSamplePitch / SetSpecificSampleVolume call its
    // CASoundEmitter slots 3 / 4 / 21 / 22 / 9 / 13 / 6 / 7. The member is absent from the donor PDBs: descriptive name.
    CASoundEmitter* PSoundEmitter; // +98
#else
    unsigned char _pad_0x98[4]; // Native-only pointer, ctor8367c9; absent from all three donor PDBs, identity unresolved.
#endif
    bool AttachedToThing; // +9c, ctor8367d3 and writes836981/8369af
#ifdef FABLE_RETAIL_CTHINGSOUNDEMITTER_MEMBERS
    CIntelligentPointer<const CThing> PAttachedThing; // +9d
    bool ContinuePlayingAfterAttachedThingDeath; // +a5, ctor8367e5
    C3DVector Pos; // +a6
#else
    unsigned char PAttachedThing[8]; // +9d CIntelligentPointer<const CThing>; ctor8367bb,8367df
    bool ContinuePlayingAfterAttachedThingDeath; // +a5, ctor8367e5
    unsigned char Pos[12]; // +a6 C3DVector; native8369c2 takes this exact unaligned address
#endif
    unsigned char _pad_0xb2[6];
public: static CClass* __fastcall Alloc();
#ifdef FABLE_RETAIL_CTHINGSOUNDEMITTER_SOUNDEMITTER
    // Ego_r.pdb members (agentSND2 opt-in): the sound forwarders.
    unsigned long PlaySample(CASoundBank& bank, long sample, bool loop, const CSoundInit& init, bool attached);   // 0x00836810
    unsigned long PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, CSoundInit& init, bool attached);   // 0x00836840
    bool IsSamplePlaying(unsigned long sampleID) const;                                    // 0x00836870
    bool IsSampleLoadedAndPlaying(unsigned long sampleID) const;                           // 0x008368A0
    void StopSpecificSample(unsigned long sampleID);                                       // 0x008368D0
    void ReleaseLoop(unsigned long sampleID);                                              // 0x008368F0
    void SetSpecificSamplePitch(unsigned long sampleID, float pitch);
    void SetSpecificSampleVolume(unsigned long sampleID, float volume);
protected:
    virtual bool OnCreate();                                                               // 0x00836740 ?OnCreate@CThingSoundEmitter@@MAE_NXZ
public:
    virtual ~CThingSoundEmitter();                                                         // 0x00836A50
#endif
};
#ifdef FABLE_RETAIL_CTHINGSOUNDEMITTER_MEMBERS
#pragma pack(pop)
#endif
FABLE_STATIC_ASSERT(sizeof(CThingSoundEmitter)==0xb8);
#endif
