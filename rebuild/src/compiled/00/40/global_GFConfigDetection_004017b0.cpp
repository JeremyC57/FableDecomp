// _global::GFConfigDetection() -- load ConfigDetect.dll, detect the video card and import its recommended settings into the display/detail globals (software-safe defaults when it asks for software mode); false when the DLL or its exports fail
// Leaf: ?GFConfigDetection@@YI_NXZ
// Flags: /GS /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004017B0, 875 bytes (+5 int3), plain `ret`, /GS cookie (0x0139C8A8, check
// 0x00BFE9F9) around the 260-byte error-text buffer.  Replaces the landed asm_bake bootstrap with compiler-emitted C++.
// Callees: InitExternalStrings 0x0099E260 (@InitExternalStrings@0), LoadLibraryA / GetProcAddress (cached in esi) / MessageBoxA
// through the IAT, strncpy 0x00BFE9C2, DisplayErrorDialog 0x0099DCB0 (_DisplayErrorDialog@8, also handed to the DLL as its
// error callback), CSystemRegistry ctor 0x0099CF10 / GetBOOL 0x0099CFE0 (Ego_r.pdb name; sweep2) / WriteBOOL 0x0099D100 / dtor 0x0099CF70.
// Data: the DLL handle 0x013B7C8C and its result block pointer 0x013BD6E8 (filled by CreateConfigDetection); the registry key
// and value CWideStrings 0x013BCA28 / 0x013BCA2C; the caps the user-profile code reads: max antialiasing 0x01375080
// (CUserProfileManager::GetMaxAntialiasing 0x00409090), max resolution width 0x01375084 (GetMaxResolution 0x00408900), max
// refresh rate 0x01375088 (GetMaxRefreshRate 0x00406890), the UI-background capture flag 0x013750A7 (CaptureBackBufferForUI-
// Background 0x00406CC0); the other detail globals 0x01375070..0x013750AC are named descriptively.
// Shape: early `return false` for the DLL and CreateConfigDetection; the video-card probe nests the whole success path
// (`if (init) { strncpy; if (init(0, text)) { ...; return true; } MessageBoxA(...); } return false;`), which is what moves
// the MessageBoxA block and the shared `return false` to the end.  The result block is read through the global pointer
// (VC7.1 keeps it in eax across the byte-flag stores); the software-mode branch returns before the four detail clamps.
#include <stddef.h>
struct HINSTANCE__;
struct HWND__;
extern "C" __declspec(dllimport) HINSTANCE__* __stdcall LoadLibraryA(const char* name);                     // [0x0143FE40]
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(HINSTANCE__* module, const char* name);     // [0x0143FE44]
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(HWND__* window, const char* text, const char* caption, unsigned int type);   // [0x014402C4]
extern "C" char* __cdecl strncpy(char* dest, const char* src, size_t count);                               // 0x00BFE9C2

class CWideString {
public:
    void* PStringData;
};
class CSystemRegistry {                                 // 8 bytes {root key, open key}
public:
    CSystemRegistry(const CWideString& subKey, long rootSelection);   // 0x0099CF10
    ~CSystemRegistry();                                 // 0x0099CF70
    int GetBOOL(const CWideString& valueName, int defaultValue) const;   // 0x0099CFE0
    void WriteBOOL(const CWideString& valueName, int value) const;       // 0x0099D100
private:
    void* RootKey;
    void* OpenKey;
};

// Naming (NF183, 2026-11-20, q4245): the descriptive SConfigDetectionInfo / g_pConfigDetectionInfo model of the result block moved onto
//   the shared engine/ConfigDetection.h: 0x013BD6E8 is Ego_d.pdb's public ?pConfig@@3PAVConfigDetection@@A (the spelling the landed
//   DisplayErrorDialog 0x0099DCB0 binds through engine/ErrorDialog.h); the fields read here are the Ego_r TPI members at the same offsets.
#include "engine/ConfigDetection.h"
struct SErrorDialogParams {                             // DisplayErrorDialog's request
    unsigned long TextID;
    unsigned long Flags;
};

typedef int (__stdcall* PFNDisplayErrorDialog)(const SErrorDialogParams* params, int unused);
typedef int (__stdcall* PFNCreateConfigDetection)(ConfigDetection** info, PFNDisplayErrorDialog displayError);
typedef int (__stdcall* PFNInitVideoCardInfo)(int adapter, char* errorText);
typedef void (__stdcall* PFNCheckRequirements)();

void __fastcall InitExternalStrings();                                          // 0x0099E260
int __stdcall DisplayErrorDialog(const SErrorDialogParams* params, int unused);   // 0x0099DCB0

HINSTANCE__*          g_hConfigDetection;               // 0x013B7C8C
ConfigDetection*      pConfig;                          // 0x013BD6E8 ?pConfig@@3PAVConfigDetection@@A
extern CWideString    g_ConfigRegistryKey;              // 0x013BCA28
extern CWideString    g_ConfigRegistryValue;            // 0x013BCA2C
extern long  g_MaxDetailA;                              // 0x0137507C
extern long  g_MaxDetailB;                              // 0x01375078
extern long  g_MaxDetailC;                              // 0x01375070
extern long  g_MaxDetailD;                              // 0x01375074
extern long  g_DetailA;                                 // 0x01375098
extern long  g_DetailB;                                 // 0x01375094
extern long  g_DetailC;                                 // 0x0137508C
extern long  g_DetailD;                                 // 0x01375090
extern long  g_MaxAntialiasing;                             // 0x01375080
extern long g_MaxResolutionWidth;                          // 0x01375084
extern long g_MaxRefreshRate;                            // 0x01375088
extern long g_MemoryA;                                // 0x0137509C
extern long g_MemoryB;                                // 0x013750A0
extern bool  g_FeatureA;                                // 0x013750A4
extern bool  g_FeatureB;                                // 0x013750A5
extern bool  g_FeatureD;                                // 0x013750A6
extern bool  g_bCaptureUIBackground;                                // 0x013750A7
extern bool  g_FeatureE;                                // 0x013750A9
extern bool  g_FeatureF;                                // 0x013750AA
extern bool  g_FeatureG;                                // 0x013750AB
extern bool  g_UseSoftwareMode;                         // 0x013750AC

