// CGameScriptInterface::Play2DSound(const CCharString& symbol) const -- plays the engine sound bank's sample for the symbol once, unpositioned, through the game state's sound player; the invalid sound handle when the symbol has no sample
// Leaf: ?Play2DSound@CGameScriptInterface@@UBEKABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088F570, length 145 bytes.
// Sound manager (agentSND2, 2026-11-10): PSoundManager of engine/CASoundManager.h, slot 9 Get2DEmitter and its slot 3 PlaySample;
//   engine/CSoundInit.h (FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR) and engine/CASoundBank.h (GetSoundIndexFromSymbol opt-in); was a local
//   CGameState_Retail / CSoundPlayer_Retail / CSoundInit / CASoundBank model.
// Callees: CASoundBank::GetSoundIndexFromSymbol @ 0x00A01920 on the engine's sound bank (g_pEngine @ 0x013B86A0, +0x10), the game state's (0x013B8394) virtual slot 9 (+0x24, the sound player),
// the player's virtual slot 3 (+0x0c, PlaySample(bank, sample, loop, init, attached)); the invalid handle global @ 0x0129D25C.
// Shape: the CSoundInit default constructor of the landed PlaySoundAtPos 0x00891170 (Volume/Pitch/Pan/NoPlays/
// PitchFaderBits/OverrideID in its list, PlaybackType and GainFaderBits assigned in its body), the caller then sets
// GainFaderBits = 0x8010 (retail stores +0,+4,+8,+c,+18,+1c,+10,+14 and CSEs the constant 1 into edi for NoPlays,
// PlaybackType and the `attached` push); the sound bank comes through an inline GetSampleBank() (the engine global is
// loaded before the symbol argument) and is re-read from the global for the PlaySample argument.
// Leaf: Play2DSound
#include <string.h>
struct CThing_Retail;
class CScriptThing;
// CCharString (Ego_r.pdb layout, as the landed CCharString::operator!= 0x0099E960): a 4-byte handle on a 17-byte block.
#pragma pack(push, 1)
struct CBasicStringChar {                   // CBasicString<char>, 13 bytes
    char*         PData;                    // +0x00
    unsigned long StringLength;             // +0x04
    unsigned long DataLength : 31;          // +0x08
    bool          UseFastExtend : 1;        // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
class CCharStringData { public:                    // 17 bytes
    CBasicStringChar Data;                  // +0x00
    long             NoRefs;                // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;           // +0x00
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
    bool IsEmpty() const { return PStringData == 0 || PStringData->Data.StringLength == 0; }
};
#define FABLE_RETAIL_CASOUNDBANK_SOUNDINDEX
#define FABLE_RETAIL_CSOUNDINIT_INLINE_CTOR
#include "engine/CASoundBank.h"
#include "engine/CSoundInit.h"
#include "engine/CASoundManager.h"
struct CEngine_Retail {
    unsigned char       _pad00[0x10];
    CASoundBank* m_pSoundBank;               // +0x10
    CASoundBank& GetSampleBank() const { return *m_pSoundBank; }
};
extern CEngine_Retail*    g_pEngine;                // 0x013B86A0
extern long               g_InvalidSoundHandle;     // 0x0129D25C
class CWorld;
struct CPlayerManager_Retail;
struct CDefinitionManager_Retail;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

unsigned long CGameScriptInterface::Play2DSound(const CCharString& symbol) const
{
    long sample = g_pEngine->GetSampleBank().GetSoundIndexFromSymbol(symbol);
    if (sample) {
        CSoundInit init;
        init.GainFaderBits = 0x8010;
        return PSoundManager->Get2DEmitter().PlaySample(g_pEngine->GetSampleBank(), sample, false, init, true);
    }
    return g_InvalidSoundHandle;
}