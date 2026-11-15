// CUserProfileManager::SetDefaultValues() -- reset all live profile settings
// Leaf: ?SetDefaultValues@CUserProfileManager@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x0040C790, 1284 bytes, thiscall, ret.
// Grade: RELOCATION_MATCH; guarded null-def antialiasing statement reproduces the retail arm layout.
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

extern long g_MaxDetailA;                                            // 0x0137507C
extern long g_MaxDetailB;                                            // 0x01375078
extern long g_MaxDetailC;                                            // 0x01375070
extern long g_MaxDetailD;                                            // 0x01375074
extern long g_DetailA;                                               // 0x01375098
extern long g_DetailB;                                               // 0x01375094
extern long g_DetailC;                                               // 0x0137508C
extern long g_DetailD;                                               // 0x01375090
extern long g_MaxAntialiasing;                                       // 0x01375080
extern long g_MaxResolutionWidth;                                    // 0x01375084
extern long g_MaxRefreshRate;                                        // 0x01375088
extern long g_MemoryA;                                               // 0x0137509C
extern long g_MemoryB;                                               // 0x013750A0
extern const long g_refreshRateOptions[4];                           // 0x0122DE30

// 0x004083E0, called at 0x004093F3 (same source file per Ego_r.pdb); graded RELOCATION_MATCH against 0x004083E0 (see header).
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



// Expanded at 0x00409548..0x0040954E. No symbol of this TU binds 0x00406880 (an ICF fold landed as CModeBlock::GetAnimFilter).
const std::vector<CDisplayModeDesc>& CUserProfileManager::GetDisplayModes() const
{
    return DisplayModes;
}

// 0x00406D90 out of line; expanded here at 0x0040957E..0x004095D2. Graded MATCH against 0x00406D90. Differs from the landed
// ClampResolution_00406d90 row (unsigned long*, made-up IsModeWiderThan, local CRetailVector): see header / NAME_FIXES.tsv.
void CUserProfileManager::ClampResolution(unsigned long& resolution, long maxWidth)
{
    if (resolution >= DisplayModes.size())
        resolution = 0;
    if (maxWidth != -1) {
        if (DisplayModes[resolution].Dimensions.X > maxWidth) {
            unsigned long index = resolution;
            // Unsigned guard (always true) keeps the loop unrotated as in retail: cmp ; jle ; dec ; jmp at 0x004095C6..0x004095D2.
            while (index >= 0 && DisplayModes[index].Dimensions.X > maxWidth)
                --index;
            resolution = index;
        }
    }
}

// 0x004069A0 out of line (no landed row); expanded here at 0x004096CA..0x004096FC. Graded RELOCATION_MATCH against 0x004069A0.
void CUserProfileManager::ClampRefreshRate(unsigned long& refreshRate, long maxRefreshRate)
{
    if (refreshRate >= 4)
        refreshRate = 0;
    if (maxRefreshRate != -1) {
        if (g_refreshRateOptions[refreshRate] > maxRefreshRate) {
            unsigned long index = refreshRate;
            // Unsigned guard (always true) keeps the loop unrotated as in retail: cmp ; jle ; dec ; jmp at 0x004096F0..0x004096FA.
            while (index >= 0 && g_refreshRateOptions[index] > maxRefreshRate)
                --index;
            refreshRate = index;
        }
    }
}

