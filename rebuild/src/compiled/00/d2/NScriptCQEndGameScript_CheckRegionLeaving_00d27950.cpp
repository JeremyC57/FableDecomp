// CQ_EndGameScript::CheckRegionLeaving() -- end-game quest thread: while running, keep the Darkwood5 creature generators off; wait until Darkwood5 is loaded, kick off the quest start screen once, wait for ShowUndeadBar, show the undead bar (when UndeadBar is below the def maximum), wait until Darkwood5 is unloaded again and remove the bar
// Leaf: ?CheckRegionLeaving@CQ_EndGameScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D27950, 686 bytes (688 with padding).
// Label correction: catalog "CBeggar::Main" is propagated. The quest Main stores 0x00D27950 as the functor of the thread
//   named "CheckRegionLeaving" (0x00D277DC..0x00D27809), and the members match Ego_r.pdb CQ_EndGameScript (- 0x14).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0; CGameScriptInterface (+0x40) slots 7 NewScriptFrame, 12 IsRegionLoaded, 309 KickOffQuestStartScreen,
//   321 DisplayQuestInfo, 324 AddQuestInfoBar, 338 RemoveQuestInfoElement, 558 SetCreatureGeneratorsEnabled,
//   655 GetActiveQuestName. Script def global 0x0143E90C (+0xbe4: the undead bar maximum).
//   Literals "Darkwood5" 0x012CBB58, "HUD_QUEST_ICON_UNDEAD" 0x012CBB64, "" 0x0122D70E.
// Layout: CQuestScriptBase (retail 0x48: GSI +0x40); Ego_r.pdb CQ_EndGameScript - 0x14: UndeadBar +0x48, GuiCounter +0x4c,
//   QuestStartScreened +0x51, ShowUndeadBar +0x54. CRGBColour {B, G, R, A} with an R, G, B, A ctor (landed CRockTroll::Main).
// Shape: the region tests are string temporaries in loop conditions: the first
//   loop the plain negation (neg/sbb/inc), the second `? true : false` (test/setne).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad[0xbe4];
    float         UndeadBarMaximum;                    // +0xbe4 (retail offset)
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
class CScriptBase { public:                                   // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                  // 0x00CB7940
};
struct CQuestScriptBase : CScriptBase {                // retail 0x48 bytes
    CGameScriptInterface* GSI;                         // +0x40
    void*                 Owner;                       // +0x44
};
namespace NScript {
class CQ_EndGameScript : public CQuestScriptBase {
public:
    float UndeadBar;                                   // +0x48
    long  GuiCounter;                                  // +0x4c
    bool  SeenAbbey;                                   // +0x50
    bool  QuestStartScreened;                          // +0x51
    bool  SeenMotherDisappear;                         // +0x52
    bool  MotherSpoken;                                // +0x53
    bool  ShowUndeadBar;                               // +0x54
    void CheckRegionLeaving();
};
} // namespace NScript
using NScript::CQ_EndGameScript;

void NScript::CQ_EndGameScript::CheckRegionLeaving()
{
    while (!IsActiveThreadTerminating()) {
        GSI->SetCreatureGeneratorsEnabled("Darkwood5", false);
        while (!GSI->IsRegionLoaded("Darkwood5")) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (!QuestStartScreened) {
            if (IsActiveThreadTerminating())
                return;
            GSI->KickOffQuestStartScreen(GSI->GetActiveQuestName(), true, false);
            QuestStartScreened = true;
        }
        while (!ShowUndeadBar) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (UndeadBar < pGlobalScriptDef->UndeadBarMaximum) {
            if (IsActiveThreadTerminating())
                return;
            GSI->DisplayQuestInfo(true);
            GuiCounter = GSI->AddQuestInfoBar(pGlobalScriptDef->UndeadBarMaximum, 0.0f, CRGBColour(0, 255, 0, 255), CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_UNDEAD", "", 1.0f);
        }
        while (GSI->IsRegionLoaded("Darkwood5") ? true : false) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        GSI->RemoveQuestInfoElement(GuiCounter);
        GSI->NewScriptFrame();
    }
}