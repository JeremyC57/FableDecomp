#ifndef FABLE_ENGINE_CENGINELANDSCAPERENDERER_H
#define FABLE_ENGINE_CENGINELANDSCAPERENDERER_H
#ifdef FABLE_RETAIL_CENGINELANDSCAPERENDERER
// Retail opt-in (chunk 723): CEngineLandscapeRenderer : CEngineComponent (RTTI), vtable 0x012A2B54, sizeof 0x6B4 (operator new 0x6B4).
// Members: Ego_r.pdb names at the retail offsets, typed so that the destructor 0x00B67920 destroys them in retail's reverse order.
// Two corrections to the default model below, both from that destructor: GPUUpdateQueue is 4 bytes (+0x0C, no TaskList) and
// MeshBuilder starts at +0x10 (0x488 bytes; `lea ecx,[esi+10h] ; call ~CEngineLandscapeMeshBuilder 0x00BE2B20`). Nested structs:
// CLandscapeSettings (U, ?GetSettings@CEngineLandscapeRenderer@@QAEABUCLandscapeSettings@1@XZ), CSubPatchSorterEntry /
// CBackgroundLoadingOcclusionBoxTask (U, 8 bytes, Ego_r bitfields), CFastForegroundTextureLoadInfo (three CArrays, retail 0x24).
#ifndef FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT
#endif
#ifndef FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CINDEXBUFFER
#endif
#ifndef FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CVERTEXSHADER
#endif
#ifndef FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE
#endif
#ifndef FABLE_RETAIL_CLANDSCAPEPATCHTESSELATOR
#define FABLE_RETAIL_CLANDSCAPEPATCHTESSELATOR
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CArray.h"
#include "engine/CEngineComponent.h"
#include "engine/CLandscapeGPUUpdateQueue.h"
#include "engine/CEngineLandscapeMeshBuilder.h"
#include "engine/CIndexBuffer.h"
#include "engine/CTexture.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CLandscapePatchTesselator.h"
class CEngineLandscapeMemoryManager;
class CEngineMemoryInit;
class CLandscapeBackgroundPatch;
class CLandscapeBackgroundTreeNode;
enum EWaterType;                                                         // MS forward enum
struct CLandscapeTheme {                                                 // Ego_r.pdb, 0x38 bytes
    long       BaseTexture;                                              // +0x00
    long       BackgroundBaseTexture;                                    // +0x04
    long       CliffTexture;                                             // +0x08
    long       BackgroundCliffTexture;                                   // +0x0c
    long       BaseBumpMap;                                              // +0x10
    long       CliffBumpMap;                                             // +0x14
    long       BaseTextureMinMipMap;                                     // +0x18
    long       CliffTextureMinMipMap;                                    // +0x1c
    long       BaseBumpMapMinMipMap;                                     // +0x20
    long       CliffBumpMapMinMipMap;                                    // +0x24
    float      BaseSelfIllumination;                                     // +0x28
    float      CliffSelfIllumination;                                    // +0x2c
    EWaterType WaterType;                                                // +0x30
    float      WaterHeight;                                              // +0x34
};
class CEngineLandscapeRenderer : public CEngineComponent {
public:
    struct CLandscapeSettings {                                          // Ego_r.pdb, 0x4c bytes
        CLandscapeSettings();                                            // ??0CLandscapeSettings@CEngineLandscapeRenderer@@QAE@XZ
        float         MeshDetailLevel;                                   // +0x00
        float         ProceduralTextureDetail;                           // +0x04
        bool          DrawForeground;                                    // +0x08
        bool          DrawBackground;                                    // +0x09
        bool          DrawBackgroundWater;                               // +0x0a
        float         ForegroundFadeStart;                               // +0x0c
        float         ForegroundFadeEnd;                                 // +0x10
        unsigned char ForegroundFadeTransform[0x10];                     // +0x14 C4DVector
        unsigned char BackgroundFadeTransform[0x10];                     // +0x24 C4DVector
        bool          EnableUpdate;                                      // +0x34
        bool          FogEnable;                                         // +0x35
        bool          StaticMapOcclusionEnable;                          // +0x36
        bool          BumpMappingEnabled;                                // +0x37
        float         LODUpdateMaxTime;                                  // +0x38
        float         ForegroundMaxPixelsPerTexel;                       // +0x3c
        bool          EnableForegroundWithoutGameMap;                    // +0x40
        bool          DrawDebugDisplay;                                  // +0x41
        long          MinimumFileBlockSize;                              // +0x44
        bool          UpdateTesselation;                                 // +0x48
    };
    struct CSubPatchSorterEntry {                                        // Ego_r.pdb, 8 bytes
        CLandscapeBackgroundPatch* Patch;                                // +0x00
        long AddedThisFrame : 1;                                         // +0x04
        long IsOcclusionBox : 1;
        long OcclusionID : 30;
    };
    struct CBackgroundLoadingOcclusionBoxTask {                          // Ego_r.pdb, 8 bytes
        CLandscapeBackgroundTreeNode* Node;                              // +0x00
        long Visible : 1;                                                // +0x04
        long VisibilityFlagValid : 1;
        long OcclusionID : 30;
    };
    class CFastForegroundTextureLoadInfo {                               // Ego_r.pdb; retail 0x24 (three 12-byte CArrays)
    public:
        CFastForegroundTextureLoadInfo();                                // ??0CFastForegroundTextureLoadInfo@CEngineLandscapeRenderer@@QAE@XZ
        CArray<unsigned long> TexturesUsedThisFrame;                     // +0x00
        CArray<float>         TextureIDToMinPatchDistanceSquared;        // +0x0c
        CArray<unsigned char> CurrentTextureLODLevels;                   // +0x18
    };
    CEngineLandscapeRenderer(const CEngineMemoryInit& init);             // ??0CEngineLandscapeRenderer@@QAE@ABVCEngineMemoryInit@@@Z
    virtual ~CEngineLandscapeRenderer();                                 // ??1CEngineLandscapeRenderer@@UAE@XZ 0x00B67920
    virtual unsigned long PeekSceneFilterFlags() const;                  // ?PeekSceneFilterFlags@CEngineLandscapeRenderer@@UBEKXZ (chunk 731)
#ifdef FABLE_RETAIL_CENGINELANDSCAPERENDERER_ZSORT
    // Nested opt-in (chunk 1016, row 0x00B67E30): Ego_r.pdb / FableWin.pdb
    // ?RegisterSubPatchForZSorting@CEngineLandscapeRenderer@@QAEXAAJPAVCLandscapeBackgroundPatch@@@Z (out of line at 0x00B67E30; the
    // only caller 0x00BE5D20 passes the patch's slot word and the patch). A declaration only: no layout change.
    void RegisterSubPatchForZSorting(long& index, CLandscapeBackgroundPatch* patch);    // 0x00B67E30
#endif
    CLandscapeGPUUpdateQueue       GPUUpdateQueue;                       // +0x00c
    CEngineLandscapeMeshBuilder    MeshBuilder;                          // +0x010
    float                          LODZBandsZ[8];                        // +0x498
    CArray<CLandscapeTheme>        LandscapeThemes;                      // +0x4b8
    CVertexShader                  BackgroundVertexShader;               // +0x4c4
    CVertexShader                  BackgroundAlphaVertexShader;          // +0x4cc
    CVertexShader                  ForegroundBlackoutVertexShader;       // +0x4d4
    CArray<CVertexShader>          ForegroundVertexShaders;              // +0x4dc
    CArray<CVertexShader>          ForegroundSpotLightVertexShaders;     // +0x4e8
    CArray<CVertexShader>          ForegroundDepthBufferShadowedVertexShaders;      // +0x4f4
    CArray<CVertexShader>          ForegroundBumpDepthBufferShadowedVertexShaders;  // +0x500
    CArray<CVertexShader>          ForegroundSpotDepthBufferShadowedVertexShaders;  // +0x50c
    CVertexShader                  ProcTextureVertexShader;              // +0x518
    CArray<CVertexShader>          ForegroundColourBufferShadowedVertexShaders;     // +0x520
    CArray<CVertexShader>          ForegroundBumpColourBufferShadowedVertexShaders; // +0x52c
    CArray<CVertexShader>          ForegroundSpotColourBufferShadowedVertexShaders; // +0x538
    CArray<CVertexShader>*         ForegroundShadowedVertexShaders;      // +0x544
    CArray<CVertexShader>*         ForegroundBumpShadowedVertexShaders;  // +0x548
    CArray<CVertexShader>*         ForegroundSpotShadowedVertexShaders;  // +0x54c
    CVertexShader                  ShadowProjectionVertexShader;         // +0x550
    CVertexShader                  ShadowRenderVertexShader;             // +0x558
    CPixelShader                   BackgroundPixelShader;                // +0x560
    CPixelShader                   ForegroundPixelShader;                // +0x568
    CPixelShader                   ForegroundBlackoutPixelShader;        // +0x570
    CPixelShader                   ForegroundDepthBufferShadowedPixelShader;        // +0x578
    CPixelShader                   ForegroundBumpDepthBufferShadowedPixelShader;    // +0x580
    CPixelShader                   ForegroundColourBufferShadowedPixelShader;       // +0x588
    CPixelShader                   ForegroundBumpColourBufferShadowedPixelShader;   // +0x590
    CPixelShader                   ProcTexturePixelShader;               // +0x598
    CPixelShader*                  ForegroundShadowedPixelShader;        // +0x5a0
    CPixelShader*                  ForegroundBumpShadowedPixelShader;    // +0x5a4
    CIndexBuffer                   ForegroundBasePatchIndexBuffer;       // +0x5a8
    CTexture                       ForegroundBlendTables[5];             // +0x5bc
    CArray<CSubPatchSorterEntry>   SortedSubPatchList;                   // +0x5e4
    CArray<CBackgroundLoadingOcclusionBoxTask> BackgroundLoadingOcclusionTests;     // +0x5f0
    CArray<long>                   FreeBackgroundLoadingOcclusionHandles;           // +0x5fc
    CLandscapeSettings             Settings;                             // +0x608
    CArray<unsigned short>         TesselationOverflowIndexBuffer;       // +0x654
    CArray<CTVertexLandscapeBackground> TesselationOverflowVertexBuffer; // +0x660
    CLandscapePatchTesselator      Tesselator;                           // +0x66c
    CFastForegroundTextureLoadInfo FastForegroundTextureLoadInfo;        // +0x68c
    CEngineLandscapeMemoryManager* MemoryManager;                        // +0x6b0
};
FABLE_STATIC_ASSERT(sizeof(CEngineLandscapeRenderer) == 0x6b4);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, MeshBuilder) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, LODZBandsZ) == 0x498);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, BackgroundVertexShader) == 0x4c4);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundShadowedVertexShaders) == 0x544);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundBasePatchIndexBuffer) == 0x5a8);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundBlendTables) == 0x5bc);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, Settings) == 0x608);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, Tesselator) == 0x66c);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, FastForegroundTextureLoadInfo) == 0x68c);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, MemoryManager) == 0x6b0);
#else // flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineLandscapeRenderer = 0xf5c.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CEngineLandscapeRenderer = 0x6b4.
// TLC drift (stl_layout.RESIZE): MeshBuilder is 0x484 bytes, not the STLport-relaid 0xcc0 -- operator new 0x6B4 and the landed
//   models of 0x00B677A0/0x00BF5CA0/0x00B6B0B0/0x00B68610/0x00BE5D80 put every later member 0x83C lower.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CEngineLandscapeMemoryManager;
class CPixelShader;

