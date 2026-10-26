// CEngine::SetVideoOptions(const CEngineVideoOptions& options) -- apply new video options: copy them (and publish the global
//   pointer), set the display mode (requested settings; then without antialiasing / refresh; then clamped to 1024x768 -- any
//   fallback makes the result false, total failure returns false), then push the vsync, effects quality, hi-res texture memory
//   and mesh detail settings to their subsystems and restore component video memory; the post-device-reset flag is cleared for
//   the duration and set again on every exit after the initialised check.
// Leaf: SetVideoOptions
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B24690, 443 bytes (+int3), `ret 4`.
// Callees: CDisplayManager::SetDisplayMode 0x009BFF40 (x3) and 0x009BFF10 (bool !vsync, 1, 1; the manifest's
//   CTCInventoryAbilities::AddLearnedExpression label is propagated), CShaderBankStateBlock::Set 0x00B50C60,
//   CEngineScreenEffectColourFilter::SetEffectQuality 0x00B5A0B0, slot 13 of the object at 0x01436E54,
//   CEngineResourceManager::ResizeHiresTextureMemory 0x00B3B4F0, 0x00B32010 on the object at 0x01436E80 (quality, def; the
//   manifest's `Catch@0040d88c` label is wrong), CEngineLocalDetailGenerator::UpdateLoadingDistanceVideoOptions 0x00B59870,
//   0x00B2F740 on the object at 0x01436E58 (manifest `CXMVPlaybackInfo` ctor label is propagated),
//   CEngine::SmartRestoreComponentVideoMemory 0x00B24500. Subsystem class names past the manifest's are descriptive.
// Layout: Ego_r.pdb CEngine less 8 bytes in this region (IsInitialised +0x84, EnablePostDeviceReset +0xbd, VideoOptions +0xc0,
//   PEngineVideoOptionsDefs[4] +0x120); engine/CEngine.h carries the PDB offsets (+0x8c ...), hence a local model.
// Shape: the bool result lives in the dead `options` argument slot; the SmartRestore failure is `result = false` falling into
//   the shared exit (an early `return false` there puts the display-mode failure block out of line); the effects definition is
//   re-indexed from `this` for each of the three users, the texture/mesh definitions are read through the global engine.
class C2DExtentsI { public: long X; long Y; };
class C2DExtentsF { public: float X; float Y; };
enum EEngineVideoOptionQuality { QUALITY_LOW, QUALITY_MEDIUM, QUALITY_HIGH, QUALITY_ULTRA };
enum ESurfaceMultisampleType { MULTISAMPLE_NONE = 0 };
enum ECreateDeviceSuccess { CREATE_DEVICE_FAILED = 0, CREATE_DEVICE_OK = 2 };
class CEngineVideoOptions { public:
    C2DExtentsI               ScreenResolution;   // +0x00
    C2DExtentsF               ScreenAspectRatio;  // +0x08
    long                      BitsPerPixel;       // +0x10
    long                      RefreshRate;        // +0x14
    bool                      VerticalSync;       // +0x18
    EEngineVideoOptionQuality LandscapeDetail;    // +0x1c
    EEngineVideoOptionQuality MeshDetail;         // +0x20
    EEngineVideoOptionQuality TextureDetail;      // +0x24
    EEngineVideoOptionQuality ShadowQuality;      // +0x28
    EEngineVideoOptionQuality EffectsQuality;     // +0x2c
    ESurfaceMultisampleType   Antialiasing;       // +0x30
};
class CEngineVideoOptionsDef { public:
    unsigned char _pad[0x3c];
    long          HiresTextureMemory;             // +0x3c
};
class CDisplayManager { public:
    ECreateDeviceSuccess SetDisplayMode(const C2DExtentsI& resolution, unsigned long bitsPerPixel, ESurfaceMultisampleType antialiasing, unsigned long refreshRate);   // 0x009BFF40
    void SetPresentParameters(bool immediate, bool a, bool b);   // 0x009BFF10
};
struct CShaderBankStateBlock { void Set(CEngineVideoOptionsDef* def); };                       // 0x00B50C60
class CEngineScreenEffectColourFilter { public: void SetEffectQuality(CEngineVideoOptionsDef* def); };   // 0x00B5A0B0
struct CEngineVideoOptionsListener {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual void SetEffectQuality(CEngineVideoOptionsDef* def);   // slot 13 (+0x34)
};
class CEngineResourceManager { public: bool ResizeHiresTextureMemory(long size); };        // 0x00B3B4F0
struct CEngineMeshDetailManager { void SetMeshDetail(EEngineVideoOptionQuality quality, CEngineVideoOptionsDef* def); };   // 0x00B32010
class CEngineLocalDetailGenerator { public: void UpdateLoadingDistanceVideoOptions(CEngineVideoOptionsDef* def); };   // 0x00B59870
struct CEngineVideoOptionsSink { void OnVideoOptionsChanged(); };                 // 0x00B2F740

