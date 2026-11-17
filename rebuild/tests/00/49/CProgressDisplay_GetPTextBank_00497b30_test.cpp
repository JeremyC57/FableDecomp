// Behaviour test for CProgressDisplay::GetPTextBank @ 0x00497B30 (NF47 round 7): the row now reads the PDB statics
// ?PMainGameComponent@CMainGameComponent@@1PAV1@A and ?m_pNewFrontendGameComponent@CNewFrontendGameComponent@@0PAV1@A and the
// counted pointers' object words (+0x14 / +0x60) through the shared headers, and is the private const
// ?GetPTextBank@CProgressDisplay@@ABEPBVCDataBank@NGameText@@XZ. The test defines both statics, reaches the private/protected names
// through explicit instantiations (access is not checked there, C++03 14.7.2/8) and keeps the original PassPattern.
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
#include <stdio.h>

CMainGameComponent* CMainGameComponent::PMainGameComponent = 0;
CNewFrontendGameComponent* CNewFrontendGameComponent::m_pNewFrontendGameComponent = 0;

typedef const NGameText::CDataBank* (CProgressDisplay::*GetPTextBankFn)() const;
static GetPTextBankFn g_GetPTextBank = 0;
static CMainGameComponent** g_ppMain = 0;
static CNewFrontendGameComponent** g_ppFront = 0;

template <GetPTextBankFn F> struct GrabGetPTextBank {
    GrabGetPTextBank() { g_GetPTextBank = F; }
    static GrabGetPTextBank inst;
};
template <GetPTextBankFn F> GrabGetPTextBank<F> GrabGetPTextBank<F>::inst;
template struct GrabGetPTextBank<&CProgressDisplay::GetPTextBank>;

template <CMainGameComponent** P> struct GrabMainStatic {
    GrabMainStatic() { g_ppMain = P; }
    static GrabMainStatic inst;
};
template <CMainGameComponent** P> GrabMainStatic<P> GrabMainStatic<P>::inst;
template struct GrabMainStatic<&CMainGameComponent::PMainGameComponent>;

template <CNewFrontendGameComponent** P> struct GrabFrontStatic {
    GrabFrontStatic() { g_ppFront = P; }
    static GrabFrontStatic inst;
};
template <CNewFrontendGameComponent** P> GrabFrontStatic<P> GrabFrontStatic<P>::inst;
template struct GrabFrontStatic<&CNewFrontendGameComponent::m_pNewFrontendGameComponent>;

static void* g_displayStore[0x40];
static void* g_mainStore[0x40];      // only the PTextBank object word +0x14 is read
static void* g_frontStore[0x40];     // only the m_pTextBank object word +0x60 is read
static int g_progressBank[4];
static int g_gameBank[4];

static const NGameText::CDataBank* Call()
{
    const CProgressDisplay* display = reinterpret_cast<const CProgressDisplay*>(g_displayStore);
    return (display->*g_GetPTextBank)();
}

int main()
{
    const NGameText::CDataBank* progressBank = reinterpret_cast<const NGameText::CDataBank*>(g_progressBank);
    const NGameText::CDataBank* gameBank = reinterpret_cast<const NGameText::CDataBank*>(g_gameBank);
    if (!g_GetPTextBank || !g_ppMain || !g_ppFront)
        return 9;

    if (Call() != 0)
        return 1;

    *g_ppMain = reinterpret_cast<CMainGameComponent*>(g_mainStore);
    *g_ppFront = reinterpret_cast<CNewFrontendGameComponent*>(g_frontStore);
    if (Call() != 0)
        return 2;

    g_frontStore[0x60 / sizeof(void*)] = g_gameBank;
    if (Call() != gameBank)
        return 3;

    g_mainStore[0x14 / sizeof(void*)] = g_progressBank;
    if (Call() != progressBank)
        return 4;

    *g_ppMain = 0;
    if (Call() != gameBank)
        return 5;

    printf("FABLETLC_PROGRESS_TEXT_BANK_BEHAVIOR PASS\n");
    return 0;
}
