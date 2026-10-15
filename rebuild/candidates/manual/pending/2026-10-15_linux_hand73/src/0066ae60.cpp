// CGameScriptThing::IsSampleLoadedAndPlaying(unsigned long sampleID) -- while the global game state exists and the scripted thing (PThing) is set and not dead, ask the thing whether the sound sample is loaded and playing; else false
// Leaf: IsSampleLoadedAndPlaying
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066AE60, length 69 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 three times (on PThing at +0xc; the pointer is
// re-fetched for every access), CThing::IsSampleLoadedAndPlaying @ 0x008368A0 (the CThingSoundEmitter label of the callee
// is propagated: it is a CThing member that re-checks the game state and the dead bit, then asks the
// thing's sound emitter at +0x98).
// Shape: the global test comes first and `push esi` is hoisted above its branch because both exits
// pop it; the intelligent pointer's address is kept in esi; the callee's bool is returned as is and the
// shared false exit is `xor al,al` (nested ifs, one `return false` at the end). Twin of the landed void
// SetToKillOnLevelUnload bodies 0x0066AD90/0x0066ADD0 (same guard chain).
// `this` uses engine/CGameScriptThing.h (PThing is CIntelligentPointer<CThing>, opaque in the header,
// so it is reached through a local view of the intelligent pointer).
// The manifest label "CScriptGameResourceObjectScriptedThing::GetScriptThing" is propagated.
#include "engine/CGameScriptThing.h"

extern void* g_pGameState_013b8394;               // 0x013B8394

struct CThing_Retail {
    unsigned char  _pad00[0x91];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    bool IsSampleLoadedAndPlaying(unsigned long sampleID) const;   // 0x008368A0
};
struct CIntelligentPointer_Retail {                // CIntelligentPointer<CThing>: vptr + the pointee link
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
};

struct CGameScriptThing_Methods : CGameScriptThing {
    bool IsSampleLoadedAndPlaying(unsigned long sampleID);
};

bool CGameScriptThing_Methods::IsSampleLoadedAndPlaying(unsigned long sampleID)
{
    if (g_pGameState_013b8394) {
        CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)PThing;
        if (thing->GetPItem()) {
            if (!thing->GetPItem()->m_dead)
                return thing->GetPItem()->IsSampleLoadedAndPlaying(sampleID);
        }
    }
    return false;
}
