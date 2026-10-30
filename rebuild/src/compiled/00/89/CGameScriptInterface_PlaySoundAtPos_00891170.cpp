// CGameScriptInterface::PlaySoundAtPos(const C3DVector& position, const CCharString& sound) const -- play a sound
// sample once from a new unattached sound emitter at the position; returns the sound handle or the invalid handle
// Leaf: ?PlaySoundAtPos@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00891170, length 176 bytes, `ret 8`.
// Callees: CASoundBank::GetSoundIndexFromSymbol(const CCharString&) @ 0x00A01920, the static `__fastcall`
// CThingSoundEmitter::Create(const C3DVector&, const CThing*, bool) @ 0x008369F0 (labelled
// Sound_CreateAttachedEmitter), CThingSoundEmitter::PlaySample(CASoundBank&, long, bool, const CSoundInit&, bool)
// @ 0x00836810. Globals: the main game component 0x013B86A0 (+0x10 the sample bank), the invalid sound handle
// 0x0129D25C.
// Layout: CThingSoundEmitter dead bit +0x91 bit 0; CSoundInit (PDB, 0x20 bytes) Volume, Pitch, Pan, NoPlays,
// PlaybackType, GainFaderBits, PitchFaderBits, OverrideID.
// Shape: the CSoundInit default ctor initialises Volume/Pitch/Pan/NoPlays/PitchFaderBits/OverrideID in its list and
// assigns PlaybackType and GainFaderBits in its body (retail stores PlaybackType after OverrideID); the caller then
// sets GainFaderBits = 0x8010; the sample bank comes through an inline GetSampleBank() (global loaded before the
// argument) and is re-read from the global for the PlaySample argument.

class CCharString;
class C3DVector { public: float X, Y, Z; };
class CThing;

class CASoundBank { public:
public:
    long GetSoundIndexFromSymbol(const CCharString&) const;   // 0x00A01920
};

class CSoundInit { public:
    float Volume, Pitch, Pan;
    long NoPlays, PlaybackType;
    unsigned long GainFaderBits, PitchFaderBits;
    long OverrideID;
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), PitchFaderBits(0), OverrideID(-1)
    {
        PlaybackType = 1;
        GainFaderBits = 0;
    }
};

class CThingSoundEmitter { public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;               // +0x91 bit 0
    unsigned char _flags91_1 : 7;
    static CThingSoundEmitter* __fastcall Create(const C3DVector& position, const CThing* parent, bool unattached);   // 0x008369F0
    unsigned long PlaySample(CASoundBank& bank, long sample, bool loop, const CSoundInit& init, bool attached);       // 0x00836810
};

class CMainGameComponent { public:
    unsigned char _pad00[0x10];
    CASoundBank* PSampleBank;             // +0x10
    CASoundBank& GetSampleBank() const { return *PSampleBank; }
};
extern CMainGameComponent* g_pMainGameComponent;    // 0x013B86A0
extern unsigned long g_InvalidSoundHandle;          // 0x0129D25C

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

unsigned long CGameScriptInterface::PlaySoundAtPos(const C3DVector& position, const CCharString& sound) const
{
    long sample = g_pMainGameComponent->GetSampleBank().GetSoundIndexFromSymbol(sound);
    if (sample) {
        CThingSoundEmitter* emitter = CThingSoundEmitter::Create(position, 0, true);
        if (emitter && !emitter->Dead) {
            CSoundInit init;
            init.GainFaderBits = 0x8010;
            return emitter->PlaySample(g_pMainGameComponent->GetSampleBank(), sample, false, init, false);
        }
    }
    return g_InvalidSoundHandle;
}