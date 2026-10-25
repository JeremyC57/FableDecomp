// Behaviour test for 0x004017B0 GFConfigDetection: missing DLL / exports or a failed video probe return false (the probe
// failure shows its error text); otherwise the detection block's flags and 1-based levels are imported (levels - 1,
// non-zero caps only), the software-mode answer (block or dialog) zeroes the levels and caps the mode at 1024 / 60 Hz,
// else each default level is clamped to its maximum; the "asked" registry flag is always written back as 1.
#include <cstdio>
#include <cstring>

struct SConfigDetectionInfo {
    unsigned long MaxResolutionWidth; long SupportsFeatureG; long RequiresSoftwareMode;
    long SupportsFeatureA, SupportsFeatureB, NoBackBufferCapture, SupportsFeatureD, LacksFeatureE, LacksFeatureF;
    long MaxDetailA, MaxDetailB, MaxDetailC, MaxDetailD, DefaultDetailA, DefaultDetailB, DefaultDetailC, DefaultDetailD;
    long MaxAntialiasing; unsigned long RecommendedMemoryA, RecommendedMemoryB;
};
struct SErrorDialogParams { unsigned long TextID; unsigned long Flags; };
typedef int (*PFNDisplayErrorDialog)(const SErrorDialogParams*, int);
typedef int (*PFNCreateConfigDetection)(SConfigDetectionInfo**, PFNDisplayErrorDialog);
typedef int (*PFNInitVideoCardInfo)(int, char*);
typedef void (*PFNCheckRequirements)();

// --- environment mocks
static int g_haveDll, g_haveCreate, g_createOk, g_haveInit, g_initOk, g_haveCheck, g_checked, g_boxes, g_dialogAnswer, g_dialogs;
static int g_regAsk, g_regWritten;
static char g_boxText[64];
static SConfigDetectionInfo g_info;
static int g_dll;
static void* LoadLibraryA_(const char* n) { return (g_haveDll && std::strcmp(n, "ConfigDetect.dll") == 0) ? &g_dll : 0; }
static int Create_(SConfigDetectionInfo** p, PFNDisplayErrorDialog) { *p = &g_info; return g_createOk; }
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
static SConfigDetectionInfo* g_pConfigDetectionInfo;
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
    if (!create(&g_pConfigDetectionInfo, DisplayErrorDialog)) return false;
    PFNInitVideoCardInfo init = (PFNInitVideoCardInfo)GetProcAddress_(g_hConfigDetection, "Config_InitVideoCardInfo");
    if (init) {
        std::strncpy(errorText, "unspecified", 0x103);
        if (init(0, errorText)) {
            PFNCheckRequirements check = (PFNCheckRequirements)GetProcAddress_(g_hConfigDetection, "CheckRequirements");
            if (check) check();
            SConfigDetectionInfo* i = g_pConfigDetectionInfo;
            g_FeatureA = i->SupportsFeatureA != 0; g_FeatureB = i->SupportsFeatureB != 0;
            g_bCaptureUIBackground = i->NoBackBufferCapture == 0; g_FeatureD = i->SupportsFeatureD != 0;
            g_FeatureE = i->LacksFeatureE == 0; g_FeatureF = i->LacksFeatureF == 0; g_FeatureG = i->SupportsFeatureG != 0;
            g_MaxDetailA = i->MaxDetailA - 1; g_MaxDetailB = i->MaxDetailB - 1; g_MaxDetailC = i->MaxDetailC - 1; g_MaxDetailD = i->MaxDetailD - 1;
            g_DetailA = i->DefaultDetailA - 1; g_DetailB = i->DefaultDetailB - 1; g_DetailC = i->DefaultDetailC - 1; g_DetailD = i->DefaultDetailD - 1;
            g_MaxAntialiasing = i->MaxAntialiasing - 1;
            if (i->MaxResolutionWidth > 0) g_MaxResolutionWidth = i->MaxResolutionWidth;
            if (i->RecommendedMemoryA > 0) g_MemoryA = i->RecommendedMemoryA;
            if (i->RecommendedMemoryB > 0) g_MemoryB = i->RecommendedMemoryB;
            CSystemRegistry registry;
            int ask = registry.ReadBOOL(0);
            g_UseSoftwareMode = g_pConfigDetectionInfo->RequiresSoftwareMode != 0;
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
    g_info.MaxResolutionWidth = 1600; g_info.SupportsFeatureA = 1; g_info.NoBackBufferCapture = 0; g_info.LacksFeatureF = 5;
    g_info.MaxDetailA = 2; g_info.DefaultDetailA = 3;      // default above max: clamped to max
    g_info.MaxDetailB = 4; g_info.DefaultDetailB = 2;      // default below max: kept
    g_info.MaxDetailC = 0; g_info.DefaultDetailC = 3;      // max unknown (-1): not clamped
    g_info.MaxDetailD = 3; g_info.DefaultDetailD = 3;
    g_info.MaxAntialiasing = 5; g_info.RecommendedMemoryA = 0; g_info.RecommendedMemoryB = 64;
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
    reset(); g_info.RequiresSoftwareMode = 1; g_regAsk = 1;
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