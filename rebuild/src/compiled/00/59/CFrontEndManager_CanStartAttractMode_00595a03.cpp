// NUISystem::CFrontEndManager::CanStartAttractMode() -- false on the PC build. Ego_r.pdb ?CanStartAttractMode@CFrontEndManager@NUISystem@@QAE_NXZ (the header RUN opt-in names 0x00595A03).
// Leaf: ?CanStartAttractMode@CFrontEndManager@NUISystem@@QAE_NXZ
// NF37 (2026-11-16): moved off its stand-in onto the shared engine/CFrontEndManager.h; include chain = the preamble of the landed 00/42/CNewFrontendGameComponent_Run_0042ec7c.cpp.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR_FORCEINLINE_COPY
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_FORCEMUSICPLAY
#define FABLE_RETAIL_CFRONTENDMANAGER
#define FABLE_RETAIL_CFRONTENDMANAGER_RUN
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN
#define FABLE_RETAIL_CTBASESINGLETON_RELEASE
#define FABLE_RETAIL_CFONTMANAGER_ASYNC
#define FABLE_RETAIL_CASOUNDMANAGER
#define FABLE_RETAIL_CINPUTMANAGER_CLEAREVENTS
#define FABLE_RETAIL_CBANKFILEMANAGER
#define FABLE_RETAIL_CBANKFILEMANAGER_HEADERPATHS
#define FABLE_RETAIL_CBANKFILEMANAGER_CLOSERETAILBANK
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_RESETDIMENSIONS
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#include "fable_string.h"
#include "fable_profile.h"
#include "fable_retail_banks.h"
#include "engine/CRGBColour.h"
#include "engine/CCountedPointer.h"
#include "engine/CSystemManager.h"
#include "engine/CRenderManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CFontBank.h"
#include "engine/CFontManager.h"
#include "engine/CInputManager.h"
#include "engine/CASoundManager.h"
#include "engine/CFrontEndManager.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/CTBaseSingleton.h"
#include "engine/CFrontEndDef.h"
#include "engine/CMovieInfo.h"
#include "engine/CXMVPlayer.h"
#include "engine/CXMVPlayerBuffers.h"
#include "engine/CEngineInterpolationInfo.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CMainGameComponent.h"
#include "engine/CFrontendGameComponent.h"
#include "engine/CNewFrontendGameComponent.h"
#include <stdlib.h>

bool NUISystem::CFrontEndManager::CanStartAttractMode()
{
    return false;
}
