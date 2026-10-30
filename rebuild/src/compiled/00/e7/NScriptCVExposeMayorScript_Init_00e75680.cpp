// CV_ExposeMayorScript::Init() -- reset the path/stage, copy the evidence text and the stables flash distance from the script def, clear the six progress flags
// Leaf: ?Init@CV_ExposeMayorScript@NScript@@
// Retail: Fable.exe @ 0x00E75680, 73 bytes (+ int3 padding). Reached only through a vtable word (0x012E96D8).
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0.
// Layout: Ego_r.pdb CV_ExposeMayorScript - 0x14: Path +0x48, StageNumber +0x4c, HeroFlashedLightsCorrectly ..
// MayorExposeGhostCutscenePlayed +0x50..+0x55, Evidence (CCharString) +0x98, MaxFlashDistFromStables +0x9c. The script
// def @ 0x0143E90C holds the evidence string at +0x588 and the distance at +0x58c (descriptive names).
// Shape: statement order = store order (the string assignment call separates the two zero words from the six zero
// bytes, all sharing ebx); the float is copied with integer moves.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#include "fable_string.h"

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* Game;                          // +0x40
    void* Owner;                                         // +0x44
};

class CScriptDef { public:
    unsigned char _pad[0x588];
    CCharString ExposeMayorEvidence;                     // +0x588
    float ExposeMayorMaxFlashDistFromStables;            // +0x58c
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

namespace NScript {
class CV_ExposeMayorScript : public CQuestScriptBase {
public:
    long Path;                                           // +0x48 (PATH_ID)
    long StageNumber;                                    // +0x4c (STAGE_IN_PATH)
    bool HeroFlashedLightsCorrectly;                     // +0x50
    bool PrisonerTalkedAboutLamp;                        // +0x51
    bool ResetLamp;                                      // +0x52
    bool SaidYesToBecomeMayor;                           // +0x53
    bool QuestCardGiven;                                 // +0x54
    bool MayorExposeGhostCutscenePlayed;                 // +0x55
    unsigned char _seh58[0x98 - 0x56];                   // +0x58 four CScriptGameResourceObjectScriptedThingBase
    CCharString Evidence;                                // +0x98
    float MaxFlashDistFromStables;                       // +0x9c
    void Init();
};
} // namespace NScript
using NScript::CV_ExposeMayorScript;

void NScript::CV_ExposeMayorScript::Init()
{
    Path = 0;
    StageNumber = 0;
    Evidence = pGlobalScriptDef->ExposeMayorEvidence;
    HeroFlashedLightsCorrectly = false;
    PrisonerTalkedAboutLamp = false;
    ResetLamp = false;
    SaidYesToBecomeMayor = false;
    QuestCardGiven = false;
    MayorExposeGhostCutscenePlayed = false;
    MaxFlashDistFromStables = pGlobalScriptDef->ExposeMayorMaxFlashDistFromStables;
}