#pragma pack(push, 1)
class CEngineLandscapeRenderer {
public:
    void*                          __vftable;                // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char                  _pad_0x04[0x4];           // +0x04
    unsigned char                  _pad_0x08[0x4];           // +0x08
    unsigned char                  GPUUpdateQueue[0x8];      // +0x0c CLandscapeGPUUpdateQueue
    unsigned char                  MeshBuilder[0x484];       // +0x14 CEngineLandscapeMeshBuilder
    float                          LODZBandsZ[8];            // +0x498
    unsigned char                  LandscapeThemes[0xc];     // +0x4b8 CArray<CLandscapeTheme>
    unsigned char                  BackgroundVertexShader[0x8]; // +0x4c4 CVertexShader
    unsigned char                  BackgroundAlphaVertexShader[0x8]; // +0x4cc CVertexShader
    unsigned char                  ForegroundBlackoutVertexShader[0x8]; // +0x4d4 CVertexShader
    unsigned char                  ForegroundVertexShaders[0xc];  // +0x4dc CArray<CVertexShader>
    unsigned char                  ForegroundSpotLightVertexShaders[0xc];  // +0x4e8 CArray<CVertexShader>
    unsigned char                  ForegroundDepthBufferShadowedVertexShaders[0xc];  // +0x4f4 CArray<CVertexShader>
    unsigned char                  ForegroundBumpDepthBufferShadowedVertexShaders[0xc];  // +0x500 CArray<CVertexShader>
    unsigned char                  ForegroundSpotDepthBufferShadowedVertexShaders[0xc];  // +0x50c CArray<CVertexShader>
    unsigned char                  ProcTextureVertexShader[0x8]; // +0x518 CVertexShader
    unsigned char                  ForegroundColourBufferShadowedVertexShaders[0xc];  // +0x520 CArray<CVertexShader>
    unsigned char                  ForegroundBumpColourBufferShadowedVertexShaders[0xc];  // +0x52c CArray<CVertexShader>
    unsigned char                  ForegroundSpotColourBufferShadowedVertexShaders[0xc];  // +0x538 CArray<CVertexShader>
    void*                          ForegroundShadowedVertexShaders; // +0x544 CArray<CVertexShader> *
    void*                          ForegroundBumpShadowedVertexShaders; // +0x548 CArray<CVertexShader> *
    void*                          ForegroundSpotShadowedVertexShaders; // +0x54c CArray<CVertexShader> *
    unsigned char                  ShadowProjectionVertexShader[0x8]; // +0x550 CVertexShader
    unsigned char                  ShadowRenderVertexShader[0x8]; // +0x558 CVertexShader
    unsigned char                  BackgroundPixelShader[0x8]; // +0x560 CPixelShader
    unsigned char                  ForegroundPixelShader[0x8]; // +0x568 CPixelShader
    unsigned char                  ForegroundBlackoutPixelShader[0x8]; // +0x570 CPixelShader
    unsigned char                  ForegroundDepthBufferShadowedPixelShader[0x8]; // +0x578 CPixelShader
    unsigned char                  ForegroundBumpDepthBufferShadowedPixelShader[0x8]; // +0x580 CPixelShader
    unsigned char                  ForegroundColourBufferShadowedPixelShader[0x8]; // +0x588 CPixelShader
    unsigned char                  ForegroundBumpColourBufferShadowedPixelShader[0x8]; // +0x590 CPixelShader
    unsigned char                  ProcTexturePixelShader[0x8]; // +0x598 CPixelShader
    CPixelShader*                  ForegroundShadowedPixelShader; // +0x5a0
    CPixelShader*                  ForegroundBumpShadowedPixelShader; // +0x5a4
    unsigned char                  ForegroundBasePatchIndexBuffer[0x14]; // +0x5a8 CIndexBuffer
    unsigned char                  ForegroundBlendTables[0x28]; // +0x5bc CTexture[5]
    unsigned char                  SortedSubPatchList[0xc];  // +0x5e4 CArray<CEngineLandscapeRenderer::CSubPatchSorterEntry>
    unsigned char                  BackgroundLoadingOcclusionTests[0xc];  // +0x5f0 CArray<CEngineLandscapeRenderer::CBackgroundLoadingOcclusionBoxTask>
    unsigned char                  FreeBackgroundLoadingOcclusionHandles[0xc];  // +0x5fc CArray<long>
    unsigned char                  Settings[0x4c];           // +0x608 CLandscapeSettings
    unsigned char                  TesselationOverflowIndexBuffer[0xc];  // +0x654 CArray<unsigned_short>
    unsigned char                  TesselationOverflowVertexBuffer[0xc];  // +0x660 CArray<CTVertexLandscapeBackground>
    unsigned char                  Tesselator[0x20];         // +0x66c CLandscapePatchTesselator
    unsigned char                  FastForegroundTextureLoadInfo[0x24]; // +0x68c CFastForegroundTextureLoadInfo
    CEngineLandscapeMemoryManager* MemoryManager;            // +0x6b0
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineLandscapeRenderer) == 0x6b4);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, GPUUpdateQueue) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, MeshBuilder) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, LODZBandsZ) == 0x498);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, LandscapeThemes) == 0x4b8);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, BackgroundVertexShader) == 0x4c4);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, BackgroundAlphaVertexShader) == 0x4cc);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundBlackoutVertexShader) == 0x4d4);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundVertexShaders) == 0x4dc);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundSpotLightVertexShaders) == 0x4e8);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundDepthBufferShadowedVertexShaders) == 0x4f4);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundBumpDepthBufferShadowedVertexShaders) == 0x500);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundSpotDepthBufferShadowedVertexShaders) == 0x50c);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ProcTextureVertexShader) == 0x518);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundColourBufferShadowedVertexShaders) == 0x520);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundBumpColourBufferShadowedVertexShaders) == 0x52c);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundSpotColourBufferShadowedVertexShaders) == 0x538);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundShadowedVertexShaders) == 0x544);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundBumpShadowedVertexShaders) == 0x548);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundSpotShadowedVertexShaders) == 0x54c);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ShadowProjectionVertexShader) == 0x550);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ShadowRenderVertexShader) == 0x558);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, BackgroundPixelShader) == 0x560);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundPixelShader) == 0x568);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundBlackoutPixelShader) == 0x570);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundDepthBufferShadowedPixelShader) == 0x578);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundBumpDepthBufferShadowedPixelShader) == 0x580);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundColourBufferShadowedPixelShader) == 0x588);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundBumpColourBufferShadowedPixelShader) == 0x590);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ProcTexturePixelShader) == 0x598);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundShadowedPixelShader) == 0x5a0);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundBumpShadowedPixelShader) == 0x5a4);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundBasePatchIndexBuffer) == 0x5a8);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, ForegroundBlendTables) == 0x5bc);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, SortedSubPatchList) == 0x5e4);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, BackgroundLoadingOcclusionTests) == 0x5f0);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, FreeBackgroundLoadingOcclusionHandles) == 0x5fc);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, Settings) == 0x608);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, TesselationOverflowIndexBuffer) == 0x654);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, TesselationOverflowVertexBuffer) == 0x660);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, Tesselator) == 0x66c);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, FastForegroundTextureLoadInfo) == 0x68c);
FABLE_STATIC_ASSERT(offsetof(CEngineLandscapeRenderer, MemoryManager) == 0x6b0);
#endif // FABLE_RETAIL_CENGINELANDSCAPERENDERER

#endif // FABLE_ENGINE_CENGINELANDSCAPERENDERER_H
