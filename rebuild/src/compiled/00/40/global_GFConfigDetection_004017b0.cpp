// _global::GFConfigDetection() -- load ConfigDetect.dll, detect the video card and import its recommended settings into the display/detail globals (software-safe defaults when it asks for software mode); false when the DLL or its exports fail
// Leaf: ?GFConfigDetection@@
// Flags: /GS /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004017B0, 875 bytes (+5 int3), plain `ret`, /GS cookie (0x0139C8A8, check
// 0x00BFE9F9) around the 260-byte error-text buffer.  Replaces the landed asm_bake bootstrap with compiler-emitted C++.
// Callees: InitExternalStrings 0x0099E260 (@InitExternalStrings@0), LoadLibraryA / GetProcAddress (cached in esi) / MessageBoxA
// through the IAT, strncpy 0x00BFE9C2, DisplayErrorDialog 0x0099DCB0 (_DisplayErrorDialog@8, also handed to the DLL as its
// error callback), CSystemRegistry ctor 0x0099CF10 / ReadBOOL 0x0099CFE0 / WriteBOOL 0x0099D100 / dtor 0x0099CF70.
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
    int ReadBOOL(const CWideString& valueName, int defaultValue) const;   // 0x0099CFE0
    void WriteBOOL(const CWideString& valueName, int value) const;       // 0x0099D100
private:
    void* RootKey;
    void* OpenKey;
};

// ConfigDetect.dll's detection results (descriptive; only the fields read here)
struct SConfigDetectionInfo {
    unsigned char _pad00[0x04];
    unsigned long MaxResolutionWidth;             // +0x04
    unsigned char _pad08[0x14 - 0x08];
    long          SupportsFeatureG;                     // +0x14
    unsigned char _pad18[0x58 - 0x18];
    long          RequiresSoftwareMode;                 // +0x58
    unsigned char _pad5c[0x70 - 0x5c];
    long          SupportsFeatureA;                     // +0x70
    long          SupportsFeatureB;                     // +0x74
    long          NoBackBufferCapture;                        // +0x78
    long          SupportsFeatureD;                     // +0x7c
    unsigned char _pad80[0x88 - 0x80];
    long          LacksFeatureE;                        // +0x88
    long          LacksFeatureF;                        // +0x8c
    long          MaxDetailA;                           // +0x90 (1-based)
    long          MaxDetailB;                           // +0x94
    long          MaxDetailC;                           // +0x98
    long          MaxDetailD;                           // +0x9c
    long          DefaultDetailA;                       // +0xa0
    long          DefaultDetailB;                       // +0xa4
    long          DefaultDetailC;                       // +0xa8
    long          DefaultDetailD;                       // +0xac
    long          MaxAntialiasing;                          // +0xb0
    unsigned char _padb4[0xb8 - 0xb4];
    unsigned long RecommendedMemoryA;                   // +0xb8
    unsigned long RecommendedMemoryB;                   // +0xbc
};
struct SErrorDialogParams {                             // DisplayErrorDialog's request
    unsigned long TextID;
    unsigned long Flags;
};

typedef int (__stdcall* PFNDisplayErrorDialog)(const SErrorDialogParams* params, int unused);
typedef int (__stdcall* PFNCreateConfigDetection)(SConfigDetectionInfo** info, PFNDisplayErrorDialog displayError);
typedef int (__stdcall* PFNInitVideoCardInfo)(int adapter, char* errorText);
typedef void (__stdcall* PFNCheckRequirements)();

void __fastcall InitExternalStrings();                                          // 0x0099E260
int __stdcall DisplayErrorDialog(const SErrorDialogParams* params, int unused);   // 0x0099DCB0

HINSTANCE__*          g_hConfigDetection;               // 0x013B7C8C
SConfigDetectionInfo* g_pConfigDetectionInfo;           // 0x013BD6E8
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
extern unsigned long g_MaxResolutionWidth;                   // 0x01375084
extern unsigned long g_MaxRefreshRate;                     // 0x01375088
extern unsigned long g_MemoryA;                         // 0x0137509C
extern unsigned long g_MemoryB;                         // 0x013750A0
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
    if (!createConfigDetection(&g_pConfigDetectionInfo, DisplayErrorDialog))
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

            g_FeatureA = g_pConfigDetectionInfo->SupportsFeatureA != 0;
            g_FeatureB = g_pConfigDetectionInfo->SupportsFeatureB != 0;
            g_bCaptureUIBackground = g_pConfigDetectionInfo->NoBackBufferCapture == 0;
            g_FeatureD = g_pConfigDetectionInfo->SupportsFeatureD != 0;
            g_FeatureE = g_pConfigDetectionInfo->LacksFeatureE == 0;
            g_FeatureF = g_pConfigDetectionInfo->LacksFeatureF == 0;
            g_FeatureG = g_pConfigDetectionInfo->SupportsFeatureG != 0;
            g_MaxDetailA = g_pConfigDetectionInfo->MaxDetailA - 1;
            g_MaxDetailB = g_pConfigDetectionInfo->MaxDetailB - 1;
            g_MaxDetailC = g_pConfigDetectionInfo->MaxDetailC - 1;
            g_MaxDetailD = g_pConfigDetectionInfo->MaxDetailD - 1;
            g_DetailA = g_pConfigDetectionInfo->DefaultDetailA - 1;
            g_DetailB = g_pConfigDetectionInfo->DefaultDetailB - 1;
            g_DetailC = g_pConfigDetectionInfo->DefaultDetailC - 1;
            g_DetailD = g_pConfigDetectionInfo->DefaultDetailD - 1;
            g_MaxAntialiasing = g_pConfigDetectionInfo->MaxAntialiasing - 1;
            if (g_pConfigDetectionInfo->MaxResolutionWidth > 0)
                g_MaxResolutionWidth = g_pConfigDetectionInfo->MaxResolutionWidth;
            if (g_pConfigDetectionInfo->RecommendedMemoryA > 0)
                g_MemoryA = g_pConfigDetectionInfo->RecommendedMemoryA;
            if (g_pConfigDetectionInfo->RecommendedMemoryB > 0)
                g_MemoryB = g_pConfigDetectionInfo->RecommendedMemoryB;

            CSystemRegistry registry(g_ConfigRegistryKey, 0);
            int askSoftwareMode = registry.ReadBOOL(g_ConfigRegistryValue, 0);
            g_UseSoftwareMode = g_pConfigDetectionInfo->RequiresSoftwareMode != 0;
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