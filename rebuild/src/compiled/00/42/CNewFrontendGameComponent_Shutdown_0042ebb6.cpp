// CNewFrontendGameComponent::Shutdown() -- tear the front end down: drop the front-end def, unregister the component, release the scene, hand the game state back unless a game is being created, unload the XMV code, empty the save-file lists and clear the screen to opaque black
// Leaf: ?Shutdown@CNewFrontendGameComponent@@AAEXXZ
// Naming wave 18: PDisplayManager is GPDisplayManager (?GPDisplayManager@@3PAVCDisplayManager@@A); ChatGPT pass 10 audit, raw Ego_r public.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042EBB6, 198 bytes (`ret`). Size-optimised TU with a frame pointer: `#pragma optimize("y",
//   off)` + `#pragma optimize("s", on)` (as the landed CNewFrontendGameComponent rows of this module).
// Sound manager (agentSND2, 2026-11-10): engine/CASoundManager.h PSoundManager with the slot names and the FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
//   non-virtuals (Fable Anniversary 360 PDB names, TU order).
// Callees: CStartupDefinitionManager::Set 0x004336B0 (Ego_r + FableWin ?Set@CStartupDefinitionManager@@SIXPAV1@@Z, static __fastcall,
//   called with 0), CleanUpInterfaces 0x0042DD28 and ReleaseXMVCode 0x0042DBD8, PSoundManager's (0x013B8394) slots 16 StopAllSounds
//   (+0x40) and 18 StopMusic (+0x48, fade 0) and its UnloadAllBanks 0x00991750 / CloseAllBanks 0x009918F0, vector<CWideString>::erase(first,
//   last) 0x00412450 and vector<CDateAndTime>::erase 0x00411B00 (out of line in /Os), CDisplayManager::ClearRenderTarget(const CRGBColour&,
//   unsigned long) 0x009BE420 and SwapScreens 0x009BEEB0 on GPDisplayManager (0x013B8390); the private static
//   CNewFrontendGameComponent::m_pNewFrontendGameComponent 0x013B871C (the live component) and PFontManager 0x013B838C, whose inline
//   SetLoadFontGlyphsAsynchronously(true) stores the byte +0x38 (Ego_r.pdb; engine/CFontManager.h FABLE_RETAIL_CFONTMANAGER_ASYNC).
// Header port (NF231, land1122nf230): engine/CNewFrontendGameComponent.h (RUN + the FILENAMES_FILETIMES opt-in: FileNames
//   std::vector<CWideString> +0x6C, FileTimes std::vector<CDateAndTime> +0x78, Ego_r + Ego_d), m_pFrontEndDef CDefPointer<const
//   CFrontEndDef> +0x3C (both PDBs); `m_pFrontEndDef = 0` is FableWin ?Reset@?$CDefPointer@$$CBVCFrontEndDef@@@@QAEXPBVCFrontEndDef@@@Z
//   Reset(0) (release, then the second null store 0x0042EBD6). Was a row-local model of the class, CDefPointer, vector, CWideString,
//   CDateAndTime, CRGBColour, CDisplayManager and CFrontEndDef, a CFrontendGlobal_013b838c stand-in and SetActiveFrontend_004336b0.
// Shape: clear() is erase(begin(), end()) pushing last then first; the colour is a CRGBColour temporary built R, G, B, A.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RESET
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_FILENAMES_FILETIMES
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_SET
#define FABLE_RETAIL_CFONTMANAGER_ASYNC
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CPIXELFORMAT
#include "engine/CNewFrontendGameComponent.h"
#include "engine/CStartupDefinitionManager.h"
#include "engine/CFrontEndDef.h"
#include "engine/CRGBColour.h"
#include "engine/IDirect3DDevice9.h"
#include "engine/CDisplayManager.h"
#include "engine/CFontManager.h"
#include "engine/CASoundManager.h"
extern CDisplayManager* GPDisplayManager;                        // 0x013B8390

void CNewFrontendGameComponent::Shutdown()
{
    m_pFrontEndDef.Reset(0);
    CStartupDefinitionManager::Set(0);
    m_pNewFrontendGameComponent = 0;
    CleanUpInterfaces();
    PFontManager->SetLoadFontGlyphsAsynchronously(true);
    if (!m_bCreateGame && PSoundManager) {
        PSoundManager->StopAllSounds();
        PSoundManager->StopMusic(0);
        PSoundManager->UnloadAllBanks();
        PSoundManager->CloseAllBanks();
    }
    ReleaseXMVCode();
    FileNames.clear();
    FileTimes.clear();
    if (GPDisplayManager) {
        GPDisplayManager->ClearRenderTarget(CRGBColour(0, 0, 0, 0xff), 7);
        GPDisplayManager->SwapScreens();
    }
}
