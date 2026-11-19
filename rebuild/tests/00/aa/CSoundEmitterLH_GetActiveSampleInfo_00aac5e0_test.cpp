// Behaviour test for 0x00AAC5E0 CSoundEmitterLH::GetActiveSampleInfo: the row's defines, includes and body verbatim, on a zeroed emitter
// image (manager pointer, an in-place STLport list of counted active sounds). Only sounds of the asked bank AND sample are reported, each as
// {sample, bank, start time} in list order, through the vector's out-of-line push_back (recorded here); nothing at all without a sound
// system; an empty list reports nothing.
#include <stdio.h>
#include <string.h>
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

CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
static CActiveSoundInfo* g_out[8];
static long g_n = 0;
static CActiveSoundInfo g_store[8] = { CActiveSoundInfo(0, 0, 0.0f), CActiveSoundInfo(0, 0, 0.0f), CActiveSoundInfo(0, 0, 0.0f), CActiveSoundInfo(0, 0, 0.0f),
                                       CActiveSoundInfo(0, 0, 0.0f), CActiveSoundInfo(0, 0, 0.0f), CActiveSoundInfo(0, 0, 0.0f), CActiveSoundInfo(0, 0, 0.0f) };
template <> void std::vector<CActiveSoundInfo, std::allocator<CActiveSoundInfo> >::push_back(const CActiveSoundInfo& x)
{
    if (g_n < 8) { g_store[g_n].SampleIndex = x.SampleIndex; g_store[g_n].PBank = x.PBank; g_store[g_n].StartTime = x.StartTime; ++g_n; }
}
typedef CSoundEmitterLH::CActiveSound TSound;
static double g_emitter[sizeof(CSoundEmitterLH) / 8 + 1];
static double g_manager[sizeof(CSoundManagerLH) / 8 + 1];
static double g_sounds[5][sizeof(TSound) / 8 + 1];
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
int main()
{
    memset(g_emitter, 0, sizeof(g_emitter)); memset(g_manager, 0, sizeof(g_manager)); memset(g_sounds, 0, sizeof(g_sounds));
    CSoundEmitterLH* e = reinterpret_cast<CSoundEmitterLH*>(g_emitter);
    CSoundManagerLH* m = reinterpret_cast<CSoundManagerLH*>(g_manager);
    e->PManager = m;
    new (&e->ActiveSounds) std::list<CCountedPointer<TSound> >();
    CASoundBank* bankA = reinterpret_cast<CASoundBank*>(&g_manager[0]);
    CASoundBank* bankB = reinterpret_cast<CASoundBank*>(&g_manager[1]);
    std::vector<CActiveSoundInfo, std::allocator<CActiveSoundInfo> >& infos = *reinterpret_cast<std::vector<CActiveSoundInfo, std::allocator<CActiveSoundInfo> >*>(&g_manager[2]);
    // empty list, system up: nothing
    m->SoundSystem = reinterpret_cast<CSoundSystem*>(&g_manager[3]);
    e->CSoundEmitterLH::GetActiveSampleInfo(*bankA, 7, infos);
    check(g_n == 0, "empty list");
    TSound* s[5];
    CASoundBank* banks[5] = { bankA, bankB, bankA, bankA, bankA };
    long samples[5] = { 7, 7, 8, 7, -7 };
    for (int i = 0; i < 5; ++i) {
        s[i] = reinterpret_cast<TSound*>(g_sounds[i]);
        s[i]->PBank = banks[i]; s[i]->Sample = samples[i]; s[i]->StartTime = 1.5f + i; s[i]->SampleID = 100 + i;
        CCountedPointer<TSound> p; p.Data = s[i];
        e->ActiveSounds.push_back(p);
        p.Data = 0;
    }
    // no sound system: nothing
    m->SoundSystem = 0;
    e->CSoundEmitterLH::GetActiveSampleInfo(*bankA, 7, infos);
    check(g_n == 0, "no sound system");
    m->SoundSystem = reinterpret_cast<CSoundSystem*>(&g_manager[3]);
    e->CSoundEmitterLH::GetActiveSampleInfo(*bankA, 7, infos);
    check(g_n == 2, "two matches (bank and sample)");
    check(g_store[0].SampleIndex == 7 && g_store[0].PBank == bankA && g_store[0].StartTime == 1.5f, "first match: sample, bank, start time");
    check(g_store[1].SampleIndex == 7 && g_store[1].PBank == bankA && g_store[1].StartTime == 4.5f, "second match in list order");
    g_n = 0;
    e->CSoundEmitterLH::GetActiveSampleInfo(*bankB, 7, infos);
    check(g_n == 1 && g_store[0].PBank == bankB && g_store[0].StartTime == 2.5f, "other bank");
    g_n = 0;
    e->CSoundEmitterLH::GetActiveSampleInfo(*bankB, 8, infos);
    check(g_n == 0, "bank matches, sample does not");
    if (!g_fail) printf("00aac5e0_TEST PASS\n");
    return g_fail;
}
