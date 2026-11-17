// NPlayerGui::CDrawNPCName::ShowScreenshotMessage() -- start the screenshot message: ScreenShotMessageCounter (+0x2C) = the current world frame.
// Leaf: ?ShowScreenshotMessage@CDrawNPCName@NPlayerGui@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00657280, length 19 bytes (`mov esi,ecx ; mov ecx,[0x013B86A0] ; call 0x004167C8 ;
//   mov [esi+0x2C],eax`).
// Identity (NF56, round 7, queue q10404): Ego_r.pdb 0x00699480 and FableWin.pdb 0x02114070 ?ShowScreenshotMessage@CDrawNPCName@NPlayerGui@@QAEXXZ;
//   engine/NPlayerGui_CDrawNPCName.h puts ScreenShotMessageCounter at +0x2C (the ctor's -1 store, 0x0043C340) and its SHOWSCREENSHOTMESSAGE
//   opt-in documents this address; the landed caller CPlayerGui::ShowScreenshotMessage 0x004379E0 already binds this spelling. The
//   catalog label CAIStateGroup_BuyFromSeller::BoughtSomething was a propagated guess (that class's FrameLastBought is at +0x14, not +0x2C).
//   Was the stand-in `C::Bought` over a local CMainGameComponent model.
// Callees: CMainGameComponent::GetCurrentWorldFrame() const @ 0x004167C8 on the protected static PMainGameComponent (0x013B86A0), read
//   through the inline static accessor Get().
#define FABLE_RETAIL_NPLAYERGUI_CDRAWNPCNAME_SHOWSCREENSHOTMESSAGE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLDFRAME
#include "engine/NPlayerGui_CDrawNPCName.h"
#include "engine/CMainGameComponent.h"

void NPlayerGui::CDrawNPCName::ShowScreenshotMessage()
{
    ScreenShotMessageCounter = CMainGameComponent::Get()->GetCurrentWorldFrame();
}