enum ESmartRestoreComponentVideoMemoryResult { SMART_RESTORE_OK = 0 };

class CEngine { public:
    unsigned char           _pad00[0x84];
    bool                    IsInitialised;                // +0x84
    unsigned char           _pad85[0xbd - 0x85];
    bool                    EnablePostDeviceReset;        // +0xbd
    unsigned char           _padbe[2];
    CEngineVideoOptions     VideoOptions;                 // +0xc0
    unsigned char           _padf4[0x120 - 0xf4];
    CEngineVideoOptionsDef* PEngineVideoOptionsDefs[4];   // +0x120 CDefPointer<CEngineVideoOptionsDef>
    ESmartRestoreComponentVideoMemoryResult SmartRestoreComponentVideoMemory();   // 0x00B24500
    bool SetVideoOptions(const CEngineVideoOptions& options);
};

extern CDisplayManager*                 g_pDisplayManager_01436e1c;        // 0x01436E1C
extern CEngineScreenEffectColourFilter* g_pColourFilter;          // 0x01436E40
extern CShaderBankStateBlock*           g_pShaderBankStateBlock;  // 0x01436E44
extern CEngineVideoOptionsListener*     g_pVideoOptionsListener;  // 0x01436E54
extern CEngineVideoOptionsSink*         g_pVideoOptionsSink;      // 0x01436E58
extern CEngineMeshDetailManager*        g_pMeshDetailManager;     // 0x01436E80
extern CEngineLocalDetailGenerator*     g_pLocalDetailGenerator;  // 0x01436E88
extern CEngineResourceManager*          g_pResourceManager;       // 0x01436E98
extern CEngine*                         g_pEngine_01436ea4;                // 0x01436EA4
extern CEngineVideoOptions*             g_pVideoOptions;          // 0x01436EB0

bool CEngine::SetVideoOptions(const CEngineVideoOptions& options)
{
    if (!IsInitialised)
        return false;
    EnablePostDeviceReset = false;
    VideoOptions = options;
    g_pVideoOptions = &VideoOptions;
    bool result = true;
    if (g_pDisplayManager_01436e1c->SetDisplayMode(options.ScreenResolution, options.BitsPerPixel, options.Antialiasing, options.RefreshRate) != CREATE_DEVICE_OK) {
        result = false;
        if (g_pDisplayManager_01436e1c->SetDisplayMode(options.ScreenResolution, options.BitsPerPixel, MULTISAMPLE_NONE, 0) == CREATE_DEVICE_FAILED) {
            C2DExtentsI resolution = options.ScreenResolution;
            if (resolution.X > 1024)
                resolution.X = 1024;
            if (resolution.Y > 768)
                resolution.Y = 768;
            if (g_pDisplayManager_01436e1c->SetDisplayMode(resolution, options.BitsPerPixel, MULTISAMPLE_NONE, 0) == CREATE_DEVICE_FAILED) {
                EnablePostDeviceReset = true;
                return false;
            }
        }
    }
    g_pDisplayManager_01436e1c->SetPresentParameters(!options.VerticalSync, true, true);
    g_pShaderBankStateBlock->Set(PEngineVideoOptionsDefs[VideoOptions.EffectsQuality]);
    g_pColourFilter->SetEffectQuality(PEngineVideoOptionsDefs[VideoOptions.EffectsQuality]);
    g_pVideoOptionsListener->SetEffectQuality(PEngineVideoOptionsDefs[VideoOptions.EffectsQuality]);
    g_pResourceManager->ResizeHiresTextureMemory(g_pEngine_01436ea4->PEngineVideoOptionsDefs[VideoOptions.TextureDetail]->HiresTextureMemory);
    CEngineVideoOptionsDef* meshDef = g_pEngine_01436ea4->PEngineVideoOptionsDefs[VideoOptions.MeshDetail];
    g_pMeshDetailManager->SetMeshDetail(VideoOptions.MeshDetail, meshDef);
    if (g_pLocalDetailGenerator)
        g_pLocalDetailGenerator->UpdateLoadingDistanceVideoOptions(meshDef);
    g_pVideoOptionsSink->OnVideoOptionsChanged();
    if (SmartRestoreComponentVideoMemory() != SMART_RESTORE_OK)
        result = false;
    EnablePostDeviceReset = true;
    return result;
}