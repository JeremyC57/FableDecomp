// CV_AmbushScamScript::AssignMoralityEtcForEvilEnding() -- give the hero the ambush-scam good morality, renown and experience from the script def
// Leaf: ?AssignMoralityEtcForEvilEnding@CV_AmbushScamScript@@
// Retail: Fable.exe @ 0x00E2BAC0, 77 bytes (+ int3 padding). Called once from the quest's ending code.
// Callees (virtual, CGameScriptInterface through Game at +0x40): GiveHeroMorality(float) slot 156 (+0x270),
// GiveHeroRenownPoints slot 161 (+0x284), GiveHeroExperience slot 128 (+0x200).
// Data: the script def @ 0x0143E90C. Its retail AmbushScam block sits 4 bytes below Ego_r.pdb's CScriptDef offsets:
// Experience +0x2a8, Renown +0x2ac, MoralityEvil +0x2b0, MoralityGood +0x2b4 -- this body reads +0x2b4 (MoralityGood).
// Label note: with the good-morality value this is the good-ending twin of 0x00E2BB10; the catalog name is shared.
// Shape: each argument is read through the def global and pushed as a dword (a float lvalue argument is copied with
// integer moves); the interface pointer is re-read from +0x40 for each call.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptDef { public:
    unsigned char _pad[0x2a8];
    long AmbushScamExperience;                           // +0x2a8
    long AmbushScamRenown;                               // +0x2ac
    float AmbushScamMoralityEvil;                        // +0x2b0
    float AmbushScamMoralityGood;                        // +0x2b4
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

struct CV_AmbushScamScript {
    unsigned char _base[0x40];
    CGameScriptInterface* Game;                          // +0x40
    void AssignMoralityEtcForEvilEnding();
};

void CV_AmbushScamScript::AssignMoralityEtcForEvilEnding()
{
    Game->GiveHeroMorality(pGlobalScriptDef->AmbushScamMoralityGood);
    Game->GiveHeroRenownPoints(pGlobalScriptDef->AmbushScamRenown);
    Game->GiveHeroExperience(pGlobalScriptDef->AmbushScamExperience);
}