// CPlayerGui::SetNPCNameDisplay(const CWideString& name) -- forwards the name to the NPC-name drawer (PDrawNPCName +0x248)
// Leaf: ?SetNPCNameDisplay@CPlayerGui@@QAEXABVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004379D0: mov ecx,[ecx+248h] ; jmp 0x00657230 (the name argument passes through on the stack).
// Name: Ego_r.pdb 0x0044A6B0 + FableWin.pdb 0x01A80930 ?SetNPCNameDisplay@CPlayerGui@@QAEXABVCWideString@@@Z (engine/CPlayerGui.h
//   NPCNAMEDISPLAY; Ego_r SetNPCNameDisplay 0x0044A6B0 directly before ShowScreenshotMessage 0x0044A6C0 = retail 0x004379D0 / 0x004379E0,
//   the landed twin); the landed 0x00805600 CTCTargetingPlayer::FrameUpdate binds it. Callee Ego_r.pdb 0x00699420
//   ?SetName@CDrawNPCName@NPlayerGui@@QAEXABVCWideString@@@Z = retail 0x00657230 (landed). NF95 2026-11-19 (q11267): was the stand-in
//   T::Run (catalog Forward__Cons_val_std__allocator_std__pair_EHero_via0x248_004379d0, a propagated label).
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_NPCNAMEDISPLAY
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerGui.h"
#include "engine/NPlayerGui_CDrawNPCName.h"
void CPlayerGui::SetNPCNameDisplay(const CWideString& name)
{
    PDrawNPCName->SetName(name);
}
