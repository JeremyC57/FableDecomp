// CPlayerGui::ShowScreenshotMessage() -- forwards to the NPC-name illustrator (PDrawNPCName +0x248)
// Leaf: ?ShowScreenshotMessage@CPlayerGui@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004379E0: mov ecx,[ecx+248h] ; jmp 0x00657280.
// Name: Ego_r.pdb 0x0044A6C0 + FableWin.pdb 0x01A80960 ?ShowScreenshotMessage@CPlayerGui@@QAEXXZ (engine/CPlayerGui.h SCREENSHOTMESSAGE:
//   Ego_r Show / Hide / IsShowing 0x0044A6C0 / D0 / E0 = retail 0x004379E0 / F0 / 0x00437A00); the landed 0x0055D180 TakeQuickScreenshot
//   binds it. Callee Ego_r.pdb 0x00699480 ?ShowScreenshotMessage@CDrawNPCName@NPlayerGui@@QAEXXZ = retail 0x00657280 (overlay opt-in
//   FABLE_RETAIL_NPLAYERGUI_CDRAWNPCNAME_SHOWSCREENSHOTMESSAGE). NF33 2026-11-16: was the stand-in T::Run (catalog
//   Forward_BoughtSomething_via0x248, a propagated label).
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_SCREENSHOTMESSAGE
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_NPLAYERGUI_CDRAWNPCNAME_SHOWSCREENSHOTMESSAGE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerGui.h"
#include "engine/NPlayerGui_CDrawNPCName.h"
void CPlayerGui::ShowScreenshotMessage()
{
    PDrawNPCName->ShowScreenshotMessage();
}
