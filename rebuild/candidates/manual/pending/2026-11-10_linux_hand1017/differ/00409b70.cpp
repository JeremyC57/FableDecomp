// CUserProfileManager::SetDefaultValuesForNewProfile() -- fill the NewProfile copies of the options with their defaults: gammas, HUD alpha, the gameplay toggles and camera inversions, the audio volumes, then the video choices (config-detection / config-options-def detail, antialiasing, resolution and refresh), aspect ratio, vsync, the score-board ports and the WASD flag
// Grade: DIFFER(1089v1105) -- the prior (pending/2026-11-03_linux_hand733) re-graded unchanged with ITS header overlay (that overlay rewrites fable_user_profile.h and 18 engine headers outside any #ifdef, so it cannot be graded on the committed headers: SRC_FAIL ClampVideoOption / DisplayModes); residue unchanged: retail defers the non-null-def arm (0x00409E20..0x00409ECD) after the ClampResolution loop. 2 grades (1 SRC_FAIL on committed headers, 1 DIFFER with the prior overlay).
// Leaf: ?SetDefaultValuesForNewProfile@CUserProfileManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00409B70, 1105 bytes, thiscall, plain `ret`.
// Callees and shape: as SetDefaultValues 0x0040C790, on the NewProfile members (GetConfigOptionsDefaultsDef 0x004083E0,
//   ClampVideoOption 0x004068D0, ClampAntialiasing 0x00409140, the display-manager reads, the expanded ClampResolution /
//   ClampRefreshRate TU bodies).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CTEXTURE
#include "fable_string.h"
#include "fable_game.h"
#include "fable_user_profile.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CStartupDefinitionManager.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
class CNewFrontendGameComponent {
private:
    static CNewFrontendGameComponent* m_pNewFrontendGameComponent;   // 0x013B871C (bound spelling)
public:
    static CNewFrontendGameComponent* __fastcall Get() { return m_pNewFrontendGameComponent; }   // FableWin ?Get@CNewFrontendGameComponent@@SIPAV1@XZ (inline)
};

class CConfigOptionsDefaultsDef : public CParentDefClassBase {      // Ego_r.pdb members at retail offsets (Ego_r - 4), sizeof 0x74
public:
    unsigned long Antialiasing;                                      // +0x3c
    long          ResolutionWidth;                                   // +0x40
    long          ResolutionHeight;                                  // +0x44
    long          BitDepth;                                          // +0x48
    long          MinResolutionWidth;                                // +0x4c
    long          MinResolutionHeight;                               // +0x50
    float         TextureDetail;                                     // +0x54
    float         MaxTextureDetail;                                  // +0x58
    float         ShadowDetail;                                      // +0x5c
    float         MaxShadowDetail;                                   // +0x60
    float         MeshDetail;                                        // +0x64
    float         MaxMeshDetail;                                     // +0x68
    float         EffectsDetail;                                     // +0x6c
    float         MaxEffectsDetail;                                  // +0x70
};

template<class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* result) const   // 0x00410890 for T = CConfigOptionsDefaultsDef
{
    if (index <= 0) return false;
    CDefPointer<const CDefClassBase> definition = GetPDefFromGlobalIndex(index);
    if (!definition.IsNull()) { result->SetDynamicCast(definition); return true; }
    return false;
}

extern long g_MaxDetailA;                                            // 0x0137507C config-detection cap: max effects detail (-1 = none)
extern long g_MaxDetailD;                                            // 0x01375074 max texture detail
extern long g_MaxDetailB;                                            // 0x01375078 max shadow detail
extern long g_MaxDetailC;                                            // 0x01375070 max mesh detail
extern long g_DetailA;                                               // 0x01375098 config-detection effects detail (-1 = none)
extern long g_DetailB;                                               // 0x01375094 shadow detail
extern long g_DetailC;                                               // 0x0137508C mesh detail
extern long g_DetailD;                                               // 0x01375090 texture detail
extern long g_MaxAntialiasing;                                       // 0x01375080
extern unsigned long g_MaxResolutionWidth;                           // 0x01375084
extern unsigned long g_MaxRefreshRate;                               // 0x01375088
extern unsigned long g_MemoryA;                                      // 0x0137509C config-detection resolution width (established spelling)
extern unsigned long g_MemoryB;                                      // 0x013750A0 config-detection resolution height (established spelling)
extern const long g_refreshRateOptions[4];                           // 0x0122DE30

CDefPointer<const CConfigOptionsDefaultsDef> CUserProfileManager::GetConfigOptionsDefaultsDef() const
{
    CDefPointer<const CConfigOptionsDefaultsDef> result;
    if (CMainGameComponent::Get())
    {
        long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("CONFIG_OPTIONS_DEFAULTS_DEF", 1);
        CGameDefinitionManager::Get().CDefinitionManager::GetDef(index, &result);
    }
    else if (CNewFrontendGameComponent::Get())
    {
        long index = CStartupDefinitionManager::Get().GetGlobalIndexFromClassIndex("CONFIG_OPTIONS_DEFAULTS_DEF", 1);
        CStartupDefinitionManager::Get().GetDef(index, &result);
    }
    return result;
}

__forceinline void ClampToLimit(float& out, float limit, float secondaryLimit)
{
    if (limit != -1.0f) {
        if (out > limit)
            out = limit;
    }
    else if (secondaryLimit != -1.0f) {
        if (out > secondaryLimit)
            out = secondaryLimit;
    }
    else
        out = 0.0f;
}

