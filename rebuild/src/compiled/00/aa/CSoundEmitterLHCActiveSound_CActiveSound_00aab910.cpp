// CSoundEmitterLH::CActiveSound::CActiveSound(long sampleID, CSoundBankLH* atmos, float startTime) -- CBaseClassNonCopyable(), own vtable, state 2, the event handle, no bank, the start time, started, an empty name, the atmos
// Leaf: ??0CActiveSound@CSoundEmitterLH@@QAE@JPAVCSoundBankLH@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAB910, length 71 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (engine/CBaseClassNonCopyable.h, protected IAE as both PDBs
// spell it), CCharString::CCharString() @ 0x0099E4B0.
// Identity: vtable 0x0129E67C = CSoundEmitterLH::CActiveSound (rtti-vtables.tsv; its deleting destructor
// 0x00AAC7F0 destroys the string at +0x1c); the manifest's "CTCCarrying::CTCCarrying" label is propagated
// and rejected. The only caller, CSoundEmitterLH::PlayAtmos 0x00AAC1D0 (call at 0x00AAC27C, landed with this spelling), does
// `new (0x24 bytes) CActiveSound(sampleID, atmos, (float)GFGetTime())` with the non-negative play result, the CSoundBankLH* that
// CSoundManagerLH::FindByName 0x00A3ED30 returned and the start time. Anniversary 360 RTTI
// ??_7CActiveSound@CSoundEmitterUN@@6BCBaseClassNonCopyable@@@ (a class on CBaseClassNonCopyable). No PDB covers the audio
// module: the member names are the descriptive ones of the landed PlayAtmos model (one spelling in every row).
// Names (agentNF3, 2026-11-10): the second parameter was a descriptive CSoundEmitterSource* (struct, `U`); the local
// CBaseClassNonCopyable stand-in (public ctor, `U` key) is replaced by the shared header.
// Shape: init list in declaration order; the scheduler hoists the two argument stores above the vtable
// store, integer zeros share eax, and the name's out-of-line constructor runs before the last member's
// store. The float argument is copied through ecx.
#include "fable_string.h"
#include "engine/CBaseClassNonCopyable.h"

class CASoundBank;
class CSoundBankLH;                                       // RTTI .?AVCSoundBankLH@@ (vtable 0x0129C9DC)

class CSoundEmitterLH {
public:
    class CActiveSound : public CBaseClassNonCopyable {   // vtable 0x0129E67C, 0x24 bytes (descriptive members)
    public:
        long          State;                              // +0x04
        long          SampleID;                           // +0x08 the libpk event handle
        CASoundBank*  PBank;                              // +0x0C
        float         StartTime;                          // +0x10
        bool          Started;                            // +0x14
        bool          Stopping;                           // +0x15
        long          Sample;                             // +0x18
        CCharString   Name;                               // +0x1C
        CSoundBankLH* PAtmos;                             // +0x20
        CActiveSound(long sampleID, CSoundBankLH* atmos, float startTime);
    };
};

CSoundEmitterLH::CActiveSound::CActiveSound(long sampleID, CSoundBankLH* atmos, float startTime)
    : State(2)
    , SampleID(sampleID)
    , PBank(0)
    , StartTime(startTime)
    , Started(true)
    , Stopping(false)
    , Sample(0)
    , Name()
    , PAtmos(atmos)
{
}
