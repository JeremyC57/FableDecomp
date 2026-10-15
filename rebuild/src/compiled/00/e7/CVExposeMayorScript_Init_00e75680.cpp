// CV_ExposeMayorScript::Init() -- reset the path/stage, copy the evidence text and the stables flash distance from the script def, clear the six progress flags
// Retail: Fable.exe @ 0x00E75680, 73 bytes (+ int3 padding). Reached only through a vtable word (0x012E96D8).
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0.
// Layout: Ego_r.pdb CV_ExposeMayorScript - 0x14: Path +0x48, StageNumber +0x4c, HeroFlashedLightsCorrectly ..
// MayorExposeGhostCutscenePlayed +0x50..+0x55, Evidence (CCharString) +0x98, MaxFlashDistFromStables +0x9c. The script
// def @ 0x0143E90C holds the evidence string at +0x588 and the distance at +0x58c (descriptive names).
// Shape: statement order = store order (the string assignment call separates the two zero words from the six zero
// bytes, all sharing ebx); the float is copied with integer moves.
#include "fable_string.h"

struct CGameScriptInterface {                            // retail vtable @ 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                 // slot 7 (+0x1c)
};

struct CScriptBase {                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04..+0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* Game;                          // +0x40
    void* Owner;                                         // +0x44
};

struct CScriptDef {
    unsigned char _pad[0x588];
    CCharString ExposeMayorEvidence;                     // +0x588
    float ExposeMayorMaxFlashDistFromStables;            // +0x58c
};
extern CScriptDef* g_pScriptDef;                         // 0x0143E90C

struct CV_ExposeMayorScript : CQuestScriptBase {
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

void CV_ExposeMayorScript::Init()
{
    Path = 0;
    StageNumber = 0;
    Evidence = g_pScriptDef->ExposeMayorEvidence;
    HeroFlashedLightsCorrectly = false;
    PrisonerTalkedAboutLamp = false;
    ResetLamp = false;
    SaidYesToBecomeMayor = false;
    QuestCardGiven = false;
    MayorExposeGhostCutscenePlayed = false;
    MaxFlashDistFromStables = g_pScriptDef->ExposeMayorMaxFlashDistFromStables;
}