// CUserProfileManager::ApplyVideoValues() -- hand the current (or new-profile) video options to the 3D engine: aspect ratio, the four detail qualities, the chosen display mode and the nearest refresh rate the adapter offers for it, the antialiasing mode and vertical sync; when the engine rejects live values, reset them to the defaults, apply and save those, and tell an open in-game menu
// Grade: DIFFER(1925v1941) -- frame 0x7C v 0x78 (one extra 4-byte local once C2DExtentsI has its user copy ctor) shifts every slot by 4; retail keeps the def in ebx and the chosen refresh rate in the frame ([esp+14h]), ours def in ebp and the rate in ebx (dl=224).
// Leaf: ?ApplyVideoValues@CUserProfileManager@@IAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040A7F0, 1941 bytes, thiscall, plain `ret`.
// Callees: ConvertFloatToVideoOptionQuality 0x00406820 (same-TU body: VC7.1 then folds the four Convert(0.0f) of the reset path
//   into one call), GetConfigOptionsDefaultsDef 0x004083E0 (same-TU body: the def stays in a register), GFGetSystemManager
//   0x009A4EC0 -> CDisplayManager::EnumerateDisplayModes 0x009C02D0 (by-value C2DExtentsI min / max, built in their argument
//   slots) / EnumerateAntiAliasingModes 0x009C05C0 / GetRenderTargetDimensions 0x009BEDC0 (the inline
//   NUISystem::CUIScreenUnitConverter::ResetDimensions), CIEngine::SetVideoOptions (vtable +0x38, through
//   CMainGameComponent::GetDisplayEngine().Get3DEngine()), the const CPlayerManager::GetMainPlayer 0x00449960 (via
//   PeekPlayerManager), CPlayer::GetPPlayerCharacterThing 0x00487B70, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020
//   (the inline CThing::GetTC<CTCInGameMenu>), CTCInGameMenu::VideoOptionsNotAccepted 0x0047F30F / VideoOptionsChanged 0x0047E651,
//   CUserProfileManager::Save 0x0040A740, free 0x00BFEA14 (the CArray buffers).
// Shape: three written-out fills of the CEngineVideoOptions block (new-profile values; live values; the reset defaults when the
//   engine rejects the live set), each with its own display-mode / def / antialiasing-mode locals; FableWin.pdb lists the inline
//   helpers of this function (0x018844D0..0x01884760): CEngineVideoOptions(), CDisplayEngine::Get3DEngine,
//   CMainGameComponent::PeekPlayerManager, C2DExtentsF(float,float) / Initialise, CTCInGameMenu::IsOpen,
//   CUIScreenUnitConverter::ResetDimensions. The aspect switch has an explicit `case 0` (retail's `sub eax,0 ; je`); the display
//   mode is copied (`CDisplayModeDesc mode = *it`: X, Y and the rate load before the compares).
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
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C2DEXTENTSI
#define FABLE_RETAIL_C2DEXTENTSF
#include "fable_string.h"
#include "fable_game.h"
#include "fable_user_profile.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CStartupDefinitionManager.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CArray.h"
#include "engine/CIEngine.h"
#include "engine/CDisplayEngine.h"
#include "engine/CEngineVideoOptions.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThing.h"
#include "engine/CTCInGameMenu.h"
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

EEngineVideoOptionQuality CUserProfileManager::ConvertFloatToVideoOptionQuality(float value)   // landed 0x00406820 (same-TU body)
{
    EEngineVideoOptionQuality quality = (EEngineVideoOptionQuality)0;
    if (value >= 0.0f)
        quality = (EEngineVideoOptionQuality)0;
    if (value >= 1.0f)
        quality = (EEngineVideoOptionQuality)1;
    if (value >= 2.0f)
        quality = (EEngineVideoOptionQuality)2;
    if (value >= 3.0f)
        quality = (EEngineVideoOptionQuality)3;
    return quality;
}

