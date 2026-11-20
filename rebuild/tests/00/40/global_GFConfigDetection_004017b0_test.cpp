// Behaviour test for 0x004017B0 GFConfigDetection: missing DLL / exports or a failed video probe return false (the probe
// failure shows its error text); otherwise the detection block's flags and 1-based levels are imported (levels - 1,
// non-zero caps only), the software-mode answer (block or dialog) zeroes the levels and caps the mode at 1024 / 60 Hz,
// else each default level is clamped to its maximum; the "asked" registry flag is always written back as 1.
#include <cstdio>
#include <cstring>

#include "engine/ConfigDetection.h"   // the result block (Ego_r TPI members), as the row (NF183 q4245)
struct SErrorDialogParams { unsigned long TextID; unsigned long Flags; };
typedef int (*PFNDisplayErrorDialog)(const SErrorDialogParams*, int);
typedef int (*PFNCreateConfigDetection)(ConfigDetection**, PFNDisplayErrorDialog);
typedef int (*PFNInitVideoCardInfo)(int, char*);
typedef void (*PFNCheckRequirements)();

// --- environment mocks
static int g_haveDll, g_haveCreate, g_createOk, g_haveInit, g_initOk, g_haveCheck, g_checked, g_boxes, g_dialogAnswer, g_dialogs;
static int g_regAsk, g_regWritten;
static char g_boxText[64];
static ConfigDetection g_info;
static int g_dll;
static void* LoadLibraryA_(const char* n) { return (g_haveDll && std::strcmp(n, "ConfigDetect.dll") == 0) ? &g_dll : 0; }
static int Create_(ConfigDetection** p, PFNDisplayErrorDialog) { *p = &g_info; return g_createOk; }
static int Init_(int adapter, char* text) { if (adapter != 0 || std::strcmp(text, "unspecified") != 0) return 0; if (!g_initOk) std::strcpy(text, "no D3D9"); return g_initOk; }
static void Check_() { ++g_checked; }
static void* GetProcAddress_(void* m, const char* n)
{
    if (m != &g_dll) return 0;
    if (!std::strcmp(n, "CreateConfigDetection")) return g_haveCreate ? (void*)Create_ : 0;
    if (!std::strcmp(n, "Config_InitVideoCardInfo")) return g_haveInit ? (void*)Init_ : 0;
    if (!std::strcmp(n, "CheckRequirements")) return g_haveCheck ? (void*)Check_ : 0;
    return 0;
}
static int MessageBoxA_(void*, const char* text, const char* caption, unsigned int) { if (!std::strcmp(caption, "ConfigDetection Error")) { ++g_boxes; std::strcpy(g_boxText, text); } return 1; }
static int DisplayErrorDialog(const SErrorDialogParams* p, int) { ++g_dialogs; return (p->TextID == 0x801 && p->Flags == 0) ? g_dialogAnswer : 0; }
struct CSystemRegistry {
    int ReadBOOL(int) const { return g_regAsk; }
    void WriteBOOL(int v) const { g_regWritten = v; }
};
static void InitExternalStrings() { }

static void* g_hConfigDetection;
static ConfigDetection* pConfig;
static long g_MaxDetailA, g_MaxDetailB, g_MaxDetailC, g_MaxDetailD, g_DetailA, g_DetailB, g_DetailC, g_DetailD, g_MaxAntialiasing;
static unsigned long g_MaxResolutionWidth, g_MaxRefreshRate, g_MemoryA, g_MemoryB;
static bool g_FeatureA, g_FeatureB, g_FeatureD, g_bCaptureUIBackground, g_FeatureE, g_FeatureF, g_FeatureG, g_UseSoftwareMode;

