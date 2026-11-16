// CUserProfileManager::LoadGameState(CAFile& file) -- read the profile text (a "FablePro" header: version, total length, then the text; or a bare legacy text file), restore every option with its default, re-clamp the video options against the config detection and the config-options def, reload the tutorial list and the save-slot names (a slot keeps its name only when it resolves to a path), then apply
// Leaf: ?LoadGameState@CUserProfileManager@@IAEXAAVCAFile@@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040BCA0, 2790 bytes, thiscall, `ret 4`, /GS cookie (the 8-byte signature buffer).
// Callees: CAFile::Read / SetPosition (vtable slots 3 / 5), CFileDataInputStream(CAFile&, unsigned long) 0x00994700 and
//   ~CFileDataInputStream 0x00994780, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inline Read),
//   CDataInputStream::ReadString 0x00996BE0, CCharString::LoadFromFile 0x0099F2E0, CStringParser(const CCharString&, ...)
//   0x00404720 (its implicit dtor inline: the STLport list free loop, ~CCharString x2, ~CBaseClass 0x0099A2E0),
//   CPersistContext(CStringParser&, ...) 0x009BADD0, the CPersistContext::Transfer<T> instances bool 0x004045C0 / float
//   0x00410620 / unsigned long 0x004106F0 / CCharString 0x004109A0 / CWideString 0x00410AC0 / long 0x00410BE0,
//   TransferVector<CActionInputControl> 0x00412DB0 / <ETutorialCategory> 0x00412E80, std::__copy<CActionInputControl*>
//   0x0040E2D0 (vector::clear), CUserProfileManager::ClampResolution 0x00406D90 / ClampAntialiasing 0x00409140 /
//   GetConfigOptionsDefaultsDef 0x004083E0 / ClampVideoOption 0x004068D0 / GetPathNameFromFileName 0x00408120 /
//   ApplyValues 0x00407370 / ApplyVideoValues 0x0040A7F0, vector<CWideString>::erase 0x00412450 / _M_fill_insert
//   0x004126E0, GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690, the CCharString /
//   CWideString ctors, dtors, comparisons and ToWideString 0x0099E480.
// Shape: the defaults are literal third arguments (one `const T&` temporary slot per type) except the strings, which go
//   through the inline two-argument Transfer + GetDefaultVal<T>() (ctor, copy, dtor of the default). ClampRefreshRate is
//   expanded (its TU body, the vacuous `index >= 0` keeps the down-walk unrotated, as in SetDefaultValues); ClampResolution
//   and GetConfigOptionsDefaultsDef stay calls, but the TU sees GetConfigOptionsDefaultsDef's body: VC7.1 then knows the
//   by-value CDefPointer result does not escape, keeps the def in esi across the ClampVideoOption calls and drops the dead
//   `Object = 0` of the final release (declared-only: reloads from the frame, different slot). The body is verbatim the
//   same-TU copy the landed 0x004093E0 SetDefaultValuesVideo carries (0x004083E0 on shared headers); its GetDef<T>(long) is the shared
//   FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY. The header reads expand the shared FABLE_FORCEINLINE ReadULONG ->
//   ReadType<unsigned long> -> Read chain (retail has the fully inline reads). The STLport list dtor of the parser is inline
//   (free loop + head free).
// Layout: fable_user_profile.h with the typed containers (..._VIDEODEFAULTS + ..._PASS22_DEFAULTS + ..._PASS22_STATE) and the
//   LoadGameState declaration (..._LOADGAMESTATE); engine/CConfigOptionsDefaultsDef.h (PASS22 model).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CCONFIGOPTIONSDEFAULTSDEF_PASS22
#define FABLE_RETAIL_CUSERPROFILEMANAGER_VIDEODEFAULTS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_PASS22_DEFAULTS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_PASS22_STATE
#define FABLE_RETAIL_CUSERPROFILEMANAGER_LOADGAMESTATE
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_READSTRING
#define FABLE_RETAIL_STLP_QUEUE
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CAFILE
#include "fable_string.h"
#include "engine/CMainGameComponent.h"
#include "fable_user_profile.h"
#include "engine/CConfigOptionsDefaultsDef.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CStartupDefinitionManager.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CAFile.h"
#include "engine/CFileDataInputStream.h"
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CNewFrontendGameComponent.h"
#include <string.h>
extern long g_MaxDetailA;                                            // 0x0137507C config-detection cap: max effects detail (-1 = none)
extern long g_MaxDetailD;                                            // 0x01375074 max texture detail
extern long g_MaxDetailB;                                            // 0x01375078 max shadow detail
extern long g_MaxDetailC;                                            // 0x01375070 max mesh detail
extern long g_MaxAntialiasing;                                       // 0x01375080
extern long g_MaxResolutionWidth;                                    // 0x01375084
extern long g_MaxRefreshRate;                                        // 0x01375088
extern const long g_refreshRateOptions[4];                           // 0x0122DE30

