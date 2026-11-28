// Flags: 
// CEngine::SetVideoOptions(const CEngineVideoOptions& options) -- apply new video options: copy them (and publish the global
//   pointer), set the display mode (requested settings; then without antialiasing / refresh; then clamped to 1024x768 -- any
//   fallback makes the result false, total failure returns false), then push the vsync, effects quality, hi-res texture memory
//   and mesh detail settings to their subsystems and restore component video memory; the post-device-reset flag is cleared for
//   the duration and set again on every exit after the initialised check.
// Leaf: ?SetVideoOptions@CEngine@@UAE_NABVCEngineVideoOptions@@@Z
// NF95 (part 2, q11381): 0x01436EB0 is NEngineInternal::GPVideoOptions (FableWin.pdb public ?GPVideoOptions@NEngineInternal@@3PBVCEngineVideoOptions@@B,
//   FableWin 0x04AC8094; Ego_r.pdb data order GPLightingManager / GPEngine / GPVideoOptions; CEngine::SetVideoOptions 0x00B24690 stores
//   &VideoOptions there), a pointer to const CEngineVideoOptions (Ego_r.pdb layout, 0x34 bytes).
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
#include "engine/EEngineVideoOptionQuality.h"
enum ESurfaceMultisampleType { MULTISAMPLE_NONE = 0 };
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
#define FABLE_RETAIL_CENGINEVIDEOOPTIONSDEF_HIRESTEXTUREMEMORY
#include "engine/CEngineVideoOptionsDef.h"   // HiresTextureMemory +0x3C (retail)
class CDisplayManager { public:
    // NF140 (q4056): Ego_r.pdb / FableWin.pdb nest the result enum in CDisplayManager (Ego_d.pdb LF_ENUM enumerators).
    enum ECreateDeviceSuccess { CREATE_DEVICE_FAILED = 0, CREATE_DEVICE_DEVICE_LOST = 1, CREATE_DEVICE_SUCCESS = 2 };
    ECreateDeviceSuccess SetDisplayMode(const C2DExtentsI& resolution, unsigned long bitsPerPixel, ESurfaceMultisampleType antialiasing, unsigned long refreshRate);   // 0x009BFF40
    // NF145 (q11808): in-place respell to the PDB overload (Ego_r.pdb 0x00B24FA0 / FableWin.pdb 0x0301CD60, bool / long / bool; was a
    // three-bool SetPresentParameters stand-in); the pushes are the same (`push 1 ; push 1 ; push <bool>`).
    void ChangePresentationInterval(bool present_immediate, long vsync_count, bool present_immediate_if_sync_missed);   // 0x009BFF10
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
class CEngineLocalDetailGenerator { public: void UpdateLoadingDistanceVideoOptions(const CEngineVideoOptionsDef& def); };   // 0x00B59870
#define FABLE_RETAIL_ENGINE_LOADSCENE_PASS30
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER
#include "engine/CEnginePrimitiveOcclusionManager.h"                 // 0x00B2F740

enum ESmartRestoreComponentVideoMemoryResult { SMART_RESTORE_OK = 0 };

class CEngine { public:
    unsigned char _pad00[0x80];
    bool                    IsInitialised;                // +0x84
    unsigned char           _pad85[0xbd - 0x85];
    bool                    EnablePostDeviceReset;        // +0xbd
    unsigned char           _padbe[2];
    CEngineVideoOptions     VideoOptions;                 // +0xc0
    unsigned char           _padf4[0x120 - 0xf4];
    CEngineVideoOptionsDef* PEngineVideoOptionsDefs[4];   // +0x120 CDefPointer<CEngineVideoOptionsDef>
    ESmartRestoreComponentVideoMemoryResult SmartRestoreComponentVideoMemory();   // 0x00B24500
    virtual bool SetVideoOptions(const CEngineVideoOptions& options);
};

namespace NEngineInternal { extern CDisplayManager* GPDisplayManager; }   // 0x01436E1C ?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A
extern CEngineScreenEffectColourFilter* g_pColourFilter;          // 0x01436E40
extern CShaderBankStateBlock*           g_pShaderBankStateBlock;  // 0x01436E44
extern CEngineVideoOptionsListener*     g_pVideoOptionsListener;  // 0x01436E54
#include "engine/NEngineInternal.h"      // 0x01436E58
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER_PASS22_DRAWDISTANCE
#include "engine/CEnginePrimitiveRenderer.h"   // ?SetDrawDistanceQuality@CEnginePrimitiveRenderer@@QAEXW4EEngineVideoOptionQuality@@ABVCEngineVideoOptionsDef@@@Z 0x00B32010
namespace NEngineInternal { extern CEnginePrimitiveRenderer* GPPrimitiveRenderer; }   // 0x01436E80 (the engine's primitive-renderer component; Codex name fix 2026-11-10)
// The primitive renderer is the shared engine/CEnginePrimitiveRenderer.h retail class (SetDrawDistanceQuality, PASS22_DRAWDISTANCE).
// NF479 (Codex pass 29 name fixes): ?GPLocalDetailGenerator@NEngineInternal@@3PAVCEngineLocalDetailGenerator@@A (raw Ego_r / FableWin /
//   Ego_d PDB publics; was the free g_pLocalDetailGenerator).
namespace NEngineInternal { extern CEngineLocalDetailGenerator*     GPLocalDetailGenerator; }  // 0x01436E88
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }       // 0x01436E98
namespace NEngineInternal { extern CEngine* GPEngine; }                // 0x01436EA4
namespace NEngineInternal { extern const CEngineVideoOptions* GPVideoOptions; }   // 0x01436EB0 (NF95 q11381: FableWin.pdb ?GPVideoOptions@NEngineInternal@@3PBVCEngineVideoOptions@@B)

