// CSoundEmitterLH::StopAllCriteriaContainingString(const CCharString& text) -- when the sound system is up: stop every still-playing active sound whose name contains the text and mark it -1, then prune the list
// Leaf: ?StopAllCriteriaContainingString@CSoundEmitterLH@@UAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAC460, 133 bytes (`ret 4`).
// Name (agentNF3, 2026-11-10): vtable 0x0129E6EC (RTTI CSoundEmitterLH : CASoundEmitter : CBaseClass) slot 12 = 0x00AAC460;
//   engine/CASoundEmitter.h slot 12 (Fable Anniversary 360 PDB ?StopAllCriteriaContainingString@CSoundEmitterUN@@UAAXABVCCharString@@@Z).
//   Was landed as the descriptive CSoundEmitterLH::StopSoundsContaining over a local model (catalog label
//   "CModeHoverBase::RemoveHoverAnimsBySubstring" propagated).
// Model: the landed CSoundEmitterLH rows' (StopSpecificSample 0x00AAC400 et al.): shared engine/CASoundEmitter.h base, descriptive
//   members (PManager +0x04, ActiveSounds +0x14, the STLport list of CCountedPointer<CActiveSound>).
// Callees: strstr @ 0x00BFEB3A (`__cdecl`, the sound's name first), the sound system's slot 6 (event manager), its slot 22 (+0x58,
//   StopEvent), CSoundEmitterLH::RemoveInactiveSounds @ 0x00AABCA0; CCharString::NullChar 0x0129AAF4 (the inline GetAsCharArray).
// Shape: the -1 lives in ebp for the compare and the store; both texts through one null-case-first accessor.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CASOUNDMANAGER_SIZE40
#include "engine/C3DVector.h"
#include "engine/CASoundManager.h"
#include "engine/CASoundEmitter.h"
#include "engine/CASoundBank.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"

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
    };
    CSoundManagerLH*                           PManager;      // +0x04
    C3DVector                                  Position;      // +0x08
    std::list<CCountedPointer<CActiveSound> >  ActiveSounds;  // +0x14
    bool                                       Positional;    // +0x18
    bool                                       Playing;       // +0x19
    bool                                       Active;        // +0x1A
    long                                       EmitterID;     // +0x1C
    void RemoveInactiveSounds();                                             // 0x00AABCA0
    virtual void StopAllCriteriaContainingString(const CCharString& text);
};
extern "C" char* __cdecl strstr(const char* text, const char* pattern);   // 0x00BFEB3A
void CSoundEmitterLH::StopAllCriteriaContainingString(const CCharString& text)
{
    if (PManager->m_pSoundSystem) {
        for (std::list<CCountedPointer<CActiveSound> >::iterator it = ActiveSounds.begin(); it != ActiveSounds.end(); ++it) {
            CActiveSound* sound = it->Data;
            if (sound->SampleID != -1 && strstr(sound->Name.GetAsCharArray(), text.GetAsCharArray())) {
                PManager->m_pSoundSystem->GetEventManager()->StopEvent(sound->SampleID);
                sound->SampleID = -1;
            }
        }
        RemoveInactiveSounds();
    }
}