// the function under test (shape of 0x004017B0)
static bool GFConfigDetection()
{
    char errorText[0x104];
    InitExternalStrings();
    g_hConfigDetection = LoadLibraryA_("ConfigDetect.dll");
    if (g_hConfigDetection == 0) return false;
    PFNCreateConfigDetection create = (PFNCreateConfigDetection)GetProcAddress_(g_hConfigDetection, "CreateConfigDetection");
    if (create == 0) return false;
    if (!create(&pConfig, DisplayErrorDialog)) return false;
    PFNInitVideoCardInfo init = (PFNInitVideoCardInfo)GetProcAddress_(g_hConfigDetection, "Config_InitVideoCardInfo");
    if (init) {
        std::strncpy(errorText, "unspecified", 0x103);
        if (init(0, errorText)) {
            PFNCheckRequirements check = (PFNCheckRequirements)GetProcAddress_(g_hConfigDetection, "CheckRequirements");
            if (check) check();
            ConfigDetection* i = pConfig;
            g_FeatureA = i->cf_UseAlternateRenderTargetClear != 0; g_FeatureB = i->cf_UseAlternateOutlineEffect != 0;
            g_bCaptureUIBackground = i->cf_DisableStretchRect == 0; g_FeatureD = i->cf_UseAlternateSkyPixelShader != 0;
            g_FeatureE = i->cf_DisableFilteredScreenGUIBackground == 0; g_FeatureF = i->cf_DisableTextureTransforms == 0; g_FeatureG = i->cf_UseFixedFunction != 0;
            g_MaxDetailA = i->cf_MaxEffectsDetail - 1; g_MaxDetailB = i->cf_MaxShadowDetail - 1; g_MaxDetailC = i->cf_MaxMeshDetail - 1; g_MaxDetailD = i->cf_MaxTextureDetail - 1;
            g_DetailA = i->cf_DefaultEffectsDetail - 1; g_DetailB = i->cf_DefaultShadowDetail - 1; g_DetailC = i->cf_DefaultMeshDetail - 1; g_DetailD = i->cf_DefaultTextureDetail - 1;
            g_MaxAntialiasing = i->cf_MaxAntiAliasLevel - 1;
            if (i->cf_MaximumResolution > 0) g_MaxResolutionWidth = i->cf_MaximumResolution;
            if (i->cf_GenericFlag0 > 0) g_MemoryA = i->cf_GenericFlag0;
            if (i->cf_GenericFlag1 > 0) g_MemoryB = i->cf_GenericFlag1;
            CSystemRegistry registry;
            int ask = registry.ReadBOOL(0);
            g_UseSoftwareMode = pConfig->cf_SafeMode != 0;
            if (!g_UseSoftwareMode && ask) {
                SErrorDialogParams params; params.TextID = 0x801; params.Flags = 0;
                if (DisplayErrorDialog(&params, 0) == 1) g_UseSoftwareMode = true;
            }
            registry.WriteBOOL(1);
            if (g_UseSoftwareMode) {
                g_MaxDetailA = g_MaxDetailB = g_MaxDetailC = g_MaxDetailD = 0;
                g_DetailA = g_DetailB = g_DetailC = g_DetailD = 0; g_MaxAntialiasing = 0;
                g_MaxResolutionWidth = 0x400; g_MaxRefreshRate = 0x3c;
                return true;
            }
            if (g_MaxDetailA >= 0 && g_DetailA >= 0 && g_MaxDetailA < g_DetailA) g_DetailA = g_MaxDetailA;
            if (g_MaxDetailB >= 0 && g_DetailB >= 0 && g_MaxDetailB < g_DetailB) g_DetailB = g_MaxDetailB;
            if (g_MaxDetailC >= 0 && g_DetailC >= 0 && g_MaxDetailC < g_DetailC) g_DetailC = g_MaxDetailC;
            if (g_MaxDetailD >= 0 && g_DetailD >= 0 && g_MaxDetailD < g_DetailD) g_DetailD = g_MaxDetailD;
            return true;
        }
        MessageBoxA_(0, errorText, "ConfigDetection Error", 0);
    }
    return false;
}