void CUserProfileManager::ApplyVideoValues()
{
    CEngineVideoOptions options;
    long refreshRate;
    if (UseNewProfileValues) {
        switch (ScreenAspectRatioNewProfile) {
        case 0: options.ScreenAspectRatio.Initialise(0.0f, 0.0f); break;
        case 1: options.ScreenAspectRatio.Initialise(4.0f, 3.0f); break;
        case 2: options.ScreenAspectRatio.Initialise(16.0f, 9.0f); break;
        default: options.ScreenAspectRatio.Initialise(0.0f, 0.0f); break;
        }
        options.MeshDetail = ConvertFloatToVideoOptionQuality(MeshDetailNewProfile);
        options.TextureDetail = ConvertFloatToVideoOptionQuality(TextureDetailNewProfile);
        options.ShadowQuality = ConvertFloatToVideoOptionQuality(ShadowDetailNewProfile);
        options.EffectsQuality = ConvertFloatToVideoOptionQuality(EffectsDetailNewProfile);
        options.BitsPerPixel = DisplayModes[ResolutionNewProfile].BitsPerPixel;
        options.ScreenResolution = DisplayModes[ResolutionNewProfile].Dimensions;
        CArray<CDisplayModeDesc> displayModes;
        CDefPointer<const CConfigOptionsDefaultsDef> def = GetConfigOptionsDefaultsDef();
        long minWidth = 1024;
        long minHeight = 768;
        if (!def.IsNull()) {
            minWidth = def->MinResolutionWidth;
            minHeight = def->MinResolutionHeight;
        }
        GFGetSystemManager()->GetDisplayManager()->EnumerateDisplayModes(displayModes, C2DExtentsI(minWidth, minHeight), C2DExtentsI(0x7fff, 0x7fff));
        refreshRate = DisplayModes[ResolutionNewProfile].RefreshRate;
        for (CArray<CDisplayModeDesc>::iterator it = displayModes.begin(); it != displayModes.end(); ++it) {
            CDisplayModeDesc mode = *it;
            if (mode.Dimensions.X == options.ScreenResolution.X && mode.Dimensions.Y == options.ScreenResolution.Y && mode.BitsPerPixel == options.BitsPerPixel) {
                if (abs(g_refreshRateOptions[RefreshRateNewProfile] - mode.RefreshRate) < abs(g_refreshRateOptions[RefreshRateNewProfile] - refreshRate))
                    refreshRate = mode.RefreshRate;
            }
        }
        options.RefreshRate = refreshRate;
        CArray<ESurfaceMultisampleType> antialiasingModes;
        GFGetSystemManager()->GetDisplayManager()->EnumerateAntiAliasingModes(antialiasingModes);
        options.Antialiasing = antialiasingModes[AntialiasingNewProfile];
        options.VerticalSync = VerticalSyncEnabledNewProfile;
        if (CMainGameComponent::Get())
            CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetVideoOptions(options);
    } else {
        switch (ScreenAspectRatio) {
        case 0: options.ScreenAspectRatio.Initialise(0.0f, 0.0f); break;
        case 1: options.ScreenAspectRatio.Initialise(4.0f, 3.0f); break;
        case 2: options.ScreenAspectRatio.Initialise(16.0f, 9.0f); break;
        default: options.ScreenAspectRatio.Initialise(0.0f, 0.0f); break;
        }
        options.MeshDetail = ConvertFloatToVideoOptionQuality(MeshDetail);
        options.TextureDetail = ConvertFloatToVideoOptionQuality(TextureDetail);
        options.ShadowQuality = ConvertFloatToVideoOptionQuality(ShadowDetail);
        options.EffectsQuality = ConvertFloatToVideoOptionQuality(EffectsDetail);
        options.BitsPerPixel = DisplayModes[Resolution].BitsPerPixel;
        options.ScreenResolution = DisplayModes[Resolution].Dimensions;
        CArray<CDisplayModeDesc> displayModes;
        CDefPointer<const CConfigOptionsDefaultsDef> def = GetConfigOptionsDefaultsDef();
        long minWidth = 1024;
        long minHeight = 768;
        if (!def.IsNull()) {
            minWidth = def->MinResolutionWidth;
            minHeight = def->MinResolutionHeight;
        }
        GFGetSystemManager()->GetDisplayManager()->EnumerateDisplayModes(displayModes, C2DExtentsI(minWidth, minHeight), C2DExtentsI(0x7fff, 0x7fff));
        refreshRate = DisplayModes[Resolution].RefreshRate;
        for (CArray<CDisplayModeDesc>::iterator it = displayModes.begin(); it != displayModes.end(); ++it) {
            CDisplayModeDesc mode = *it;
            if (mode.Dimensions.X == options.ScreenResolution.X && mode.Dimensions.Y == options.ScreenResolution.Y && mode.BitsPerPixel == options.BitsPerPixel) {
                if (abs(g_refreshRateOptions[RefreshRate] - mode.RefreshRate) < abs(g_refreshRateOptions[RefreshRate] - refreshRate))
                    refreshRate = mode.RefreshRate;
            }
        }
        options.RefreshRate = refreshRate;
        CArray<ESurfaceMultisampleType> antialiasingModes;
        GFGetSystemManager()->GetDisplayManager()->EnumerateAntiAliasingModes(antialiasingModes);
        options.Antialiasing = antialiasingModes[Antialiasing];
        options.VerticalSync = VerticalSyncEnabled;
        if (CMainGameComponent::Get()) {
            if (!CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetVideoOptions(options)) {
                NUISystem::CUIScreenUnitConverter::ResetDimensions();
                VideoChangesAccepted = false;
                CThing* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPPlayerCharacterThing();
                if (hero && !hero->IsDead()) {
                    CTCInGameMenu* menu = 0;
                    hero->GetTC(&menu);
                    if (menu && menu->IsOpen()) {
                        menu->VideoOptionsNotAccepted();
                    } else {
                        ScreenAspectRatio = 0;
                        Resolution = 0;
                        RefreshRate = 0;
                        MeshDetail = 0.0f;
                        TextureDetail = 0.0f;
                        ShadowDetail = 0.0f;
                        EffectsDetail = 0.0f;
                        Antialiasing = 0;
                        VerticalSyncEnabled = true;
                        switch (ScreenAspectRatio) {
                        case 0: options.ScreenAspectRatio.Initialise(0.0f, 0.0f); break;
                        case 1: options.ScreenAspectRatio.Initialise(4.0f, 3.0f); break;
                        case 2: options.ScreenAspectRatio.Initialise(16.0f, 9.0f); break;
                        default: options.ScreenAspectRatio.Initialise(0.0f, 0.0f); break;
                        }
                        options.MeshDetail = ConvertFloatToVideoOptionQuality(MeshDetail);
                        options.TextureDetail = ConvertFloatToVideoOptionQuality(TextureDetail);
                        options.ShadowQuality = ConvertFloatToVideoOptionQuality(ShadowDetail);
                        options.EffectsQuality = ConvertFloatToVideoOptionQuality(EffectsDetail);
                        options.BitsPerPixel = DisplayModes[Resolution].BitsPerPixel;
                        options.ScreenResolution = DisplayModes[Resolution].Dimensions;
                        CArray<CDisplayModeDesc> displayModesReset;
                        CDefPointer<const CConfigOptionsDefaultsDef> defReset = GetConfigOptionsDefaultsDef();
                        long minWidthReset = 1024;
                        long minHeightReset = 768;
                        if (!defReset.IsNull()) {
                            minWidthReset = defReset->MinResolutionWidth;
                            minHeightReset = defReset->MinResolutionHeight;
                        }
                        GFGetSystemManager()->GetDisplayManager()->EnumerateDisplayModes(displayModesReset, C2DExtentsI(minWidthReset, minHeightReset), C2DExtentsI(0x7fff, 0x7fff));
                        refreshRate = DisplayModes[Resolution].RefreshRate;
                        for (CArray<CDisplayModeDesc>::iterator itReset = displayModesReset.begin(); itReset != displayModesReset.end(); ++itReset) {
                            CDisplayModeDesc modeReset = *itReset;
                            if (modeReset.Dimensions.X == options.ScreenResolution.X && modeReset.Dimensions.Y == options.ScreenResolution.Y && modeReset.BitsPerPixel == options.BitsPerPixel) {
                                if (abs(g_refreshRateOptions[RefreshRate] - modeReset.RefreshRate) < abs(g_refreshRateOptions[RefreshRate] - refreshRate))
                                    refreshRate = modeReset.RefreshRate;
                            }
                        }
                        options.RefreshRate = refreshRate;
                        CArray<ESurfaceMultisampleType> antialiasingModesReset;
                        GFGetSystemManager()->GetDisplayManager()->EnumerateAntiAliasingModes(antialiasingModesReset);
                        options.Antialiasing = antialiasingModesReset[Antialiasing];
                        options.VerticalSync = VerticalSyncEnabled;
                        CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetVideoOptions(options);
                        Save();
                    }
                }
            } else {
                NUISystem::CUIScreenUnitConverter::ResetDimensions();
                VideoChangesAccepted = true;
                if (DisplayDialogOnOptionsChanged) {
                    CThing* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPPlayerCharacterThing();
                    if (hero && !hero->IsDead()) {
                        CTCInGameMenu* menu = 0;
                        hero->GetTC(&menu);
                        if (menu && menu->IsOpen())
                            menu->VideoOptionsChanged();
                    }
                }
            }
        }
    }
}
