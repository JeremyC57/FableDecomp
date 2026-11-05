// CTCTalk::IsSpeechSampleLoadedAndPlaying() const -- while a speech sample is pending and its frame window has not run out, ask the owner's speech emitter whether the sample is loaded and playing; otherwise true
// Leaf: IsSpeechSampleLoadedAndPlaying
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_FrameRate (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E2DC0, length 147 bytes (padded row 160).
// Callees: the world frame getter @ 0x00416225 on g_pWorld @ 0x013B86A0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const
// @ 0x004365B0 (inlined GetTC of interface 0x10 = bit 16 of CThing+0x20), the emitter's sample query @ 0x0066AE60 (takes the sample id,
// returns bool; manifest label "CScriptGameResourceObjectScriptedThing::GetScriptThing" is propagated).
// Globals: an int frame-rate setting @ 0x01375550 (15 at rest), constant 10000.0f @ 0x0125FFE0.
// Layout (Ego_r.pdb CTCTalk = retail): SpeechSample +0x0C, SampleStartedFrame +0x10, SampleID +0x28.
// Shape: the global is copied into a named local before the frame call (both are `fild`ed from the frame; the lookup key later reuses its
// slot); the sample id test is unsigned (`jbe`); `!(elapsed > limit)` gives the NaN-aware `test ah,5 ; jnp` (`<=` gives `test ah,1`);
// GetTC must be `__forceinline` or it is emitted out of line.
class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CSpeechEmitter {
    bool IsSampleLoadedAndPlaying(unsigned long sampleID);   // 0x0066AE60
};
class CWorld { public:
    long GetFrame();                            // 0x00416225
};
extern CWorld* g_pWorld;                        // 0x013B86A0
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CTCTalk { public:
    void*         __vfptr;                      // +0x00 (CTCBase)
    const CThing* ParentThing;                  // +0x04
    unsigned char _base08[4];
    long          SpeechSample;                 // +0x0c
    long          SampleStartedFrame;           // +0x10
    long          SampleDurationFrames;         // +0x14
    long          Attitude;                     // +0x18
    float         MimeStartTime;                // +0x1c
    float         SampleLoadedTime;             // +0x20
    void*         PSpeechBank;                  // +0x24
    unsigned long SampleID;                     // +0x28
    bool IsSpeechSampleLoadedAndPlaying() const;
};

bool CTCTalk::IsSpeechSampleLoadedAndPlaying() const
{
    if (SpeechSample > 0 && SampleID > 0) {
        long rate = CMainGameComponent::GetConstantFPS();
        long elapsed = g_pWorld->GetFrame() - SampleStartedFrame;
        if (!((float)elapsed > (float)rate * 10000.0f) && ParentThing->HasTC(TCI_SOUND_PLAYER))
            return ((CSpeechEmitter*)((CSpeechEmitter*)&ParentThing->GetTC(TCI_SOUND_PLAYER)))->IsSampleLoadedAndPlaying(SampleID);
    }
    return true;
}