CCharString FABLE_FASTCALL GFIntToCharString(long value);           // 0x0099F830 ?GFIntToCharString@@YI?AVCCharString@@J@Z

// The inline two-argument Transfer and GetDefaultVal<T>() (declared in engine/CPersistContext.h; FableWin.pdb
// ??$Transfer@VCCharString@@@CPersistContext@@QAEXPBDAAVCCharString@@@Z / ??$GetDefaultVal@VCCharString@@@CPersistContext@@KI?AVCCharString@@XZ, no retail copy).
template<class T> inline void CPersistContext::Transfer(const char* name, T& value) { Transfer(name, value, GetDefaultVal<T>()); }
template<class T> inline T FABLE_FASTCALL CPersistContext::GetDefaultVal() { T value = T(); return value; }

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

void CUserProfileManager::LoadGameState(CAFile& file)
{
    CCharString text;
    char signature[8];
    file.Read(signature, 8, 0);
    if (memcmp(signature, "FablePro", 8) == 0) {
        CFileDataInputStream stream(file, 0x4000);
        stream.ReadULONG();
        unsigned long length = stream.ReadULONG();
        text = stream.ReadString(length - stream.GetPosition());
    } else {
        file.SetPosition(0);
        text.LoadFromFile(file, 0, -1);
    }
    CStringParser parser(text, 0, CCharString("", -1), 0);
    CPersistContext context(parser, 0, true, 0, 0, 0);

    CCharString profileName;
    context.Transfer("ProfileName", profileName);
    if (profileName != "")
        ProfileName = profileName.ToWideString();
    context.Transfer("SubtitlingEnabled", SubtitlingEnabled, true);
    context.Transfer("VibrationEnabled", VibrationEnabled, true);
    context.Transfer("CameraXYInverted", CameraXYInverted, false);
    context.Transfer("CameraYZInverted", CameraYZInverted, false);
    context.Transfer("CameraFPBowXYInverted", CameraFPBowXYInverted, false);
    context.Transfer("CameraFPBowYZInverted", CameraFPBowYZInverted, false);
    context.Transfer("CameraPCBowXYInverted", CameraPCBowXYInverted, false);
    context.Transfer("CameraPCBowYZInverted", CameraPCBowYZInverted, false);
    context.Transfer("CameraPCViewHeroXYInverted", CameraPCViewHeroXYInverted, false);
    context.Transfer("CameraPCViewHeroYZInverted", CameraPCViewHeroYZInverted, false);
    context.Transfer("TutorialSystemEnabled", TutorialSystemEnabled, true);
    context.Transfer("GuildMasterEnabled", GuildMasterEnabled, true);
    context.Transfer("HUDTooltipEnabled", HUDTooltipEnabled, true);
    context.Transfer("Gamma", Gamma, 1.0f);
    context.Transfer("HUDAlpha", HUDAlpha, 1.0f);
    context.Transfer("SoundVolume", SoundVolume, 0.8f);
    context.Transfer("MusicVolume", MusicVolume, 0.6f);
    context.Transfer("DialogueVolume", DialogueVolume, 0.9f);
    context.Transfer("UsingHeadPhones", UsingHeadPhones, false);
    context.Transfer("ShowingBuddyNames", ShowingBuddyNames, false);
    context.Transfer("CameraSensitivity", CameraSensitivity, 0.5f);
    context.Transfer("CameraResetting", CameraResetting, true);
    context.Transfer("Antialiasing", Antialiasing, 0UL);
    context.Transfer("TextureDetail", TextureDetail, 0.0f);
    context.Transfer("ShadowDetail", ShadowDetail, 0.0f);
    context.Transfer("MeshDetail", MeshDetail, 0.0f);
    context.Transfer("EffectsDetail", EffectsDetail, 0.0f);
    context.Transfer("Resolution", Resolution, 0UL);
    context.Transfer("RefreshRate", RefreshRate, 0UL);
    context.Transfer("ScreenAspectRatio", ScreenAspectRatio, 0UL);
    context.Transfer("VerticalSyncEnabled", VerticalSyncEnabled, true);
    AssignedInputsPassive.clear();
    AssignedInputsAggressive.clear();
    context.TransferVector("AssignedInputsPassive", AssignedInputsPassive);
    context.TransferVector("AssignedInputsAggressive", AssignedInputsAggressive);
    context.Transfer("ShowTargetingStatus", ShowTargetingStatus, true);
    context.Transfer("ScoreboardUserName", ScoreBoardUserName);
    context.Transfer("ScoreboardPassword", ScoreBoardPassword);
    context.Transfer("ScoreboardProxyIp", ScoreBoardProxyIp);
    context.Transfer("ScoreboardProxyPort", ScoreBoardProxyPort, 0L);
    context.Transfer("ScoreboardServerPort", ScoreBoardServerPort, 0L);

    ClampResolution(Resolution, g_MaxResolutionWidth);
    ClampRefreshRate(RefreshRate, g_MaxRefreshRate);
    ClampAntialiasing(Antialiasing, g_MaxAntialiasing);
    CDefPointer<const CConfigOptionsDefaultsDef> def = GetConfigOptionsDefaultsDef();
    if (def.IsNull()) {
        ClampVideoOption(EffectsDetail, -1.0f, -1.0f, (float)g_MaxDetailA, -1.0f);
        ClampVideoOption(TextureDetail, -1.0f, -1.0f, (float)g_MaxDetailD, -1.0f);
        ClampVideoOption(MeshDetail, -1.0f, -1.0f, (float)g_MaxDetailC, -1.0f);
        ClampVideoOption(ShadowDetail, -1.0f, -1.0f, (float)g_MaxDetailB, -1.0f);
    } else {
        ClampVideoOption(EffectsDetail, -1.0f, -1.0f, (float)g_MaxDetailA, def->MaxEffectsDetail);
        ClampVideoOption(TextureDetail, -1.0f, -1.0f, (float)g_MaxDetailD, def->MaxTextureDetail);
        ClampVideoOption(MeshDetail, -1.0f, -1.0f, (float)g_MaxDetailC, def->MaxMeshDetail);
        ClampVideoOption(ShadowDetail, -1.0f, -1.0f, (float)g_MaxDetailB, def->MaxShadowDetail);
    }

    context.TransferVector("GivenTutorialCategories", GivenTutorialCategories);
    SaveGameNames.clear();
    SaveGameNames.resize(50, L"");
    for (long i = 0; i < 50; ++i) {
        CCharString saveName;
        context.Transfer("SaveGameNames" + GFIntToCharString(i + 1), saveName);
        if (saveName != "") {
            CWideString path = GetPathNameFromFileName(saveName.ToWideString(), false);
            if (path != L"")
                SaveGameNames[i] = saveName.ToWideString();
        }
    }
    ApplyValues();
    if (ApplyVideoValuesOnLoad)
        ApplyVideoValues();
}