void CUserProfileManager::SetDefaultValues()
{
    Gamma = 1.0f;
    HUDAlpha = 1.0f;
    SubtitlingEnabled = true;
    VibrationEnabled = true;
    CameraXYInverted = false;
    CameraYZInverted = false;
    CameraFPBowXYInverted = false;
    CameraFPBowYZInverted = false;
    CameraPCBowXYInverted = false;
    CameraPCBowYZInverted = false;
    CameraPCViewHeroXYInverted = false;
    CameraPCViewHeroYZInverted = false;
    TutorialSystemEnabled = true;
    GuildMasterEnabled = true;
    HUDTooltipEnabled = true;
    SoundVolume = 0.8f;
    MusicVolume = 0.6f;
    DialogueVolume = 0.9f;
    ShowingBuddyNames = false;
    SoundsMuted = false;
    UsingHeadPhones = false;
    ShowTargetingStatus = true;
    CameraResetting = true;
    CameraSensitivity = 0.5f;

    CDefPointer<const CConfigOptionsDefaultsDef> def = GetConfigOptionsDefaultsDef();
    if (def.IsNull()) {
        ClampVideoOption(EffectsDetail, (float)g_DetailA, -1.0f, (float)g_MaxDetailA, -1.0f);
        ClampVideoOption(TextureDetail, (float)g_DetailD, -1.0f, (float)g_MaxDetailD, -1.0f);
        ClampVideoOption(MeshDetail, (float)g_DetailC, -1.0f, (float)g_MaxDetailC, -1.0f);
        ClampVideoOption(ShadowDetail, (float)g_DetailB, -1.0f, (float)g_MaxDetailB, -1.0f);
    } else {
        ClampVideoOption(EffectsDetail, (float)g_DetailA, def->EffectsDetail, (float)g_MaxDetailA, def->MaxEffectsDetail);
        ClampVideoOption(TextureDetail, (float)g_DetailD, def->TextureDetail, (float)g_MaxDetailD, def->MaxTextureDetail);
        ClampVideoOption(MeshDetail, (float)g_DetailC, def->MeshDetail, (float)g_MaxDetailC, def->MaxMeshDetail);
        ClampVideoOption(ShadowDetail, (float)g_DetailB, def->ShadowDetail, (float)g_MaxDetailB, def->MaxShadowDetail);
        Antialiasing = def->Antialiasing;
    }
    if (def.IsNull())
        Antialiasing = 0;
    ClampAntialiasing(Antialiasing, g_MaxAntialiasing);

    long width = -1;
    long height = -1;
    long bitDepth = GFGetSystemManager()->GetDisplayManager()->GetBitDepth();
    if ((long)g_MemoryA > 0)
        width = g_MemoryA;
    if ((long)g_MemoryB > 0)
        height = g_MemoryB;
    if (!def.IsNull()) {
        if (width == -1)
            width = def->ResolutionWidth;
        if (height == -1)
            height = def->ResolutionHeight;
        bitDepth = def->BitDepth;
    }
    if (width == -1)
        width = GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions().X;
    if (height == -1)
        height = GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions().Y;
    const std::vector<CDisplayModeDesc>& modes = GetDisplayModes();
    Resolution = 0;
    for (std::vector<CDisplayModeDesc>::const_iterator it = modes.begin(); it != modes.end(); ++it) {
        if (it->Dimensions.X == width && it->Dimensions.Y == height && it->BitsPerPixel == bitDepth)
            break;
        ++Resolution;
    }
    ClampResolution(Resolution, g_MaxResolutionWidth);

    long refreshRate = GFGetSystemManager()->GetDisplayManager()->GetRefreshRate();
    RefreshRate = 0;
    unsigned long i = 1;
    do {
        if (abs(refreshRate - g_refreshRateOptions[i]) < abs(refreshRate - g_refreshRateOptions[RefreshRate]) && g_refreshRateOptions[i] <= refreshRate)
            RefreshRate = i;
    } while (++i < 4);
    ClampRefreshRate(RefreshRate, g_MaxRefreshRate);

    ScreenAspectRatio = 0;
    VerticalSyncEnabled = true;
    ScoreBoardProxyPort = 0;
    ScoreBoardServerPort = 80;
    ResetAssignedInputs();
    GivenTutorialCategories.clear();
    while (!PendingTutorialCategories.empty())
        PendingTutorialCategories.pop();
    SaveGameNames.clear();
    SaveGameNames.resize(50, L"");
    ApplyValues();
    if (ApplyVideoValuesOnLoad)
        ApplyVideoValues();
    if (ProfileName != L"")
        Save();
}