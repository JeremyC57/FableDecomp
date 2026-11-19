// CSoundEmitterLH::PlaySample(CASoundBank& bank, long sample, bool loop, const CSoundInit& init, bool attached) -- play a bank sample
//   from this emitter: bail out with -1 when the manager has no libpk sound system, sound is disabled or the sample index is out of
//   range; build LHA::CEventPlayOptions (loop count from init, pitch, 3D unless looping, head-relative when attached, the gain / pitch
//   send-fader bits, the position from the manager's head vector or the emitter, the emitter id or init's override id); if the
//   engine bank already holds the sample, replay it (interruptable) through the event manager's bank-sample entry, otherwise start it
//   through a sample query (group 0); on success track it as a new counted CActiveSound; Playing is set either way; returns the handle.
// Leaf: ?PlaySample@CSoundEmitterLH@@UAEKAAVCASoundBank@@J_NABVCSoundInit@@1@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AABF60, 620 bytes, thiscall `ret 14h`; vtable 0x0129E6EC (CSoundEmitterLH) slot 3, the
//   engine/CASoundEmitter.h slot name (Anniversary 360 PDB + the Ego_r/FableWin CThingSoundEmitter::PlaySample forwarder).
// Callees: LHA::CEventPlayOptions ctor 0x00C0A790, SetLoopCount 0x00C0A860 / SetInterruptable 0x00C0A880 (descriptive, new opt-in),
//   SetFlag16 0x00C0A8A0 / SetFlag 0x00C0A8C0, FindFirstSetBit 0x00A3F860 (inlined in the gain loop, called once for the pitch loop,
//   as the landed sibling PlaySoundEvent 0x00AABD50), CSoundSystem::GetEventManager 0x00C0A590 (slot 6), the engine bank's slot 4
//   (+0x10) and the event manager's slots 14 (+0x38) and 16 (+0x40), CSampleQueryLH(bank, group, sample) 0x00C08750 / dtor 0x00C08780,
//   operator new 0x00BFEA1A, GFGetTime 0x009E1BC0, CActiveSound(handle, bank, sample, time, is3D, headRelative) 0x00AAB850,
//   CCountedPointer<CActiveSound>(T*) 0x00AAC820 / dtor 0x00AAC870, list::push_back 0x00AACCC0.
// Model: the landed sibling PlaySoundEvent 0x00AABD50 (same includes and opt-ins, its descriptive audio classes; the engine-bank
//   lookup and the bank-sample play entry are added here, descriptive).
// Shared headers (audit): CSoundManagerLH, CSoundSystem and CSoundEventManager come from engine/CSoundManagerLH.h / libpk/CSoundSystem.h
//   (shared since agentC40; the landed 0x00AAC5E0 uses them) with two nested opt-ins: CSOUNDMANAGERLH_HEADPOSITION (+0x74) and
//   LIBPK_CSOUNDEVENTMANAGER_PLAYSLOTS (slots 14 / 16); GFGetTime from engine/GFGetTime.h. The engine bank, the sample query and the
//   emitter stay row-local (no header).
#define FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#define FABLE_RETAIL_CASOUNDMANAGER_ENABLEFLAGS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_LHA_CEVENTPLAYOPTIONS_SENDFADERS
#define FABLE_RETAIL_LHA_CEVENTPLAYOPTIONS_PARAMSETTERS
#define FABLE_RETAIL_LIBPK_CSOUNDEVENTMANAGER_PLAYSLOTS
#define FABLE_RETAIL_CSOUNDMANAGERLH_HEADPOSITION
#include "engine/C3DVector.h"
#include "engine/CASoundManager.h"
#include "engine/CSoundManagerLH.h"
#include "engine/CASoundEmitter.h"
#include "engine/CASoundBank.h"
#include "engine/CSoundInit.h"
#include "engine/LHA_CEventPlayOptions.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/GFGetTime.h"

inline int __fastcall FindFirstSetBit(unsigned long mask, unsigned long numBytes)   // 0x00A3F860
{
    for (unsigned long i = 0; i < numBytes * 8; ++i)
        if (mask & (1 << i))
            return (int)i;
    return -1;
}

