// CSoundEmitterLH::GetActiveSampleInfo(const CASoundBank& bank, long sample, std::vector<CActiveSoundInfo>& infos) const -- when the sound system is up: append a CActiveSoundInfo {sample, bank, start time} for every active sound of this emitter playing that sample of that bank
// Leaf: ?GetActiveSampleInfo@CSoundEmitterLH@@UBEXABVCASoundBank@@JAAV?$vector@VCActiveSoundInfo@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAC5E0, 136 bytes, thiscall `ret 0Ch`; CSoundEmitterLH vtable 0x0129E6EC slot 24.
// Name: engine/CASoundEmitter.h's slot-24 name (descriptive: no PDB covers the audio module, no Anniversary counterpart); the catalog
//   label `CSoundEmitterLH::GetActiveSoundInfo` is respelled to the shared header's name (NAME_FIXES.tsv). Argument order: the shared
//   opt-in FABLE_RETAIL_CASOUNDEMITTER_ACTIVESAMPLEINFO_ORDER (the caller 0x006E2ED0 pushes the vector first; the body reads the bank first).
// Model: the landed CSoundEmitterLH rows' class text (IsSamplePlaying 0x00AABC50 is this row's loop with `return true`; descriptive
//   members, one spelling in every row) on the shared engine/CSoundManagerLH.h (SoundSystem +0x40). The record comes from the shared engine/CActiveSoundInfo.h with the new opt-in
//   FABLE_RETAIL_CACTIVESOUNDINFO_LH_CTOR (its three-argument constructor, out-of-line copy 0x00AAB810; the third word is the float
//   StartTime +0x10 of the active sound).
// Callees: CBaseClass::CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0, vector<CActiveSoundInfo>::push_back 0x00AACEF0 (out of line:
//   FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE).
// Inline-budget: FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE (declaration only) -- retail calls push_back 0x00AACEF0 (0x00AAC645); with the
//   STLport body visible VC7.1 expands it here: DIFFER(267v136) (measured 2026-11-19, ATTEMPTS.log).
#define FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#define FABLE_RETAIL_CASOUNDEMITTER_ACTIVESAMPLEINFO_ORDER
#define FABLE_RETAIL_CACTIVESOUNDINFO_LH_CTOR
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "engine/C3DVector.h"
#include "engine/CASoundManager.h"
#include "engine/CSoundManagerLH.h"
#include "engine/CASoundEmitter.h"
#include "engine/CASoundBank.h"
#include "engine/CActiveSoundInfo.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"

class CSoundBankLH;

class CSoundEmitterLH : public CASoundEmitter {           // vtable 0x0129E6EC
public:
    class CActiveSound : public CBaseClassNonCopyable {   // vtable 0x0129E67C, 0x24 bytes (descriptive members)
    public:
        long          State;                              // +0x04
        long          SampleID;                           // +0x08 the libpk event handle (-1 once stopped)
        CASoundBank*  PBank;                              // +0x0C the bank PlaySample / PlaySoundEvent played from
        float         StartTime;                          // +0x10
        bool          Started;                            // +0x14
        bool          Stopping;                           // +0x15
        long          Sample;                             // +0x18 the sample index in that bank
        CCharString   Name;                               // +0x1C
        CSoundBankLH* PAtmos;                             // +0x20
    };
    CSoundManagerLH*                           PManager;      // +0x04
    C3DVector                                  Position;      // +0x08
    std::list<CCountedPointer<CActiveSound> >  ActiveSounds;  // +0x14
    bool                                       Positional;    // +0x18
    bool                                       Playing;       // +0x19
    bool                                       Active;        // +0x1A
    long                                       EmitterID;     // +0x1C
    virtual void GetActiveSampleInfo(const CASoundBank& bank, long sample,
                                     std::vector<CActiveSoundInfo, std::allocator<CActiveSoundInfo> >& infos) const;
};

void CSoundEmitterLH::GetActiveSampleInfo(const CASoundBank& bank, long sample,
                                          std::vector<CActiveSoundInfo, std::allocator<CActiveSoundInfo> >& infos) const
{
    if (PManager->SoundSystem) {
        for (std::list<CCountedPointer<CActiveSound> >::const_iterator it = ActiveSounds.begin(); it != ActiveSounds.end(); ++it) {
            CActiveSound* sound = it->Data;
            if (sound->PBank == &bank && sound->Sample == sample)
                infos.push_back(CActiveSoundInfo(sound->PBank, sound->Sample, sound->StartTime));
        }
    }
}
