// CProgressDisplay::GetPTextBank() const -- the text bank the progress display reads its tips from: the main game component's when
// it exists and its bank is initialised, else the new front end's, else none.
// Leaf: ?GetPTextBank@CProgressDisplay@@ABEPBVCDataBank@NGameText@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00497B30, 35 bytes. Ego_r.pdb 0x004A1F90 / FableWin.pdb 0x018B46B0
//   ?GetPTextBank@CProgressDisplay@@ABEPBVCDataBank@NGameText@@XZ (private const, returns const NGameText::CDataBank*); its caller
//   CProgressDisplay::InitialiseTextures 0x0049A0B0 already binds that symbol. DrawRetailDisplay 0x00498490 expands the same body.
// NF47 (round 7): was the fable_boot.h stand-in (`CDataBank* GetPTextBank() const` on the stand-in slots
//   g_FableProgressTextBankSlot_013B86A0 / g_FableGameTextBankSlot_013B871C); now engine/CProgressDisplay.h with the PDB statics
//   ?PMainGameComponent@CMainGameComponent@@1PAV1@A 0x013B86A0 and ?m_pNewFrontendGameComponent@CNewFrontendGameComponent@@0PAV1@A
//   0x013B871C read through their inline Get(), and the inline IsTextBankInitialised() / GetTextBank() (the counted pointers' object
//   words +0x14 / +0x60), as the landed DrawRetailDisplay 0x00498490 spells the expansion (its include prelude).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_ENDSWITH
#define FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_NULLRETURN
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D_DRAW2DTEXTUREBOX
#define FABLE_RETAIL_CRENDERMANAGER2D_LOADINGSCREEN
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_GRAPHICBANK
#define FABLE_RETAIL_CPLAYERGUIDEF_PROGRESS
#define FABLE_RETAIL_SINGLETON_ISSETUP
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK_INITIALISED
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER_AUTOSAVENAME
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING
#include "fable_user_profile.h"
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CPixelFormat.h"
#include "engine/CTexture.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CFontBank.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CNewFrontendGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CProgressDisplay.h"

const NGameText::CDataBank* CProgressDisplay::GetPTextBank() const
{
    if (CMainGameComponent::Get() && CMainGameComponent::Get()->IsTextBankInitialised())
        return &CMainGameComponent::Get()->GetTextBank();
    if (CNewFrontendGameComponent::Get() && CNewFrontendGameComponent::Get()->IsTextBankInitialised())
        return &CNewFrontendGameComponent::Get()->GetTextBank();
    return 0;
}
