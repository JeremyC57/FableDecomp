#ifndef FABLE_ENGINE_CASOUNDEMITTER_H
#define FABLE_ENGINE_CASOUNDEMITTER_H
// Retail CASoundEmitter: the abstract sound emitter of the sound module (RTTI .?AVCASoundEmitter@@ : CBaseClass, vtable
// 0x0129E684, 25 slots, slot 0 the scalar deleting destructor 0x00AAB9A0, slots 1-24 _purecall 0x00BFEA08). The live
// objects are CSoundEmitterLH (RTTI CSoundEmitterLH : CASoundEmitter : CBaseClass, vtable 0x0129E6EC): the sound manager's
// own 2-D emitter at CSoundManagerLH+0x44 (built by 0x00AAC530 in the manager ctor 0x00A3EF00, returned by
// CASoundManager::Get2DEmitter, slot 9) and the 3-D emitters CASoundManager::Create3DEmitter (slot 4) news for every
// CThingSoundEmitter (the game wrapper's +0x98 member).
//
// SLOT NAMES (agentSND, 2026-11-10; table in the w22/agentSND SLOTS.tsv / NOTES.md):
//  * real, two sources: the Fable Anniversary Xbox 360 PDB (anniv360.pdb publics, lib_sound_emitter_un.cpp:
//    CSoundEmitterUN : CASoundEmitter, same interface) names every slot below except 18 and 24, with the parameter types
//    given; the TLC wrapper CThingSoundEmitter (Ego_r.pdb / FableWin.pdb names) forwards PlaySample -> slot 3,
//    PlaySoundEvent -> 4, SetSpecificSamplePitch -> 6, SetSpecificSampleVolume -> 7, StopSpecificSample -> 9,
//    ReleaseLoop -> 13, IsSampleLoadedAndPlaying -> 21, IsSamplePlaying -> 22 (landed rows 0x00836810..0x008368F0), which
//    pins the alignment of the Anniversary vtable (0x02E5889C in FableAnniversary.exe, 26 slots: one extra
//    PlayAtmos(CASoundBank&, const CCharString&) after slot 5) to the TLC one; every other slot matches by body.
//  * descriptive (no name anywhere): slot 18 Is3D (returns the +0x18 flag the positional ctor 0x00AAC580 sets; the
//    Anniversary getter is folded with another body and has no public), slot 24 GetActiveSampleInfo (collects the
//    CActiveSoundInfo records of the active sounds that match a sample; no Anniversary counterpart).
// TLC parameter types where they differ from the Anniversary build: PlaySample's sample index is `long` as the TLC
// wrapper CThingSoundEmitter::PlaySample (Ego_r.pdb ?PlaySample@CThingSoundEmitter@@QAEKAAVCASoundBank@@J_NABVCSoundInit@@1@Z).
#include "engine/CBaseClass.h"

class CASoundBank;
class CCharString;
class CSoundInit;
class C3DVector;
class CActiveSoundInfo;
namespace std { template <class _Tp> class allocator; template <class _Tp, class _Alloc> class vector; }

class CASoundEmitter : public CBaseClass {
public:
    virtual ~CASoundEmitter();                                                        // slot  0 ??_G 0x00AAB9A0
    virtual void Activate() = 0;                                                      // slot  1 (+0x04) LH 0x00AABA00
    virtual void Deactivate() = 0;                                                    // slot  2 (+0x08) LH 0x00AABA10
    virtual unsigned long PlaySample(CASoundBank& bank, long sample, bool loop,
                                     const CSoundInit& init, bool attached) = 0;     // slot  3 (+0x0C) LH 0x00AABF60
    virtual unsigned long PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop,
                                         const CSoundInit& init, bool attached) = 0; // slot  4 (+0x10) LH 0x00AABD50
    virtual unsigned long PlayAtmos(const CCharString& name) = 0;                     // slot  5 (+0x14) LH 0x00AAC1D0
    virtual void SetSpecificSamplePitch(unsigned long sampleID, float pitch) = 0;     // slot  6 (+0x18) LH 0x00AABA20
    virtual void SetSpecificSampleVolume(unsigned long sampleID, float volume) = 0;   // slot  7 (+0x1C) LH 0x00AABA40
    virtual void StopSample(CASoundBank& bank, unsigned long sample) = 0;            // slot  8 (+0x20) LH 0x00AAC340
    virtual void StopSpecificSample(unsigned long sampleID) = 0;                      // slot  9 (+0x24) LH 0x00AAC400
    virtual void StopAllSamples() = 0;                                                // slot 10 (+0x28) LH 0x00AAC2E0
    virtual void StopAllLoops() = 0;                                                  // slot 11 (+0x2C) LH 0x00AABA80
    virtual void StopAllCriteriaContainingString(const CCharString& text) = 0;        // slot 12 (+0x30) LH 0x00AAC460
    virtual void ReleaseLoop(unsigned long sampleID) = 0;                             // slot 13 (+0x34) LH 0x00AABB60
    virtual void Update() = 0;                                                        // slot 14 (+0x38) LH 0x00AABD40
    virtual void SetPos(const C3DVector& position) = 0;                               // slot 15 (+0x3C) LH 0x00AABBB0
    virtual void MovePos(const C3DVector& position) = 0;                              // slot 16 (+0x40) LH 0x00AAB9C0
    virtual const C3DVector GetPos() const = 0;                                       // slot 17 (+0x44) LH 0x00AABA60
    virtual bool Is3D() const = 0;                                                    // slot 18 (+0x48) LH 0x00AAB9D0 (descriptive)
    virtual bool IsPlaying() const = 0;                                               // slot 19 (+0x4C) LH 0x00AAB9E0
    virtual bool IsActive() const = 0;                                                // slot 20 (+0x50) LH 0x00AAB9F0
    virtual bool IsSpecificSampleLoadedAndPlaying(unsigned long sampleID) const = 0;  // slot 21 (+0x54) LH 0x00AABB00
    virtual bool IsSpecificSamplePlaying(unsigned long sampleID) const = 0;           // slot 22 (+0x58) LH 0x00AABAC0
    virtual bool IsSamplePlaying(const CASoundBank& bank, long sample) const = 0;     // slot 23 (+0x5C) LH 0x00AABC50
#ifdef FABLE_RETAIL_CASOUNDEMITTER_ACTIVESAMPLEINFO_ORDER
    // Opt-in (chunk 1024, row 0x006E2ED0): the argument order retail's caller pushes (`push &infos ; push sample ; push bank`,
    // 0x006E306A..0x006E3073) and the LH body 0x00AAC5E0 reads (bank [esp+28h] first, then the sample; the vector, third, gets the
    // CActiveSoundInfo records, RTTI vtable 0x0129E674). Same slot; the default declaration's order is unproven.
    virtual void GetActiveSampleInfo(const CASoundBank& bank, long sample,
                                     std::vector<CActiveSoundInfo, std::allocator<CActiveSoundInfo> >& infos) const = 0; // slot 24 (+0x60)
#else
    virtual void GetActiveSampleInfo(std::vector<CActiveSoundInfo, std::allocator<CActiveSoundInfo> >& infos,
                                     const CASoundBank& bank, long sample) const = 0; // slot 24 (+0x60) LH 0x00AAC5E0 (descriptive)
#endif
};
FABLE_STATIC_ASSERT(sizeof(CASoundEmitter) == 0x4);

#endif // FABLE_ENGINE_CASOUNDEMITTER_H