class CSampleQueryLH {                        // vtable 0x012B1A54, 0x14 bytes
public:
    virtual void v00();
    long        m_group;
    long        m_sample;
    const void* m_pName;
    void*       m_pBank;
    CSampleQueryLH(void* bank, long group, long sample);   // 0x00C08750
    ~CSampleQueryLH();                                     // 0x00C08780
};

class CEngineBankLH {                         // the libpk bank behind CSoundBankLH (descriptive)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual long FindSample(long sample, long* group, long* index);                                  // slot 4 (+0x10) (descriptive)
};

class CSoundBankLH : public CASoundBank {
public:
    void*          m_pSystem;                             // +0x0C
    CEngineBankLH* m_pBank;                               // +0x10
    CEngineBankLH* GetEngineBank() const { return m_pBank; }   // (descriptive)
};

class CSoundEmitterLH : public CASoundEmitter {
public:
    class CActiveSound : public CBaseClassNonCopyable {
    public:
        unsigned char _body[0x20];
        CActiveSound(long sampleID, CASoundBank& bank, long sample, float startTime, bool is3D, bool headRelative);   // 0x00AAB850
    };
    CSoundManagerLH*                           PManager;      // +0x04
    C3DVector                                  Position;      // +0x08
    std::list<CCountedPointer<CActiveSound> >  ActiveSounds;  // +0x14
    bool                                       Positional;    // +0x18
    bool                                       Playing;       // +0x19
    bool                                       Active;        // +0x1A
    long                                       EmitterID;     // +0x1C
    virtual unsigned long PlaySample(CASoundBank& bank, long sample, bool loop, const CSoundInit& init, bool attached);
};

unsigned long CSoundEmitterLH::PlaySample(CASoundBank& bank, long sample, bool loop, const CSoundInit& init, bool attached)
{
    if (!PManager->SoundSystem || !PManager->SoundEnabled || (unsigned long)sample > 100000)
        return -1;
    LHA::CEventPlayOptions options;
    options.SetLoopCount(init.NoPlays == -1 ? -1 : init.NoPlays - 1);
    options.Pitch = init.Pitch;
    if (loop)
        options.Is3D = false;
    else
        options.Is3D = Positional;
    options.HeadRelative = attached;
    unsigned long bits = init.GainFaderBits;
    while (bits) {
        int bit = FindFirstSetBit(bits, 16);
        options.SetFlag16(bit, true);
        bits ^= 1 << bit;
    }
    unsigned long pitchBits = init.PitchFaderBits;
    while (pitchBits) {
        int bit = FindFirstSetBit(bits, 16);   // as retail and the sibling: the exhausted GAIN mask (VC7.1 hoists the invariant call)
        options.SetFlag(bit, true);
        pitchBits ^= 1 << bit;
    }
    if (options.HeadRelative) {
        const C3DVector& head = PManager->HeadPosition;
        options.Pos.X = head.X;
        options.Pos.Y = head.Y;
        options.Pos.Z = head.Z;
    } else if (options.Is3D) {
        options.Pos.X = Position.X;
        options.Pos.Y = Position.Y;
        options.Pos.Z = Position.Z;
    }
    // Inferred: FITTED -- the override-first spelling (the sibling 0x00AABD50 tests `== -1`); retail here takes the override on the
    //   fall-through (`cmp eax,-1 ; je` 0x00AAC0AF); `== -1` ternary / `if (== -1)` DIFFER(620v620), this and `if (!= -1)` match.
    options.EmitterId = (init.OverrideID != -1) ? init.OverrideID : EmitterID;
    long group, index;
    long handle = static_cast<CSoundBankLH&>(bank).GetEngineBank()->FindSample(sample, &group, &index);
    if (handle <= -1) {
        handle = PManager->SoundSystem->GetEventManager()->PlayEvent(CSampleQueryLH(static_cast<CSoundBankLH&>(bank).GetEngineBank(), 0, sample), options);
    } else {
        options.SetInterruptable(true);
        handle = PManager->SoundSystem->GetEventManager()->PlayBankSample(static_cast<CSoundBankLH&>(bank).GetEngineBank(), group, index, options);
    }
    if (handle >= 0)
        ActiveSounds.push_back(CCountedPointer<CActiveSound>(new CActiveSound(handle, bank, sample, (float)GFGetTime(), options.Is3D, options.HeadRelative)));
    Playing = true;
    return handle;
}