void CUserProfileManager::ClampVideoOption(float& out, float value, float fallback, float limit, float secondaryLimit)
{
    if (value != -1.0f)
        out = value;
    else if (fallback != -1.0f) {
        out = fallback;
        ClampToLimit(out, limit, secondaryLimit);
    }
    else
        ClampToLimit(out, limit, secondaryLimit);
}

void CUserProfileManager::ClampResolution(unsigned long& resolution, long maxWidth)
{
    if (resolution >= DisplayModes.size())
        resolution = 0;
    if (maxWidth != -1) {
        if (DisplayModes[resolution].Dimensions.X > maxWidth) {
            unsigned long index = resolution;
            while (index >= 0 && DisplayModes[index].Dimensions.X > maxWidth)
                --index;
            resolution = index;
        }
    }
}

void CUserProfileManager::ClampRefreshRate(unsigned long& refreshRate, long maxRefreshRate)
{
    if (refreshRate >= 4)
        refreshRate = 0;
    if (maxRefreshRate != -1) {
        if (g_refreshRateOptions[refreshRate] > maxRefreshRate) {
            unsigned long index = refreshRate;
            while (index >= 0 && g_refreshRateOptions[index] > maxRefreshRate)
                --index;
            refreshRate = index;
        }
    }
}

void CUserProfileManager::SetDefaultValuesForNewProfile()
{
    GammaNewProfile = 1.0f;
    HUDAlphaNewProfile = 1.0f;
    SubtitlingEnabledNewProfile = true;
    VibrationEnabledNewProfile = true;
    CameraXYInvertedNewProfile = false;
    CameraYZInvertedNewProfile = false;
    CameraFPBowXYInvertedNewProfile = false;
    CameraFPBowYZInvertedNewProfile = false;
    CameraPCBowXYInvertedNewProfile = false;
    CameraPCBowYZInvertedNewProfile = false;
    CameraPCViewHeroXYInvertedNewProfile = false;
    CameraPCViewHeroYZInvertedNewProfile = false;
    TutorialSystemEnabledNewProfile = true;
    GuildMasterEnabledNewProfile = true;
    HUDTooltipEnabledNewProfile = true;
    SoundVolumeNewProfile = 0.8f;
    MusicVolumeNewProfile = 0.6f;
    DialogueVolumeNewProfile = 0.9f;
    ShowingBuddyNamesNewProfile = false;
    UsingHeadPhonesNewProfile = false;
    CameraSensitivityNewProfile = 0.5f;
    CameraResettingNewProfile = true;

    CDefPointer<const CConfigOptionsDefaultsDef> def = GetConfigOptionsDefaultsDef();
    if (def.IsNull()) {
        ClampVideoOption(EffectsDetailNewProfile, (float)g_DetailA, -1.0f, (float)g_MaxDetailA, -1.0f);
        ClampVideoOption(TextureDetailNewProfile, (float)g_DetailD, -1.0f, (float)g_MaxDetailD, -1.0f);
        ClampVideoOption(MeshDetailNewProfile, (float)g_DetailC, -1.0f, (float)g_MaxDetailC, -1.0f);
        ClampVideoOption(ShadowDetailNewProfile, (float)g_DetailB, -1.0f, (float)g_MaxDetailB, -1.0f);
        AntialiasingNewProfile = 0;
    } else {
        ClampVideoOption(EffectsDetailNewProfile, (float)g_DetailA, def->EffectsDetail, (float)g_MaxDetailA, def->MaxEffectsDetail);
        ClampVideoOption(TextureDetailNewProfile, (float)g_DetailD, def->TextureDetail, (float)g_MaxDetailD, def->MaxTextureDetail);
        ClampVideoOption(MeshDetailNewProfile, (float)g_DetailC, def->MeshDetail, (float)g_MaxDetailC, def->MaxMeshDetail);
        ClampVideoOption(ShadowDetailNewProfile, (float)g_DetailB, def->ShadowDetail, (float)g_MaxDetailB, def->MaxShadowDetail);
        AntialiasingNewProfile = def->Antialiasing;
    }
    ClampAntialiasing(AntialiasingNewProfile, g_MaxAntialiasing);

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
    ResolutionNewProfile = 0;
    for (std::vector<CDisplayModeDesc>::iterator it = DisplayModes.begin(); it != DisplayModes.end(); ++it) {
        if (it->Dimensions.X == width && it->Dimensions.Y == height && it->BitsPerPixel == bitDepth)
            break;
        ++ResolutionNewProfile;
    }
    ClampResolution(ResolutionNewProfile, g_MaxResolutionWidth);

    long refreshRate = GFGetSystemManager()->GetDisplayManager()->GetRefreshRate();
    RefreshRateNewProfile = 0;
    for (unsigned long i = 1; i < 4; ++i) {
        if (i >= 0 && abs(refreshRate - g_refreshRateOptions[i]) < abs(refreshRate - g_refreshRateOptions[RefreshRateNewProfile]) && g_refreshRateOptions[i] <= refreshRate)
            RefreshRateNewProfile = i;
    }
    ClampRefreshRate(RefreshRateNewProfile, g_MaxRefreshRate);

    ScreenAspectRatioNewProfile = 0;
    VerticalSyncEnabledNewProfile = true;
    ScoreBoardServerPort = 80;
    ScoreBoardProxyPort = 0;
    NewProfileToUseWASD = true;
    ShowTargetingStatusNewProfile = true;
}
