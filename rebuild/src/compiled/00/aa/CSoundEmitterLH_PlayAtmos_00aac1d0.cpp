// CSoundEmitterLH::PlayAtmos(const CCharString& name) -- play the named atmos at the emitter position: look it up on the manager, start it (emitter id, volume 1, the position), and on success track it as a new active sound; returns the event handle or -1
// Leaf: ?PlayAtmos@CSoundEmitterLH@@UAEKABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAC1D0, 264 bytes, thiscall `ret 4`.
// Name (agentSND2, 2026-11-10): vtable 0x0129E6EC (RTTI CSoundEmitterLH : CASoundEmitter : CBaseClass) slot 5; the slot name
//   is engine/CASoundEmitter.h's (real, Fable Anniversary 360 PDB CSoundEmitterUN publics + the TLC CThingSoundEmitter forwarders); was CSoundEmitterLH::vfunc_5.
// Model: CSoundEmitterLH derives from the shared engine/CASoundEmitter.h and overrides the slot with its header signature.
//   No PDB covers the audio module, so its members are descriptive (one spelling in every self row): PManager +0x04 (the
//   owning CSoundManagerLH, ctor 0x00AAC580), Position +0x08, ActiveSounds +0x14 (STLport list of
//   CCountedPointer<CActiveSound>, push_back 0x00AACCC0), Positional +0x18 / Playing +0x19 / Active +0x1A (the getters
//   Is3D 0x00AAB9D0 / IsPlaying 0x00AAB9E0 / IsActive 0x00AAB9F0), EmitterID +0x1C (CSoundManagerLH::GetNextEmitterID).
//   CSoundManagerLH derives from the shared engine/CASoundManager.h (opt-in FABLE_RETAIL_CASOUNDMANAGER_SIZE40: the base is
//   0x40 bytes, the LH members start with the libpk sound system at +0x40, as the landed LH ctor / dtor rows model it).
//   The libpk classes have no PDB or RTTI: CSoundSystem (vtable 0x012B1E24, the landed spelling) slot 6 0x00C0A590 returns
//   its +0x38 CSoundEventManager (vtable 0x012B2748, ctor 0x00C11D00); the event-manager slot names are descriptive, from
//   what the emitter does with them.
// Callees: CSoundManagerLH::FindByName 0x00A3ED30 (the name map at +0x64; landed spelling, which this row used to call
//   CSoundBankLH::FindSample -- one address, one name), GetPos (slot 17, once per component), the atmos object's slot 2
//   (+0x08, play), operator new 0x00BFEA1A (0x24 bytes), GFGetTime 0x009E1BC0, CActiveSound::CActiveSound 0x00AAB910,
//   CCountedPointer<CActiveSound>(CActiveSound*) 0x00AAC820 (out of line), list::push_back 0x00AACCC0 (STLport, same-TU
//   COMDAT: FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE), operator delete 0x00BFE9BC.
// Map value type: the landed FindByName / NewAtmos rows spell the +0x64 map's value CSoundBankLH*, but RTTI CSoundBankLH's slot 2 is
//   0x00A0B6C0 (UnloadCompiledBank) while this object's slot 2 starts an event: the type name is debt (NOTES).
// Shape: the position is a zero-initialised C3DVector local filled one component per GetPos() call; the counted pointer is an
//   unnamed temporary handed to push_back and released inline; both failures share the final `return -1` tail.
#define FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#include "engine/C3DVector.h"
#include "engine/CASoundManager.h"
#include "engine/CASoundEmitter.h"
#include "engine/CASoundBank.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
double __fastcall GFGetTime();                            // 0x009E1BC0 ?GFGetTime@@YINXZ

class CSoundBankLH {                                      // the +0x64 map's value (landed spelling; see the note above)
public:
    virtual void v00(); virtual void v01();
    virtual long Play(long emitterID, float volume, const C3DVector* position, long a, long b);   // slot 2 (+0x08)
};

class CSoundEventManager {                                // libpk sound-event manager, vtable 0x012B2748 (descriptive slot names)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void StopEvent(long sampleID);                                   // slot 22 (+0x58) 0x00C11330
    virtual void ReleaseEventLoop(long sampleID);                            // slot 23 (+0x5C) 0x00C114B0
    virtual void v24();
    virtual void SetEventVolume(long sampleID, float volume);                // slot 25 (+0x64) 0x00C113B0
    virtual void SetEventPitch(long sampleID, float pitch);                  // slot 26 (+0x68) 0x00C11430
    virtual void SetEventPosition(long sampleID, const C3DVector& position); // slot 27 (+0x6C) 0x00C11520
    virtual float GetEventValue(long sampleID, float* value);                // slot 28 (+0x70) 0x00C11650
    virtual long GetEventState(long sampleID);                               // slot 29 (+0x74) 0x00C11C00 (1 / 2 = playing)
};

class CSoundSystem {                                      // libpk sound system, vtable 0x012B1E24 (landed spelling)
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual CSoundEventManager* GetEventManager();                           // slot 6 (+0x18) 0x00C0A590 (descriptive)
};

class CSoundBankLH;

class CSoundManagerLH : public CASoundManager {           // vtable 0x0129D2C4
public:
    CSoundSystem* m_pSoundSystem;                         // +0x40 (the landed CSoundManagerLH rows' spelling)
    CSoundBankLH* FindByName(const CCharString& name);    // 0x00A3ED30
};

class CSoundEmitterLH : public CASoundEmitter {           // vtable 0x0129E6EC
public:
    class CActiveSound : public CBaseClassNonCopyable {   // vtable 0x0129E67C, 0x24 bytes (descriptive members)
    public:
        long          State;                              // +0x04 (0 once started by PlaySample)
        long          SampleID;                           // +0x08 the libpk event handle (-1 once stopped)
        CASoundBank*  PBank;                              // +0x0C the bank PlaySample / PlaySoundEvent played from
        float         StartTime;                          // +0x10
        bool          Started;                            // +0x14
        bool          Stopping;                           // +0x15
        long          Sample;                             // +0x18 the sample index in that bank
        CCharString   Name;                               // +0x1C
        CSoundBankLH* PAtmos;                             // +0x20
        CActiveSound(long sampleID, CSoundBankLH* atmos, float startTime);   // 0x00AAB910
    };
    CSoundManagerLH*                           PManager;      // +0x04
    C3DVector                                  Position;      // +0x08
    std::list<CCountedPointer<CActiveSound> >  ActiveSounds;  // +0x14
    bool                                       Positional;    // +0x18
    bool                                       Playing;       // +0x19
    bool                                       Active;        // +0x1A
    long                                       EmitterID;     // +0x1C
    virtual unsigned long PlayAtmos(const CCharString& name);
};

unsigned long CSoundEmitterLH::PlayAtmos(const CCharString& name)
{
    CSoundBankLH* atmos = PManager->FindByName(name);
    if (atmos) {
        C3DVector position(0.0f, 0.0f, 0.0f);
        position.X = GetPos().X;
        position.Y = GetPos().Y;
        position.Z = GetPos().Z;
        long sampleID = atmos->Play(EmitterID, 1.0f, &position, 0, 0);
        if (sampleID >= 0) {
            ActiveSounds.push_back(CCountedPointer<CActiveSound>(new CActiveSound(sampleID, atmos, (float)GFGetTime())));
            Playing = true;
            return sampleID;
        }
    }
    return -1;
}
