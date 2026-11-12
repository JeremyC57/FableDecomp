// CSoundEmitterLH::PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, const CSoundInit& init, bool attached) --
//   play the named sound event of a bank from this emitter: bail out with -1 when the manager has no libpk sound system or sound
//   is disabled; build LHA::CEventPlayOptions (pitch from init, 3D unless looping, head-relative when attached, the gain / pitch
//   send-fader bits, the position from the manager's head vector or the emitter, the emitter id or init's override id), start it
//   through the system's event manager with a sample query of (engine bank handle, event name), and on success track it as a new
//   counted CActiveSound; Playing is set either way; returns the event handle.
// Leaf: ?PlaySoundEvent@CSoundEmitterLH@@UAEKAAVCASoundBank@@ABVCCharString@@_NABVCSoundInit@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AABD50, 515 bytes, thiscall `ret 14h`; vtable 0x0129E6EC (CSoundEmitterLH) slot 4, the
//   engine/CASoundEmitter.h slot name (Anniversary 360 PDB + Ego_r/FableWin ?PlaySoundEvent@CThingSoundEmitter forwarder 0x0082A550).
// Callees: LHA::CEventPlayOptions::CEventPlayOptions 0x00C0A790, its send-fader setters 0x00C0A8A0 / 0x00C0A8C0 (landed as
//   CBitMaskPair_00c0a8c0::SetFlag16 / SetFlag; NAME_FIXES.tsv respells them LHA::CEventPlayOptions::SetFlag16 / SetFlag, the
//   method names descriptive), FindFirstSetBit 0x00A3F860 (inlined in the gain loop, called once out of line for
//   the pitch loop -- retail's argument there is the exhausted gain mask; the loop is written like the gain loop and VC7.1 hoists
//   the invariant call by itself; FindFirstSetBit is a descriptive name, no PDB name found), CSoundSystem::GetEventManager
//   0x00C0A590 (slot 6, descriptive as in the landed PlayAtmos 0x00AAC1D0; landed body gapscan_sub00c0a590), the event manager's slot 16 (+0x40), CSampleQueryLH ctor/dtor 0x00C08720 / 0x00C08780, operator new
//   0x00BFEA1A, GFGetTime 0x009E1BC0, CActiveSound ctor 0x00AAB8B0, CCountedPointer<CActiveSound>(T*) 0x00AAC820 / dtor 0x00AAC870,
//   list::push_back 0x00AACCC0 (FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE).
// Model: as the landed PlayAtmos 0x00AAC1D0 (descriptive audio members, no PDB). Shape facts: the head-relative position copies
//   through a named const C3DVector& (retail `mov edx,[eax+74h] ; add eax,74h`); EmitterId is one ternary; the engine bank handle
//   comes through a small inline accessor (descriptive GetEngineBank; without it bank / the event-manager vtable swap edi / ebx).
#define FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#define FABLE_RETAIL_CASOUNDMANAGER_ENABLEFLAGS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_LHA_CEVENTPLAYOPTIONS_SENDFADERS
#include "engine/C3DVector.h"
#include "engine/CASoundManager.h"
#include "engine/CASoundEmitter.h"
#include "engine/CASoundBank.h"
#include "engine/CSoundInit.h"
#include "engine/LHA_CEventPlayOptions.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
double __fastcall GFGetTime();                            // 0x009E1BC0 ?GFGetTime@@YINXZ

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
    CSampleQueryLH(void* bank, const void* name);          // 0x00C08720
    ~CSampleQueryLH();                                     // 0x00C08780
};

class CSoundEventManager {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual long PlayEvent(const CSampleQueryLH& query, const LHA::CEventPlayOptions& options);   // slot 16 (+0x40) (descriptive)
};

class CSoundSystem {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual CSoundEventManager* GetEventManager();        // slot 6 (+0x18) 0x00C0A590 (descriptive)
};

class CSoundBankLH : public CASoundBank {
public:
    void* m_pSystem;                                      // +0x0C
    void* m_pBank;                                        // +0x10
    void* GetEngineBank() const { return m_pBank; }      // (descriptive)
};

class CSoundManagerLH : public CASoundManager {
public:
    CSoundSystem* m_pSoundSystem;                         // +0x40
    unsigned char _pad44[0x74 - 0x44];
    C3DVector     m_vector;                               // +0x74
};

class CSoundEmitterLH : public CASoundEmitter {
public:
    class CActiveSound : public CBaseClassNonCopyable {
    public:
        unsigned char _body[0x20];
        CActiveSound(long sampleID, CASoundBank& bank, const CCharString& name, float startTime, bool is3D, bool headRelative);   // 0x00AAB8B0
    };
    CSoundManagerLH*                           PManager;      // +0x04
    C3DVector                                  Position;      // +0x08
    std::list<CCountedPointer<CActiveSound> >  ActiveSounds;  // +0x14
    bool                                       Positional;    // +0x18
    bool                                       Playing;       // +0x19
    bool                                       Active;        // +0x1A
    long                                       EmitterID;     // +0x1C
    virtual unsigned long PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, const CSoundInit& init, bool attached);
};

unsigned long CSoundEmitterLH::PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, const CSoundInit& init, bool attached)
{
    if (!PManager->m_pSoundSystem || !PManager->SoundEnabled)
        return -1;
    LHA::CEventPlayOptions options;
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
        int bit = FindFirstSetBit(bits, 16);   // as retail: the exhausted GAIN mask, not pitchBits (VC7.1 hoists the invariant call)
        options.SetFlag(bit, true);
        pitchBits ^= 1 << bit;
    }
    if (options.HeadRelative) {
        const C3DVector& head = PManager->m_vector;
        options.Pos.X = head.X;
        options.Pos.Y = head.Y;
        options.Pos.Z = head.Z;
    } else if (options.Is3D) {
        options.Pos.X = Position.X;
        options.Pos.Y = Position.Y;
        options.Pos.Z = Position.Z;
    }
    options.EmitterId = (init.OverrideID == -1) ? EmitterID : init.OverrideID;
    long handle = PManager->m_pSoundSystem->GetEventManager()->PlayEvent(CSampleQueryLH(static_cast<CSoundBankLH&>(bank).GetEngineBank(), name.GetAsCharArray()), options);
    if (handle >= 0)
        ActiveSounds.push_back(CCountedPointer<CActiveSound>(new CActiveSound(handle, bank, name, (float)GFGetTime(), options.Is3D, options.HeadRelative)));
    Playing = true;
    return handle;
}