// CUserProfileManager::SetDefaultValuesVideo() -- reset the video options to the config-options defaults def (or the config-detection overrides): gamma 1, the four detail levels through ClampVideoOption, antialiasing, the display mode matching the wanted width/height/depth, the closest refresh rate not above the display's, each clamped to the detection caps; then aspect 0, vsync on, ApplyValues
// Grade: RELOCATION_MATCH (841 bytes; also with LANDVERIFY_STRICT_HEADERS=1). Layout key: the null arm leaves Antialiasing alone and ONE guarded statement after the if-else, `if (def.IsNull()) Antialiasing = 0;`, is threaded into it by c2; the join then has a fall-through predecessor (the null arm) and the def arm is left behind the inlined ClampResolution loop, jumping back (retail 0x004095D4..0x00409672).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004093E0, length 841 bytes, thiscall, plain `ret`, no EH frame, no /GS.
// Identity: Ego_r.pdb 0x004073D0 + FableWin.pdb 0x01881E20 ?SetDefaultValuesVideo@CUserProfileManager@@QAEXXZ (public).
// Callees (called out of line): GetConfigOptionsDefaultsDef 0x004083E0 (call at 0x004093F3; body carried, see below),
//   CDefinitionManager::GetDef<CConfigOptionsDefaultsDef> 0x00410890 (called only from 0x004083E0; body from the shared header,
//   see below), ClampVideoOption 0x004068D0 (x4 per arm), ClampAntialiasing 0x00409140, GFGetSystemManager 0x009A4EC0 (x4),
//   CPixelFormat::GetColourDepth 0x009E3820 (via FableWin's inline CDisplayManager::GetBitDepth),
//   CDisplayManager::GetRenderTargetDimensions 0x009BEDC0 (x2, const C2DExtentsI by value), ApplyValues 0x00407370; the def's
//   Delete through vtable slot 1 on the last release.
// Expanded inline here (no call instruction to them in 0x004093E0): GetDisplayModes, ClampResolution, ClampRefreshRate.
// Carried bodies, each graded with THIS source against its own retail address (repair 3):
//   - GetConfigOptionsDefaultsDef 0x004083E0: NOT expanded (call at 0x004093F3); carried because the retail TU saw it: Ego_r.pdb
//     puts it in this source file at 0x00406C90, ahead of SetDefaultValuesVideo at 0x004073D0. This source grades
//     RELOCATION_MATCH against 0x004083E0. Landed row CUserProfileManager_GetConfigOptionsDefaultsDef_004083e0.cpp has the same
//     statements but differs in spelling: it tests made-up globals g_pMainGameComponent / g_pFrontendRuntime (void*) where this
//     row uses the PDB accessors CMainGameComponent::Get() / CNewFrontendGameComponent::Get() (statics PMainGameComponent
//     0x013B86A0 / m_pNewFrontendGameComponent 0x013B871C), and calls an unqualified non-template GetDef on local models where
//     this row calls the shared header's template GetDef<CConfigOptionsDefaultsDef> (NAME_FIXES.tsv).
//   - CDefinitionManager::GetDef<CConfigOptionsDefaultsDef> 0x00410890: a template, so its body is in its header in any TU that
//     instantiates it; retail's only call is 0x00408472 in GetConfigOptionsDefaultsDef (FableWin.pdb 0x01897320 has the symbol).
//     Out of line (shared header opt-in FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY, __declspec(noinline)) with the overlay
//     opt-in FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD. Without the visible body 0x004083E0 grades DIFFER(203v194) (the caller
//     then reloads the result handle after the return copy's AddRef); with it this source grades RELOCATION_MATCH against
//     0x00410890 and 0x004083E0. Landed row CDefinitionManager_GetDef00410890_00410890.cpp spells it as a made-up GetDef_00410890 on
//     local models (NAME_FIXES.tsv). 0x004093E0 itself matches with or without this body.
//   - GetDisplayModes (Ego_r.pdb 0x004054D0 + FableWin.pdb 0x01889A50 ?GetDisplayModes@CUserProfileManager@@QBEABV?$vector@
//     VCDisplayModeDesc@@...@std@@XZ): expanded at 0x00409548..0x0040954E (`mov eax,[esi+80h] ; cmp eax,[esi+84h]`). This row's
//     TU never references 0x00406880 and binds no symbol to it: the 7 bytes there (`lea eax,[ecx+80h] ; ret`) are an ICF fold
//     landed as NCreatureMode::CModeBlock::GetAnimFilter (CModeBlock_GetAnimFilter_00406880.cpp); the address belongs to whichever
//     function the linker kept, and no caller relocation shows GetDisplayModes there. The out-of-line copy compiled from this
//     source is byte-identical to those 7 bytes (fg MATCH), which proves the body only. No landed row of its own.
//   - ClampResolution 0x00406D90 (Ego_r.pdb 0x00405DA0 + FableWin.pdb 0x0188AB50 ?ClampResolution@CUserProfileManager@@AAEXAAKJ@Z):
//     no call to 0x00406D90 here; the range test and loop are inline at 0x0040957E..0x004095D2 (+ the store at 0x00409677). This
//     source grades MATCH against 0x00406D90. Landed row CUserProfileManager_ClampResolution_00406d90.cpp takes `unsigned long*`
//     (public) and uses a made-up FABLE_FORCEINLINE helper IsModeWiderThan on a local CRetailVector model; this row uses the PDB
//     `unsigned long&` parameter, the shared header's DisplayModes vector and `while (index >= 0 && ...)` (NAME_FIXES.tsv).
//   - ClampRefreshRate 0x004069A0 (Ego_r.pdb 0x004055E0 + FableWin.pdb 0x0188AC10 ?ClampRefreshRate@CUserProfileManager@@AAEXAAKJ@Z):
//     no call to 0x004069A0 here; inline at 0x004096CA..0x004096FC. This source grades RELOCATION_MATCH against the out-of-line
//     copy 0x004069A0 (52 bytes; no landed row).
// Data: caps 0x01375070..0x01375088, detection values 0x0137508C..0x013750A0 (GFConfigDetection 0x004017B0 spellings), refresh table 0x0122DE30 (= {60, 75, 85, 100} Hz in Fable.exe; extern here, values not modelled).
// Globals: descriptive names (no PDB symbol), GFConfigDetection 0x004017B0 spellings kept for one name per address; all `long`
//   (3JA): retail compares g_MemoryA/B signed here (`jle`), 0x01375084/88 signed in GetMaxResolution 0x00408900 / GetMaxRefreshRate
//   0x00406890; GFConfigDetection only stores them, so its `unsigned long` is the spelling to fix (NAME_FIXES.tsv).
// Shape notes: Antialiasing = 0 for a missing def is a guarded statement after the detail if-else (gives retail block layout); the
//   refresh pick is a do/while because retail keeps it rolled: `inc ecx ; cmp ecx,4 ; jb 0x409694` at 0x00409694..0x004096C8
//   (a `for (i = 1; i < 4; ++i)` is fully unrolled by VC7.1); the mode search walks a local const reference from
//   GetDisplayModes() (gives retail's `cmp eax,[esi+84h]` head); the clamp loops are unrotated in retail (`cmp ; jle ; dec ;
//   jmp` at 0x004095C6..0x004095D2 for ClampResolution and 0x004096F0..0x004096FA for ClampRefreshRate), which the folded
//   unsigned guard `while (index >= 0 && ...)` reproduces (Lionhead's defensive spelling; `index >= 0` is always true).
// Headers: overlay opt-ins FABLE_RETAIL_CUSERPROFILEMANAGER_VIDEODEFAULTS, FABLE_RETAIL_CDISPLAYMANAGER_VIDEODEFAULTS,
//   FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD (hdr/README.md); shared FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY. Flags: none.
// Leaf: ?SetDefaultValuesVideo@CUserProfileManager@@QAEXXZ
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
#pragma intrinsic(abs)                       // retail inlines abs in the refresh pick: `cdq ; xor eax,edx ; sub eax,edx` at 0x0040967A..0x004096C8

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

