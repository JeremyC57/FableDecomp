#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
// 0040cec0 CUserProfileManager::CUserProfileManager
// Leaf: ??0CUserProfileManager@@QAE@XZ
// Flags: none
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_RETAIL_C2DEXTENTSI_ORIGINAL_CTOR_BODIES_PASS22
#define FABLE_RETAIL_PROFILE_DEFAULT_NAME_PASS26
#define FABLE_RETAIL_CUSERPROFILEMANAGER_CTOR_PASS26
#define FABLE_RETAIL_CDISPLAYMANAGER_ENUMERATE_MODES_PASS26
#define FABLE_RETAIL_CUSERPROFILEMANAGER_PASS22_STATE
#define FABLE_RETAIL_CUSERPROFILEMANAGER_RENDERUIBACKGROUND
#define FABLE_RETAIL_CARRAY
#include "engine/CArray.h"
#include "engine/CDisplayModeDesc.h"
#define FABLE_RETAIL_CUSERPROFILEMANAGER_PASS22_DEFAULTS
#define FABLE_RETAIL_CCONFIGOPTIONSDEFAULTSDEF_PASS22
#define FABLE_RETAIL_STLP_QUEUE
#define FABLE_RETAIL_STLP_DEQUE_PASS22_POP
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER_VIDEODEFAULTS
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_VIDEODEFAULTS
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_COLOURDEPTH
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER
#include "fable_string.h"

// Inline-budget: the SaveGameNames constructor expands all fifty wide-string copies.
namespace std {
template<> FABLE_FORCEINLINE CWideString* FABLE_FASTCALL __uninitialized_fill_n(CWideString* first, unsigned int n, const CWideString& x, const __false_type&) {
    for (; n > 0; --n, ++first) _Construct(first,x);
    return first;
}
}

#include "engine/CMainGameComponent.h"
#include "fable_user_profile.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CStartupDefinitionManager.h"
#include "engine/CSystemManager.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/CNewFrontendGameComponent.h"
#include <stdlib.h>
#ifdef _MSC_VER
#pragma intrinsic(abs)                       // retail inlines abs in the refresh pick at 0x0040967A..0x004096C8
#endif

#include "engine/CConfigOptionsDefaultsDef.h"

CDefPointer<const CConfigOptionsDefaultsDef> CUserProfileManager::GetConfigOptionsDefaultsDef() const
{
    CDefPointer<const CConfigOptionsDefaultsDef> result;
    if (CMainGameComponent::Get())
    {
        long index;
        {
            CCharString name("CONFIG_OPTIONS_DEFAULTS_DEF", -1);
            index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
        }
        CGameDefinitionManager::Get().CDefinitionManager::GetDef(index, &result);
    }
    else if (CNewFrontendGameComponent::Get())
    {
        long index;
        {
            CCharString name("CONFIG_OPTIONS_DEFAULTS_DEF", -1);
            index = CStartupDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
        }
        CStartupDefinitionManager::Get().GetDef(index, &result);
    }
    return result;
}
inline CWideString CUserProfileManager::GetDefaultProfileName() const
{
    return CWideString(L"Default");
}

CUserProfileManager::CUserProfileManager()
    : UseNewProfileValues(false), SoundsMuted(false), ApplyVideoValuesOnLoad(true), DisplayDialogOnOptionsChanged(true),
      SaveGameNames(50), DisplayTutorialsDuringCutscenes(false), VideoChangesAccepted(true)
{
    CArray<CDisplayModeDesc> modes;
    CDefPointer<const CConfigOptionsDefaultsDef> pdefaults = GetConfigOptionsDefaultsDef();
    C2DExtentsI min_size(1024, 768);
    if (!pdefaults.IsNull())
        min_size.Initialise(pdefaults->MinResolutionWidth, pdefaults->MinResolutionHeight);
    GFGetSystemManager()->GetDisplayManager()->EnumerateDisplayModes(modes, min_size, C2DExtentsI(0x7fff, 0x7fff));
    CDisplayModeDesc previous_mode = *modes.begin();
    // FableWin 0187A03B retains the original for-scope compatibility guard.
    if (false) {} else for (CArray<CDisplayModeDesc>::const_iterator it = modes.begin(); it != modes.end(); ++it) {
        if (it->Dimensions.X != previous_mode.Dimensions.X || it->Dimensions.Y != previous_mode.Dimensions.Y || it->BitsPerPixel != previous_mode.BitsPerPixel) {
            DisplayModes.push_back(previous_mode);
            previous_mode = *it;
        } else {
            previous_mode = *it;
        }
    }
    if (!DisplayModes.empty()) {
        if (DisplayModes.back().Dimensions.X != previous_mode.Dimensions.X || DisplayModes.back().Dimensions.Y != previous_mode.Dimensions.Y || DisplayModes.back().BitsPerPixel != previous_mode.BitsPerPixel)
            DisplayModes.push_back(previous_mode);
    }
    SetDefaultValues();
    SetDefaultValuesForNewProfile();
    ProfileName = GetDefaultProfileName();
    PBackBufferTexture = 0;
    GFGetSystemManager()->GetDisplayManager()->AddDeviceResetCallback(this);
}