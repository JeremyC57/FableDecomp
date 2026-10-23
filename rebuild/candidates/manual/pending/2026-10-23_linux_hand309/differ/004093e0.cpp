// CUserProfileManager::SetDefaultValuesVideo() -- reset the video options to the config-options defaults (or the command-line overrides): gamma, the four detail levels, antialiasing, the display mode closest to the wanted resolution and depth, and the refresh rate, all clamped to the command-line caps; then apply
// Grade: DIFFER(924v841) -- retail puts the def branch out of line after the max-width loop, keeps width/height in ebx/ebp with -1 compares, caches the clamped index in a register in both while loops, loads begin/end fresh in the mode search, and does not peel the refresh loop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004093E0, length 841 bytes, plain `ret`.
// Callees: CUserProfileManager::GetConfigOptionsDefaultsDef 0x004083E0 (CDefPointer by value), ClampVideoOption 0x004068D0,
//   ClampAntialiasing 0x00409140, ApplyValues 0x00407370; GFGetSystemManager 0x009A4EC0 (+0x60 display manager),
//   CPixelFormat::GetColourDepth 0x009E3820 (display manager +0x1a8), CDisplayManager::GetRenderTargetDimensions 0x009BEDC0;
//   the def's release through its virtual slot 1. Data: the refresh-rate table 0x0122DE30 (4 longs); the command-line video
//   overrides 0x01375070 .. 0x013750A0 (longs).
// Layout: CUserProfileManager (Ego_r.pdb, retail = PDB - 8 after the two input vectors): Gamma +0x1c, Antialiasing +0x38,
//   TextureDetail +0x3c, ShadowDetail +0x40, MeshDetail +0x44, Resolution +0x48, RefreshRate +0x4c, VerticalSyncEnabled
//   +0x50, EffectsDetail +0x70, ScreenAspectRatio +0x74, DisplayModes +0x80 (CDisplayModeDesc, 16 bytes). CConfigOptions-
//   DefaultsDef (Ego_r.pdb, retail = PDB - 4): Antialiasing +0x3c, ResolutionWidth +0x40, ResolutionHeight +0x44, BitDepth
//   +0x48, TextureDetail +0x54 .. MaxEffectsDetail +0x70. Display manager refresh rate at +0x1c8.
#include <stdlib.h>
#pragma intrinsic(abs)
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefBase : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CConfigOptionsDefaultsDef : CDefBase {
    unsigned char _pad08[0x3c - 0x08];
    unsigned long Antialiasing;                             // +0x3c
    long          ResolutionWidth;                          // +0x40
    long          ResolutionHeight;                         // +0x44
    long          BitDepth;                                 // +0x48
    long          MinResolutionWidth;                       // +0x4c
    long          MinResolutionHeight;                      // +0x50
    float         TextureDetail;                            // +0x54
    float         MaxTextureDetail;                         // +0x58
    float         ShadowDetail;                             // +0x5c
    float         MaxShadowDetail;                          // +0x60
    float         MeshDetail;                               // +0x64
    float         MaxMeshDetail;                            // +0x68
    float         EffectsDetail;                            // +0x6c
    float         MaxEffectsDetail;                         // +0x70
};
struct CDefPointer {                                        // CDefPointer<const CConfigOptionsDefaultsDef>
    CConfigOptionsDefaultsDef* m_p;
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
};
struct C2DExtentsI { long X; long Y; };
struct CPixelFormat {
    long GetColourDepth() const;                            // 0x009E3820
};
struct CDisplayManager {
    unsigned char _pad000[0x1a8];
    CPixelFormat  PixelFormat;                              // +0x1a8
    unsigned char _pad1a9[0x1c8 - 0x1a9];
    long          RefreshRate;                              // +0x1c8
    const C2DExtentsI& GetRenderTargetDimensions(C2DExtentsI& out);   // 0x009BEDC0
};
struct CSystemManager {
    unsigned char    _pad00[0x60];
    CDisplayManager* DisplayManager;                        // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();            // 0x009A4EC0
struct CDisplayModeDesc {                                   // 16 bytes
    C2DExtentsI Dimensions;
    long        BitsPerPixel;
    long        RefreshRate;
};
extern const long g_RefreshRates[4];                        // 0x0122DE30
extern long g_CmdLineTextureDetailCap;                      // 0x01375070 (meshes)
extern long g_CmdLineMeshDetailCap;                         // 0x01375074 (textures)
extern long g_CmdLineShadowDetailCap;                       // 0x01375078
extern long g_CmdLineEffectsDetailCap;                      // 0x0137507C
extern long g_CmdLineAntialiasingCap;                       // 0x01375080
extern long g_CmdLineMaxResolutionWidth;                    // 0x01375084
extern long g_CmdLineMaxRefreshRate;                        // 0x01375088
extern long g_CmdLineMeshDetail;                            // 0x0137508C
extern long g_CmdLineTextureDetail;                         // 0x01375090
extern long g_CmdLineShadowDetail;                          // 0x01375094
extern long g_CmdLineEffectsDetail;                         // 0x01375098
extern long g_CmdLineResolutionWidth;                       // 0x0137509C
extern long g_CmdLineResolutionHeight;                      // 0x013750A0
struct CUserProfileManager {
    unsigned char     _pad00[0x1c];
    float             Gamma;                                // +0x1c
    unsigned char     _pad20[0x38 - 0x20];
    unsigned long     Antialiasing;                         // +0x38
    float             TextureDetail;                        // +0x3c
    float             ShadowDetail;                         // +0x40
    float             MeshDetail;                           // +0x44
    unsigned long     Resolution;                           // +0x48
    unsigned long     RefreshRate;                          // +0x4c
    bool              VerticalSyncEnabled;                  // +0x50
    unsigned char     _pad51[0x70 - 0x51];
    float             EffectsDetail;                        // +0x70
    unsigned long     ScreenAspectRatio;                    // +0x74
    unsigned char     _pad78[0x80 - 0x78];
    CDisplayModeDesc* DisplayModes;                         // +0x80 (vector _M_start)
    CDisplayModeDesc* DisplayModesEnd;                      // +0x84
    CDisplayModeDesc* DisplayModesCapacity;                 // +0x88
    CDefPointer GetConfigOptionsDefaultsDef() const;        // 0x004083E0
    void ClampVideoOption(float* out, float value, float fallback, float limit, float secondaryLimit);   // 0x004068D0
    void ClampAntialiasing(unsigned long* level, long maxLevel);   // 0x00409140
    void ApplyValues();                                     // 0x00407370
    void SetDefaultValuesVideo();
};

void CUserProfileManager::SetDefaultValuesVideo()
{
    Gamma = 1.0f;
    CDefPointer defaults = GetConfigOptionsDefaultsDef();
    const CConfigOptionsDefaultsDef* def = defaults.m_p;
    if (!def) {
        ClampVideoOption(&EffectsDetail, (float)g_CmdLineEffectsDetail, -1.0f, (float)g_CmdLineEffectsDetailCap, -1.0f);
        ClampVideoOption(&TextureDetail, (float)g_CmdLineTextureDetail, -1.0f, (float)g_CmdLineMeshDetailCap, -1.0f);
        ClampVideoOption(&MeshDetail, (float)g_CmdLineMeshDetail, -1.0f, (float)g_CmdLineTextureDetailCap, -1.0f);
        ClampVideoOption(&ShadowDetail, (float)g_CmdLineShadowDetail, -1.0f, (float)g_CmdLineShadowDetailCap, -1.0f);
        Antialiasing = 0;
    } else {
        ClampVideoOption(&EffectsDetail, (float)g_CmdLineEffectsDetail, def->EffectsDetail, (float)g_CmdLineEffectsDetailCap, def->MaxEffectsDetail);
        ClampVideoOption(&TextureDetail, (float)g_CmdLineTextureDetail, def->TextureDetail, (float)g_CmdLineMeshDetailCap, def->MaxTextureDetail);
        ClampVideoOption(&MeshDetail, (float)g_CmdLineMeshDetail, def->MeshDetail, (float)g_CmdLineTextureDetailCap, def->MaxMeshDetail);
        ClampVideoOption(&ShadowDetail, (float)g_CmdLineShadowDetail, def->ShadowDetail, (float)g_CmdLineShadowDetailCap, def->MaxShadowDetail);
        Antialiasing = def->Antialiasing;
    }
    ClampAntialiasing(&Antialiasing, g_CmdLineAntialiasingCap);
    long width = -1;
    long height = -1;
    long depth = GFGetSystemManager()->DisplayManager->PixelFormat.GetColourDepth();
    if (g_CmdLineResolutionWidth > 0)
        width = g_CmdLineResolutionWidth;
    if (g_CmdLineResolutionHeight > 0)
        height = g_CmdLineResolutionHeight;
    if (def) {
        if (width == -1)
            width = def->ResolutionWidth;
        if (height == -1)
            height = def->ResolutionHeight;
        depth = def->BitDepth;
    }
    C2DExtentsI extents;
    if (width == -1)
        width = GFGetSystemManager()->DisplayManager->GetRenderTargetDimensions(extents).X;
    if (height == -1)
        height = GFGetSystemManager()->DisplayManager->GetRenderTargetDimensions(extents).Y;
    Resolution = 0;
    for (CDisplayModeDesc* mode = DisplayModes; mode != DisplayModesEnd; ++mode) {
        if (mode->Dimensions.X == width && mode->Dimensions.Y == height && mode->BitsPerPixel == depth)
            break;
        ++Resolution;
    }
    if (Resolution >= (unsigned long)(DisplayModesEnd - DisplayModes))
        Resolution = 0;
    long maxWidth = g_CmdLineMaxResolutionWidth;
    if (maxWidth != -1 && DisplayModes[Resolution].Dimensions.X > maxWidth) {
        while (DisplayModes[Resolution].Dimensions.X > maxWidth)
            --Resolution;
    }
    long refresh = GFGetSystemManager()->DisplayManager->RefreshRate;
    RefreshRate = 0;
    for (unsigned long i = 1; i < 4; ++i) {
        if (abs(refresh - g_RefreshRates[i]) < abs(refresh - g_RefreshRates[RefreshRate]) && g_RefreshRates[i] <= refresh)
            RefreshRate = i;
    }
    if (RefreshRate >= 4)
        RefreshRate = 0;
    long maxRefresh = g_CmdLineMaxRefreshRate;
    if (maxRefresh != -1 && g_RefreshRates[RefreshRate] > maxRefresh) {
        while (g_RefreshRates[RefreshRate] > maxRefresh)
            --RefreshRate;
    }
    ScreenAspectRatio = 0;
    VerticalSyncEnabled = true;
    ApplyValues();
}