bool CEngine::SetVideoOptions(const CEngineVideoOptions& options)
{
    if (!IsInitialised)
        return false;
    EnablePostDeviceReset = false;
    VideoOptions = options;
    NEngineInternal::GPVideoOptions = &VideoOptions;
    bool result = true;
    if (NEngineInternal::GPDisplayManager->SetDisplayMode(options.ScreenResolution, options.BitsPerPixel, options.Antialiasing, options.RefreshRate) != CDisplayManager::CREATE_DEVICE_SUCCESS) {
        result = false;
        if (NEngineInternal::GPDisplayManager->SetDisplayMode(options.ScreenResolution, options.BitsPerPixel, MULTISAMPLE_NONE, 0) == CDisplayManager::CREATE_DEVICE_FAILED) {
            C2DExtentsI resolution = options.ScreenResolution;
            if (resolution.X > 1024)
                resolution.X = 1024;
            if (resolution.Y > 768)
                resolution.Y = 768;
            if (NEngineInternal::GPDisplayManager->SetDisplayMode(resolution, options.BitsPerPixel, MULTISAMPLE_NONE, 0) == CDisplayManager::CREATE_DEVICE_FAILED) {
                EnablePostDeviceReset = true;
                return false;
            }
        }
    }
    NEngineInternal::GPDisplayManager->ChangePresentationInterval(!options.VerticalSync, 1, true);
    g_pShaderBankStateBlock->Set(PEngineVideoOptionsDefs[VideoOptions.EffectsQuality]);
    g_pColourFilter->SetEffectQuality(PEngineVideoOptionsDefs[VideoOptions.EffectsQuality]);
    g_pVideoOptionsListener->SetEffectQuality(PEngineVideoOptionsDefs[VideoOptions.EffectsQuality]);
    NEngineInternal::GPResourceManager->ResizeHiresTextureMemory(NEngineInternal::GPEngine->PEngineVideoOptionsDefs[VideoOptions.TextureDetail]->HiresTextureMemory);
    CEngineVideoOptionsDef* meshDef = NEngineInternal::GPEngine->PEngineVideoOptionsDefs[VideoOptions.MeshDetail];
    NEngineInternal::GPPrimitiveRenderer->SetDrawDistanceQuality(VideoOptions.MeshDetail, *meshDef);
    if (NEngineInternal::GPLocalDetailGenerator)
        NEngineInternal::GPLocalDetailGenerator->UpdateLoadingDistanceVideoOptions(*meshDef);
    NEngineInternal::GPPrimitiveOcclusionManager->StartNewScene();
    if (SmartRestoreComponentVideoMemory() != SMART_RESTORE_OK)
        result = false;
    EnablePostDeviceReset = true;
    return result;
}
