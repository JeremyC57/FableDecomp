// CTCFineDialogue::IsAllowed() -- the fine dialogue may start only when the player is offered the chance to pay, the screen is not
// faded out, no subtitle text or cutscene is up and the world is not performing a region load
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E2790, length 83 bytes. Ego_r.pdb / FableWin.pdb ?IsAllowed@CTCFineDialogue@@QAE_NXZ.
// Rebased (agentNF14, w22 item 3) onto the shared retail CTCBase so the two getters bind as the real base members:
//   0x00686D60 ?PeekComponent@CTCBase@@IBEABVCMainGameComponent@@XZ (FABLE_RETAIL_CTCBASE_COMPONENT; called with ecx = this) and
//   0x00686A70 ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ (`mov ecx,esi ; call`).
// The address-suffixed stand-ins of the earlier landing are replaced by the real inline accessors (FableWin.pdb names, two sources
// each: the PDB name + the retail per-offset read):
//   PeekComponent().PeekDisplayEngine()  CMainGameComponent +0x28 PDisplayEngine (scoped_ptr word)   FableWin 0x01A700DC
//     .IsScreenFadeOut()                 CDisplayEngine +0xBC ScreenFadeOutInfo, CFadeInFadeOutBase::IsActive (Active +0)
//                                        FableWin 0x01A70098 / 0x01A65280
//   CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText()  singleton 0x013B8790, +0x18 + DisplayingText +0xE5 = +0xFD
//   CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene()      +0x18 + InCutscene +0xDE = +0xF6 (the order CTCBoastUI::Activate
//                                        0x0056AD40 also uses)
//   GetWorld().IsPerformingRegionLoad()  CWorld +0x104 RegionLoadStatus, `!= 0 && != 9` (Ego_r.pdb ?IsPerformingRegionLoad@CWorld@@QBE_NXZ)
// Layout: Ego_r.pdb CTCFineDialogue (28 bytes) over CTCBase: Fine +0xc, Money +0x10, GivePlayerChanceToPayFine +0x14,
// WaitingForConfirmation +0x15, Village +0x18.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_QUESTCOMPLETIONUI
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CPlayerGui.h"
#include "engine/CWorld.h"

class CTCVillage;

class CTCFineDialogue : public CTCBase {
public:
    bool IsAllowed();
private:
    long        Fine;                              // +0x0c
    long        Money;                             // +0x10
    bool        GivePlayerChanceToPayFine;         // +0x14
    bool        WaitingForConfirmation;            // +0x15
    CTCVillage* Village;                           // +0x18
};

bool CTCFineDialogue::IsAllowed()
{
    if (!GivePlayerChanceToPayFine)
        return false;
    if (PeekComponent().PeekDisplayEngine().IsScreenFadeOut())
        return false;
    if (CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText())
        return false;
    if (CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene())
        return false;
    if (GetWorld().IsPerformingRegionLoad())
        return false;
    return true;
}