static void reset()
{
    g_haveDll = g_haveCreate = g_createOk = g_haveInit = g_initOk = g_haveCheck = 1;
    g_checked = g_boxes = g_dialogs = 0; g_dialogAnswer = 0; g_regAsk = 0; g_regWritten = -1;
    g_MaxResolutionWidth = 0xFFFFFFFF; g_MaxRefreshRate = 0xFFFFFFFF; g_MemoryA = 11; g_MemoryB = 22;
    std::memset(&g_info, 0, sizeof(g_info));
    g_info.cf_MaximumResolution = 1600; g_info.cf_UseAlternateRenderTargetClear = 1; g_info.cf_DisableStretchRect = 0; g_info.cf_DisableTextureTransforms = 5;
    g_info.cf_MaxEffectsDetail = 2; g_info.cf_DefaultEffectsDetail = 3;      // default above max: clamped to max
    g_info.cf_MaxShadowDetail = 4; g_info.cf_DefaultShadowDetail = 2;      // default below max: kept
    g_info.cf_MaxMeshDetail = 0; g_info.cf_DefaultMeshDetail = 3;      // max unknown (-1): not clamped
    g_info.cf_MaxTextureDetail = 3; g_info.cf_DefaultTextureDetail = 3;
    g_info.cf_MaxAntiAliasLevel = 5; g_info.cf_GenericFlag0 = 0; g_info.cf_GenericFlag1 = 64;
}

int main()
{
    int ok = 1;
    reset(); g_haveDll = 0;    if (GFConfigDetection() || g_regWritten != -1) ok = 0;
    reset(); g_haveCreate = 0; if (GFConfigDetection()) ok = 0;
    reset(); g_createOk = 0;   if (GFConfigDetection()) ok = 0;
    reset(); g_haveInit = 0;   if (GFConfigDetection() || g_boxes != 0) ok = 0;
    reset(); g_initOk = 0;     if (GFConfigDetection() || g_boxes != 1 || std::strcmp(g_boxText, "no D3D9") != 0 || g_regWritten != -1) ok = 0;
    // hardware path
    reset();
    if (!GFConfigDetection() || g_checked != 1 || g_regWritten != 1 || g_dialogs != 0 || g_UseSoftwareMode) ok = 0;
    if (!g_FeatureA || g_FeatureB || !g_bCaptureUIBackground || g_FeatureD || !g_FeatureE || g_FeatureF || g_FeatureG) ok = 0;
    if (g_MaxDetailA != 1 || g_DetailA != 1 || g_DetailB != 1 || g_MaxDetailC != -1 || g_DetailC != 2 || g_DetailD != 2) ok = 0;
    if (g_MaxAntialiasing != 4 || g_MaxResolutionWidth != 1600 || g_MemoryA != 11 || g_MemoryB != 64 || g_MaxRefreshRate != 0xFFFFFFFF) ok = 0;
    // the block asks for software mode
    reset(); g_info.cf_SafeMode = 1; g_regAsk = 1;
    if (!GFConfigDetection() || !g_UseSoftwareMode || g_dialogs != 0 || g_DetailA != 0 || g_MaxAntialiasing != 0 || g_MaxResolutionWidth != 0x400 || g_MaxRefreshRate != 60) ok = 0;
    // the registry asks and the user picks software mode in the dialog / declines
    reset(); g_regAsk = 1; g_dialogAnswer = 1;
    if (!GFConfigDetection() || !g_UseSoftwareMode || g_dialogs != 1 || g_MaxDetailB != 0) ok = 0;
    reset(); g_regAsk = 1; g_dialogAnswer = 2;
    if (!GFConfigDetection() || g_UseSoftwareMode || g_dialogs != 1 || g_DetailA != 1 || g_regWritten != 1) ok = 0;
    // no CheckRequirements export is fine
    reset(); g_haveCheck = 0;  if (!GFConfigDetection() || g_checked != 0) ok = 0;
    std::printf(ok ? "004017b0_TEST PASS\n" : "004017b0_TEST FAIL\n");
    return ok ? 0 : 1;
}