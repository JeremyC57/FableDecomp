// CV_ExposeMayorScript::CheckLampSlot() -- while the lights have not been flashed and the Haunted House level is loaded, make the hand lamp the preferred quick-access item whenever the hero is near the flash-light point (and drop the preference again when he moves away); on leaving, drop it
// Leaf: ?CheckLampSlot@CV_ExposeMayorScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E76CA0, 485 bytes (496 with padding), plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF / IsDistanceBetweenThingsOver 0x00CBE3EA (`__fastcall`, distance on the
//   stack), CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface (+0x40) NewScriptFrame (slot 7),
//   IsLevelLoaded (slot 11), GetHero (slot 70), GetThingWithScriptName (slot 72, hidden result),
//   SetPreferredQuickAccessItem (slot 312).
// Strings: "EM_FlashLightPoint" 0x012E97BC, "HauntedHouse" 0x012E21F0, "OBJECT_HAND_LAMP" 0x012D16F8.
// Shape: the loop test is a named bool whose level-name temporary is built under a construction flag (zeroed at entry).
// Layout (retail = Ego_r.pdb - 0x14): HeroFlashedLightsCorrectly +0x50, MaxFlashDistFromStables +0x9c.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CScriptThing { public:                                       // vtable 0x01238C8C, 12 bytes
    void* PImpData;
    void* PImpInfo;
    virtual ~CScriptThing();                                        // 0x004AA840
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance); // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);  // 0x00CBE3EA
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptBase { public:                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
namespace NScript {
class CV_ExposeMayorScript : public CQuestScriptBase {
public:
    long  Path;                                             // +0x48 PATH_ID
    long  StageNumber;                                      // +0x4c STAGE_IN_PATH
    bool  HeroFlashedLightsCorrectly;                       // +0x50
    bool  PrisonerTalkedAboutLamp;                          // +0x51
    bool  ResetLamp;                                        // +0x52
    bool  SaidYesToBecomeMayor;                             // +0x53
    bool  QuestCardGiven;                                   // +0x54
    bool  MayorExposeGhostCutscenePlayed;                   // +0x55
    unsigned char _pad56[0x9c - 0x56];                      // seh_Suspicious +0x58 .. Evidence +0x98
    float MaxFlashDistFromStables;                          // +0x9c
    void CheckLampSlot();
};
} // namespace NScript
using NScript::CV_ExposeMayorScript;

void NScript::CV_ExposeMayorScript::CheckLampSlot()
{
    CScriptThing point = Game->GetThingWithScriptName("EM_FlashLightPoint");
    bool lampSet = false;
    for (;;) {
        bool inHouse = !HeroFlashedLightsCorrectly && Game->IsLevelLoaded("HauntedHouse");
        if (!inHouse)
            break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (lampSet) {
            if (IsActiveThreadTerminating())
                return;
            if (IsDistanceBetweenThingsOver(Game->GetHero(), point, MaxFlashDistFromStables)) {
                if (IsActiveThreadTerminating())
                    return;
                Game->SetPreferredQuickAccessItem("OBJECT_HAND_LAMP", 2, -1);
                lampSet = false;
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            if (IsDistanceBetweenThingsUnder(Game->GetHero(), point, MaxFlashDistFromStables)) {
                if (IsActiveThreadTerminating())
                    return;
                Game->SetPreferredQuickAccessItem("OBJECT_HAND_LAMP", 2, 1000000);
                lampSet = true;
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetPreferredQuickAccessItem("OBJECT_HAND_LAMP", 2, -1);
}