bool GFConfigDetection()
{
    char errorText[0x104];
    InitExternalStrings();
    g_hConfigDetection = LoadLibraryA("ConfigDetect.dll");
    if (g_hConfigDetection == 0)
        return false;
    PFNCreateConfigDetection createConfigDetection =
        (PFNCreateConfigDetection)GetProcAddress(g_hConfigDetection, "CreateConfigDetection");
    if (createConfigDetection == 0)
        return false;
    if (!createConfigDetection(&pConfig, DisplayErrorDialog))
        return false;
    PFNInitVideoCardInfo initVideoCardInfo =
        (PFNInitVideoCardInfo)GetProcAddress(g_hConfigDetection, "Config_InitVideoCardInfo");
    if (initVideoCardInfo) {
        strncpy(errorText, "unspecified", 0x103);
        if (initVideoCardInfo(0, errorText)) {
            PFNCheckRequirements checkRequirements =
                (PFNCheckRequirements)GetProcAddress(g_hConfigDetection, "CheckRequirements");
            if (checkRequirements)
                checkRequirements();

            g_FeatureA = pConfig->cf_UseAlternateRenderTargetClear != 0;
            g_FeatureB = pConfig->cf_UseAlternateOutlineEffect != 0;
            g_bCaptureUIBackground = pConfig->cf_DisableStretchRect == 0;
            g_FeatureD = pConfig->cf_UseAlternateSkyPixelShader != 0;
            g_FeatureE = pConfig->cf_DisableFilteredScreenGUIBackground == 0;
            g_FeatureF = pConfig->cf_DisableTextureTransforms == 0;
            g_FeatureG = pConfig->cf_UseFixedFunction != 0;
            g_MaxDetailA = pConfig->cf_MaxEffectsDetail - 1;
            g_MaxDetailB = pConfig->cf_MaxShadowDetail - 1;
            g_MaxDetailC = pConfig->cf_MaxMeshDetail - 1;
            g_MaxDetailD = pConfig->cf_MaxTextureDetail - 1;
            g_DetailA = pConfig->cf_DefaultEffectsDetail - 1;
            g_DetailB = pConfig->cf_DefaultShadowDetail - 1;
            g_DetailC = pConfig->cf_DefaultMeshDetail - 1;
            g_DetailD = pConfig->cf_DefaultTextureDetail - 1;
            g_MaxAntialiasing = pConfig->cf_MaxAntiAliasLevel - 1;
            if (pConfig->cf_MaximumResolution > 0)
                g_MaxResolutionWidth = pConfig->cf_MaximumResolution;
            if (pConfig->cf_GenericFlag0 > 0)
                g_MemoryA = pConfig->cf_GenericFlag0;
            if (pConfig->cf_GenericFlag1 > 0)
                g_MemoryB = pConfig->cf_GenericFlag1;

            CSystemRegistry registry(g_ConfigRegistryKey, 0);
            int askSoftwareMode = registry.GetBOOL(g_ConfigRegistryValue, 0);
            g_UseSoftwareMode = pConfig->cf_SafeMode != 0;
            if (!g_UseSoftwareMode && askSoftwareMode) {
                SErrorDialogParams params;
                params.TextID = 0x801;
                params.Flags = 0;
                if (DisplayErrorDialog(&params, 0) == 1)
                    g_UseSoftwareMode = true;
            }
            registry.WriteBOOL(g_ConfigRegistryValue, 1);
            if (g_UseSoftwareMode) {
                g_MaxDetailA = 0;
                g_MaxDetailB = 0;
                g_MaxDetailC = 0;
                g_MaxDetailD = 0;
                g_DetailA = 0;
                g_DetailB = 0;
                g_DetailC = 0;
                g_DetailD = 0;
                g_MaxAntialiasing = 0;
                g_MaxResolutionWidth = 0x400;
                g_MaxRefreshRate = 0x3c;
                return true;
            }
            if (g_MaxDetailA >= 0 && g_DetailA >= 0 && g_MaxDetailA < g_DetailA)
                g_DetailA = g_MaxDetailA;
            if (g_MaxDetailB >= 0 && g_DetailB >= 0 && g_MaxDetailB < g_DetailB)
                g_DetailB = g_MaxDetailB;
            if (g_MaxDetailC >= 0 && g_DetailC >= 0 && g_MaxDetailC < g_DetailC)
                g_DetailC = g_MaxDetailC;
            if (g_MaxDetailD >= 0 && g_DetailD >= 0 && g_MaxDetailD < g_DetailD)
                g_DetailD = g_MaxDetailD;
            return true;
        }
        MessageBoxA(0, errorText, "ConfigDetection Error", 0);
    }
    return false;
}