void CUserProfileManager::SetDefaultValuesVideo()
{
    Gamma = 1.0f;
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
    long depth = GFGetSystemManager()->GetDisplayManager()->GetBitDepth();
    if (g_MemoryA > 0)
        width = g_MemoryA;
    if (g_MemoryB > 0)
        height = g_MemoryB;
    if (!def.IsNull()) {
        if (width == -1)
            width = def->ResolutionWidth;
        if (height == -1)
            height = def->ResolutionHeight;
        depth = def->BitDepth;
    }
    if (width == -1)
        width = GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions().X;
    if (height == -1)
        height = GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions().Y;
    const std::vector<CDisplayModeDesc>& modes = GetDisplayModes();
    Resolution = 0;
    for (std::vector<CDisplayModeDesc>::const_iterator it = modes.begin(); it != modes.end(); ++it) {
        if (it->Dimensions.X == width && it->Dimensions.Y == height && it->BitsPerPixel == depth)
            break;
        ++Resolution;
    }
    ClampResolution(Resolution, g_MaxResolutionWidth);
    long refreshRate = GFGetSystemManager()->GetDisplayManager()->GetRefreshRate();
    RefreshRate = 0;
    unsigned long i = 1;
    // do/while: retail keeps this loop rolled (inc ecx ; cmp ecx,4 ; jb 0x409694 at 0x00409694..0x004096C8); a for loop unrolls.
    do {
        if (abs(refreshRate - g_refreshRateOptions[i]) < abs(refreshRate - g_refreshRateOptions[RefreshRate]) && g_refreshRateOptions[i] <= refreshRate)
            RefreshRate = i;
    } while (++i < 4);
    ClampRefreshRate(RefreshRate, g_MaxRefreshRate);
    ScreenAspectRatio = 0;
    VerticalSyncEnabled = true;
    ApplyValues();
}