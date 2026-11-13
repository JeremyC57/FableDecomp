#ifndef FABLE_ENGINE_CENGINEWATERRENDERER_H
#define FABLE_ENGINE_CENGINEWATERRENDERER_H
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER
// Retail opt-in (chunk 723): the polymorphic retail class. RTTI .?AVCEngineWaterRenderer@@ : CEngineComponent; vtable 0x012A3364:
// slot 0 ??_G 0x00B7EE60, 1 FrameStart 0x00B71FB0, 2 FrameEnd 0x00B6D500, 4 Render 0x00B783F0, 7 WorldFrameUpdate 0x00B6D6D0,
// 9 GetConsoleEnableFunctionName 0x00B7ED80, 10 PeekSceneFilterFlags 0x00B7ED70, 13 SetEffectQuality 0x00B6D6A0 (the other slots
// are CEngineComponent's own bodies). Members: Ego_r.pdb names at the retail offsets of the default model below (12-byte STLport
// CArrays), typed: the destructor 0x00B71880 destroys them in exactly this reverse order (CCountedPointer pair, CCharString,
// both tesselators, Settings, two CCharStrings, ten render lists, the shader members and the three CArray<CVertexShader>, the two
// vertex and three index buffers) and ends in ~CEngineComponent 0x00B59730. Settings: engine/CEngineWaterSettings.h
// (WorldFrameUpdate 0x00B6D6D0 = `add ecx,280h ; jmp CEngineWaterSettings::WorldFrameUpdate`). Functions: Ego_r.pdb / FableWin.pdb.
// Needs FABLE_RETAIL_CENGINECOMPONENT / CINDEXBUFFER / CVERTEXBUFFERWIN32 / CVERTEXSHADER (switched on here) and the new
// CEngineWaterSettings / CPixelShader / CWaterPatchTesselatorBase headers.
#ifndef FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT
#endif
#ifndef FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CINDEXBUFFER
#endif
#ifndef FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#endif
#ifndef FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CVERTEXSHADER
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DBoundingBox.h"
#include "engine/CEngineComponent.h"
#include "engine/CIndexBuffer.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CEngineWaterSettings.h"
#include "engine/CWaterPatchTesselatorBase.h"
class CBankFileAsyncData;
class CBankFileAsyncEntry;
class CStaticMapBankFile;
class CTexture;
class CWaterGenerator;
class CWaterPatchMesh;
class CEngineWaterBackgroundSubPatch;
class CEngineInit;
class CEngineComponentParams;
class CEngineComponentRenderParams;
class CEngineVideoOptionsDef;
class CTVertexWaterBackground { public: unsigned char Bytes[0x38]; };   // GetSizeOfVertex 0x00B7EC30 returns 0x38 (layout not modelled)
class CTVertexSeaBackground { public: unsigned char Bytes[0xc]; };      // GetSizeOfVertex 0x00B7ECE0 returns 0xC (layout not modelled)
class CEngineWaterRenderer : public CEngineComponent {
public:
    CEngineWaterRenderer(const CEngineInit& init);                       // ??0CEngineWaterRenderer@@QAE@ABVCEngineInit@@@Z
    virtual ~CEngineWaterRenderer();                                     // slot 0 (??_G 0x00B7EE60; ??1 0x00B71880)
    virtual void FrameStart(const CEngineComponentParams& params);       // slot 1  0x00B71FB0
    virtual void FrameEnd(const CEngineComponentParams& params);         // slot 2  0x00B6D500
    virtual void Render(const CEngineComponentRenderParams& params);     // slot 4  0x00B783F0
    virtual void WorldFrameUpdate(bool force);                           // slot 7  0x00B6D6D0
    virtual CCharString GetConsoleEnableFunctionName();                  // slot 9  0x00B7ED80
    virtual unsigned long PeekSceneFilterFlags() const;                  // slot 10 0x00B7ED70
    virtual bool SetEffectQuality(const CEngineVideoOptionsDef& def);    // slot 13 0x00B6D6A0
    void ReleaseSeaBuffers();                                            // 0x00B6D480 ?ReleaseSeaBuffers@CEngineWaterRenderer@@QAEXXZ
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_ADDTORENDERLIST
    // Opt-in (wf3, row 0x00BE7BE0 CLandscapeBackgroundPatch::Render): FableWin.pdb
    // ?AddMidgroundSeaPatchToRenderList@CEngineWaterRenderer@@QAEXQAVCEngineWaterBackgroundSubPatch@@_N@Z and
    // ?AddBackgroundWaterPatchToRenderList@CEngineWaterRenderer@@QAEXQAVCEngineWaterBackgroundSubPatch@@_N@Z (public, by-value pointer
    // + bool); retail expands both in their only caller 0x00BE7BE0. Declarations here; the inline bodies follow the class.
    void AddMidgroundSeaPatchToRenderList(CEngineWaterBackgroundSubPatch* const patch, bool alphaed);
    void AddBackgroundWaterPatchToRenderList(CEngineWaterBackgroundSubPatch* const patch, bool alphaed);
#endif
private:
    void BuildConstantBuffers();                                         // 0x00B6EDF0 ?BuildConstantBuffers@CEngineWaterRenderer@@AAEXXZ
    void DeleteConstantBuffers();                                        // 0x00B6D4B0 ?DeleteConstantBuffers@CEngineWaterRenderer@@AAEXXZ
    void UninitialiseDynamicTextures();                                  // ?UninitialiseDynamicTextures@CEngineWaterRenderer@@AAEXXZ
    void FinishUsingDynamicTextures();                                   // 0x00B6D630 ?FinishUsingDynamicTextures@CEngineWaterRenderer@@AAEXXZ
    void RenderSeaBackground(float time);                                // ?RenderSeaBackground@CEngineWaterRenderer@@AAEXM@Z
    void RenderEnvironmentMap();                                         // ?RenderEnvironmentMap@CEngineWaterRenderer@@AAEXXZ
    void RenderIceForeground(float time);                                // ?RenderIceForeground@CEngineWaterRenderer@@AAEXM@Z
    void RenderForeground(float time);                                   // ?RenderForeground@CEngineWaterRenderer@@AAEXM@Z
    void RenderSeaReflectiveForeground(float time);                      // ?RenderSeaReflectiveForeground@CEngineWaterRenderer@@AAEXM@Z
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_RENDER
    // Opt-in (bwf2, row 0x00B783F0 Render): Ego_r.pdb private members called by Render, and the FableWin.pdb inline
    // ?SetCommonRenderStates@CEngineWaterRenderer@@AAEXXZ (0x02D4CC20 debug; no retail copy, expanded three times by Render).
    void RenderBackground(float time);                                   // 0x00B76BC0
    void RenderIceBackground(float time);                                // 0x00B75CA0
    void RenderSeaNonReflectiveForeground(float time);                   // 0x00B77E20
    bool CreateDynamicTextures(bool water, bool sea, bool ice);          // 0x00B72180 ?CreateDynamicTextures@CEngineWaterRenderer@@AAE_N_N00@Z
    void SetCommonRenderStates();
#endif
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_SEAMIDGROUND
    // Opt-in (chunk 892, row 0x00B6FA40): Ego_r.pdb / FableWin.pdb ?RenderSeaMidground@CEngineWaterRenderer@@AAEXM@Z and
    // ?RenderBackgroundList@CEngineWaterRenderer@@AAEXAAV?$CArray@PAVCEngineWaterBackgroundSubPatch@@@@M@Z (landed 0x00B6E260).
    void RenderSeaMidground(float time);                                 // 0x00B6FA40
    void RenderBackgroundList(CArray<CEngineWaterBackgroundSubPatch*>& list, float time);   // 0x00B6E260
#endif
public:
    CIndexBuffer         ForegroundIndexBuffers;                         // +0x00c
    CIndexBuffer         ForegroundTextureIndexBuffers;                  // +0x020
    CVertexBufferWin32   ForegroundTextureVertexBuffers;                 // +0x034
    CIndexBuffer         SeaBackgroundIndexBuffer;                       // +0x068
    CVertexBufferWin32   SeaBackgroundVertexBuffer;                      // +0x07c
    long                 SeaBackgroundVertexCount;                       // +0x0b0
    long                 SeaBackgroundPolyCount;                         // +0x0b4
    long                 WaterForegroundPolyCount;                       // +0x0b8
    CVertexShader        SeaBackgroundVertexShader;                      // +0x0bc
    CPixelShader         SeaBackgroundPixelShader;                       // +0x0c4
    CVertexShader        SeaForegroundReflectiveVertexShader;            // +0x0cc
    CPixelShader         SeaForegroundReflectivePixelShader;             // +0x0d4
    CVertexShader        SeaForegroundNonReflectiveVertexShader;         // +0x0dc
    CPixelShader         SeaForegroundNonReflectivePixelShader;          // +0x0e4
    CVertexShader        ForegroundVertexShader;                         // +0x0ec
    CPixelShader         ForegroundPixelShader;                          // +0x0f4
    CPixelShader         RefractionForegroundPixelShader;                // +0x0fc
    CVertexShader        BackgroundVertexShader;                         // +0x104
    CPixelShader         BackgroundPixelShader;                          // +0x10c
    CVertexShader        SeaBackgroundLowEndVertexShader;                // +0x114
    CVertexShader        WaterBackgroundLowEndVertexShader;              // +0x11c
    CPixelShader         WaterLowEndPixelShader;                         // +0x124
    CArray<CVertexShader> IceForegroundVertexShader;                     // +0x12c
    CArray<CVertexShader> IceForegroundShadowedVertexShader;             // +0x138
    CArray<CVertexShader> IceForegroundColbuffShadowedVertexShader;      // +0x144
    CVertexShader        IceBackgroundVertexShader;                      // +0x150
    CPixelShader         IceForegroundPixelShader;                       // +0x158
    CPixelShader         IceForegroundShadowedPixelShader;               // +0x160
    CPixelShader         IceForegroundColbuffShadowedPixelShader;        // +0x168
    CPixelShader         IceBackgroundPixelShader;                       // +0x170
    CVertexShader        BumpmapVertexShader;                            // +0x178
    CPixelShader         BumpmapPixelShader;                             // +0x180
    CVertexShader        IceBumpmapVertexShader;                         // +0x188
    CPixelShader         IceBumpmapPixelShader;                          // +0x190
    CVertexShader        BumpmapRainVertexShader;                        // +0x198
    CPixelShader         BumpmapRainPixelShader;                         // +0x1a0
    CVertexShader        EnvironmentMapVertexShader;                     // +0x1a8
    CPixelShader         EnvironmentMapPixelShader;                      // +0x1b0
    CVertexShader        SkyMapVertexShader;                             // +0x1b8
    CPixelShader         SkyMapPixelShader;                              // +0x1c0
    CVertexShader        EnvMapOverlayVertexShader;                      // +0x1c8
    CPixelShader         EnvMapOverlayPixelShader;                       // +0x1d0
    long                 DynamicTexturePageHandle;                       // +0x1d8
    long                 EnvironmentMapHandle;                           // +0x1dc
    CTexture*            EnvironmentMap;                                 // +0x1e0
    long                 SeaBumpMapHandle;                               // +0x1e4
    CTexture*            SeaBumpMap;                                     // +0x1e8
    long                 WaterBumpMapHandle;                             // +0x1ec
    CTexture*            WaterBumpMap;                                   // +0x1f0
    long                 IceBumpMapHandle;                               // +0x1f4
    CTexture*            IceBumpMap;                                     // +0x1f8
    CArray<CWaterPatchMesh*> ForegroundRenderList;                       // +0x1fc
    CArray<CEngineWaterBackgroundSubPatch*> BackgroundRenderList;        // +0x208
    CArray<CEngineWaterBackgroundSubPatch*> AlphaedBackgroundRenderList; // +0x214
    CArray<std::pair<C3DBoundingBox, CWaterPatchMesh*> > IceForegroundRenderList;   // +0x220
    CArray<CEngineWaterBackgroundSubPatch*> IceBackgroundRenderList;     // +0x22c
    CArray<CEngineWaterBackgroundSubPatch*> IceAlphaedBackgroundRenderList;   // +0x238
    CArray<CWaterPatchMesh*> SeaReflectiveForegroundRenderList;          // +0x244
    CArray<CWaterPatchMesh*> SeaNonReflectiveForegroundRenderList;       // +0x250
    CArray<CEngineWaterBackgroundSubPatch*> SeaMidgroundRenderList;      // +0x25c
    CArray<CEngineWaterBackgroundSubPatch*> SeaAlphaedMidgroundRenderList;   // +0x268
    bool                 HighendWaterEnabled;                            // +0x274
    bool                 HighendWaterSupported;                          // +0x275
    bool                 HaveBackgroundSea;                              // +0x276
    CCharString          LoadedOrLoadingSeaBodyID;                       // +0x278
    CCharString          RequestedSeaBodyID;                             // +0x27c
    CEngineWaterSettings Settings;                                       // +0x280
    CWaterPatchTesselatorBase<CTVertexWaterBackground> Tesselator;       // +0x564
    CWaterPatchTesselatorBase<CTVertexSeaBackground> SeaTesselator;      // +0x584
    CWaterGenerator*     WaterGenerator;                                 // +0x5a4
    CStaticMapBankFile*  StaticMapBankFile;                              // +0x5a8
    CCharString          StaticMapSeaEntryPrefix;                        // +0x5ac
    CCountedPointer<CBankFileAsyncEntry> LoadingSeaEntry;                // +0x5b0
    CCountedPointer<CBankFileAsyncData> LoadingSea;                      // +0x5b8
    float                WaterBumpFactor;                                // +0x5c0
};
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_ADDTORENDERLIST
// Opt-in bodies (wf3, row 0x00BE7BE0; names FableWin.pdb). Retail also holds an out-of-line copy of each, unreferenced and not in
// the manifest: 0x00BE9030 (sea helper, `ret 8`) and 0x00BE9080 (background helper with the sea helper expanded in it, `ret 8`);
// the bodies below follow those copies statement for statement (PolyCount test first in both, the 3/4/5 test, the type-8 test, four
// out-of-line push_back calls 0x00BE90FB..0x00BE9138). Needs FABLE_RETAIL_CARRAY_PUSHBACK (CArray<T>::push_back, out of line at 0x00BE8FF0) and the
// sub-patch accessors (switched on here; include this header before any other include of engine/CEngineWaterBackgroundSubPatch.h).
// Both take the sub-patch by value and pass its address to push_back(const T&): each expansion stores the pointer to its own copy
// (0x00BE7C2D and 0x00BE7C55, 0x00BE7D1F). The sea helper is two push_back statements, not a list select: retail cross-jumps the
// two expansions (0x00BE7C95 `add ecx,25Ch ; jmp 0BE7C61h`) and keeps the element size 4 in esi for both.
// FABLE_FORCEINLINE (owner decision 2026-11-08, inline budget): VC7.1's /Ob2 heuristic leaves both helpers out of line at a call
// with a non-constant `alphaed`; retail expanded them there (0x00BE7C37..0x00BE7D0D), so that TU's budget differed. Same bodies.
// FABLE_RETAIL_CENGINEWATERRENDERER_ADDTORENDERLIST_PLAIN_INLINE keeps them plain `inline` (for a TU that emits the out-of-line copies).
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_ADDTORENDERLIST_PLAIN_INLINE
#define FABLE_WATER_RENDER_LIST_INLINE inline
#else
#define FABLE_WATER_RENDER_LIST_INLINE FABLE_FORCEINLINE
#endif
#ifndef FABLE_RETAIL_CENGINEWATERBACKGROUNDSUBPATCH_ACCESSORS
#define FABLE_RETAIL_CENGINEWATERBACKGROUNDSUBPATCH_ACCESSORS
#endif
#include "engine/CEngineWaterBackgroundSubPatch.h"
FABLE_WATER_RENDER_LIST_INLINE void CEngineWaterRenderer::AddMidgroundSeaPatchToRenderList(CEngineWaterBackgroundSubPatch* const patch, bool alphaed)
{
    if (!patch->IsEmpty()) {
        if (alphaed)
            SeaAlphaedMidgroundRenderList.push_back(patch);
        else
            SeaMidgroundRenderList.push_back(patch);
    }
}
FABLE_WATER_RENDER_LIST_INLINE void CEngineWaterRenderer::AddBackgroundWaterPatchToRenderList(CEngineWaterBackgroundSubPatch* const patch, bool alphaed)
{
    if (!patch->IsEmpty()) {
        if (patch->IsSea())
            AddMidgroundSeaPatchToRenderList(patch, alphaed);
        else {
            if (patch->WaterType == 8) {                     // ice
                if (alphaed)
                    IceAlphaedBackgroundRenderList.push_back(patch);
                else
                    IceBackgroundRenderList.push_back(patch);
            } else {
                if (alphaed)
                    AlphaedBackgroundRenderList.push_back(patch);
                else
                    BackgroundRenderList.push_back(patch);
            }
        }
    }
}
#endif
FABLE_STATIC_ASSERT(sizeof(CEngineWaterRenderer) == 0x5c4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, ForegroundIndexBuffers) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaBackgroundVertexShader) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, IceForegroundVertexShader) == 0x12c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, DynamicTexturePageHandle) == 0x1d8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, ForegroundRenderList) == 0x1fc);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, HighendWaterEnabled) == 0x274);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, LoadedOrLoadingSeaBodyID) == 0x278);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, Settings) == 0x280);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, Tesselator) == 0x564);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaTesselator) == 0x584);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, StaticMapSeaEntryPrefix) == 0x5ac);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, LoadingSea) == 0x5b8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, WaterBumpFactor) == 0x5c0);
#else // flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineWaterRenderer = 0x608.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CEngineWaterRenderer = 0x5c4.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CBankFileAsyncData;
class CBankFileAsyncEntry;
class CCPPointerInfo;
class CCharStringData;
class CStaticMapBankFile;
class CTexture;
class CWaterGenerator;

#pragma pack(push, 1)
class CEngineWaterRenderer {
public:
    void*                __vftable;                          // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char        _pad_0x04[0x4];                     // +0x04
    unsigned char        _pad_0x08[0x4];                     // +0x08
    unsigned char        ForegroundIndexBuffers[0x14];       // +0x0c CIndexBuffer
    unsigned char        ForegroundTextureIndexBuffers[0x14]; // +0x20 CIndexBuffer
    unsigned char        ForegroundTextureVertexBuffers[0x34]; // +0x34 CVertexBufferWin32
    unsigned char        SeaBackgroundIndexBuffer[0x14];     // +0x68 CIndexBuffer
    unsigned char        SeaBackgroundVertexBuffer[0x34];    // +0x7c CVertexBufferWin32
    long                 SeaBackgroundVertexCount;           // +0xb0
    long                 SeaBackgroundPolyCount;             // +0xb4
    long                 WaterForegroundPolyCount;           // +0xb8
    unsigned char        SeaBackgroundVertexShader[0x8];     // +0xbc CVertexShader
    unsigned char        SeaBackgroundPixelShader[0x8];      // +0xc4 CPixelShader
    unsigned char        SeaForegroundReflectiveVertexShader[0x8]; // +0xcc CVertexShader
    unsigned char        SeaForegroundReflectivePixelShader[0x8]; // +0xd4 CPixelShader
    unsigned char        SeaForegroundNonReflectiveVertexShader[0x8]; // +0xdc CVertexShader
    unsigned char        SeaForegroundNonReflectivePixelShader[0x8]; // +0xe4 CPixelShader
    unsigned char        ForegroundVertexShader[0x8];        // +0xec CVertexShader
    unsigned char        ForegroundPixelShader[0x8];         // +0xf4 CPixelShader
    unsigned char        RefractionForegroundPixelShader[0x8]; // +0xfc CPixelShader
    unsigned char        BackgroundVertexShader[0x8];        // +0x104 CVertexShader
    unsigned char        BackgroundPixelShader[0x8];         // +0x10c CPixelShader
    unsigned char        SeaBackgroundLowEndVertexShader[0x8]; // +0x114 CVertexShader
    unsigned char        WaterBackgroundLowEndVertexShader[0x8]; // +0x11c CVertexShader
    unsigned char        WaterLowEndPixelShader[0x8];        // +0x124 CPixelShader
    unsigned char        IceForegroundVertexShader[0xc];     // +0x12c CArray<CVertexShader>
    unsigned char        IceForegroundShadowedVertexShader[0xc];  // +0x138 CArray<CVertexShader>
    unsigned char        IceForegroundColbuffShadowedVertexShader[0xc];  // +0x144 CArray<CVertexShader>
    unsigned char        IceBackgroundVertexShader[0x8];     // +0x150 CVertexShader
    unsigned char        IceForegroundPixelShader[0x8];      // +0x158 CPixelShader
    unsigned char        IceForegroundShadowedPixelShader[0x8]; // +0x160 CPixelShader
    unsigned char        IceForegroundColbuffShadowedPixelShader[0x8]; // +0x168 CPixelShader
    unsigned char        IceBackgroundPixelShader[0x8];      // +0x170 CPixelShader
    unsigned char        BumpmapVertexShader[0x8];           // +0x178 CVertexShader
    unsigned char        BumpmapPixelShader[0x8];            // +0x180 CPixelShader
    unsigned char        IceBumpmapVertexShader[0x8];        // +0x188 CVertexShader
    unsigned char        IceBumpmapPixelShader[0x8];         // +0x190 CPixelShader
    unsigned char        BumpmapRainVertexShader[0x8];       // +0x198 CVertexShader
    unsigned char        BumpmapRainPixelShader[0x8];        // +0x1a0 CPixelShader
    unsigned char        EnvironmentMapVertexShader[0x8];    // +0x1a8 CVertexShader
    unsigned char        EnvironmentMapPixelShader[0x8];     // +0x1b0 CPixelShader
    unsigned char        SkyMapVertexShader[0x8];            // +0x1b8 CVertexShader
    unsigned char        SkyMapPixelShader[0x8];             // +0x1c0 CPixelShader
    unsigned char        EnvMapOverlayVertexShader[0x8];     // +0x1c8 CVertexShader
    unsigned char        EnvMapOverlayPixelShader[0x8];      // +0x1d0 CPixelShader
    long                 DynamicTexturePageHandle;           // +0x1d8
    long                 EnvironmentMapHandle;               // +0x1dc
    CTexture*            EnvironmentMap;                     // +0x1e0
    long                 SeaBumpMapHandle;                   // +0x1e4
    CTexture*            SeaBumpMap;                         // +0x1e8
    long                 WaterBumpMapHandle;                 // +0x1ec
    CTexture*            WaterBumpMap;                       // +0x1f0
    long                 IceBumpMapHandle;                   // +0x1f4
    CTexture*            IceBumpMap;                         // +0x1f8
    unsigned char        ForegroundRenderList[0xc];          // +0x1fc CArray<CWaterPatchMesh_*>
    unsigned char        BackgroundRenderList[0xc];          // +0x208 CArray<CEngineWaterBackgroundSubPatch_*>
    unsigned char        AlphaedBackgroundRenderList[0xc];   // +0x214 CArray<CEngineWaterBackgroundSubPatch_*>
    unsigned char        IceForegroundRenderList[0xc];       // +0x220 CArray<std::pair<C3DBoundingBox,CWaterPatchMesh_*>_>
    unsigned char        IceBackgroundRenderList[0xc];       // +0x22c CArray<CEngineWaterBackgroundSubPatch_*>
    unsigned char        IceAlphaedBackgroundRenderList[0xc];  // +0x238 CArray<CEngineWaterBackgroundSubPatch_*>
    unsigned char        SeaReflectiveForegroundRenderList[0xc];  // +0x244 CArray<CWaterPatchMesh_*>
    unsigned char        SeaNonReflectiveForegroundRenderList[0xc];  // +0x250 CArray<CWaterPatchMesh_*>
    unsigned char        SeaMidgroundRenderList[0xc];        // +0x25c CArray<CEngineWaterBackgroundSubPatch_*>
    unsigned char        SeaAlphaedMidgroundRenderList[0xc];  // +0x268 CArray<CEngineWaterBackgroundSubPatch_*>
    bool                 HighendWaterEnabled;                // +0x274
    bool                 HighendWaterSupported;              // +0x275
    bool                 HaveBackgroundSea;                  // +0x276
    unsigned char        _pad_0x277[0x1];                    // +0x277
    CCharStringData*     LoadedOrLoadingSeaBodyID_PStringData; // +0x278 CCharString::PStringData
    CCharStringData*     RequestedSeaBodyID_PStringData;     // +0x27c CCharString::PStringData
    unsigned char        Settings[0x2e4];                    // +0x280 CEngineWaterSettings
    unsigned char        Tesselator[0x20];                   // +0x564 CWaterPatchTesselatorBase<CTVertexWaterBackground>
    unsigned char        SeaTesselator[0x20];                // +0x584 CWaterPatchTesselatorBase<CTVertexSeaBackground>
    CWaterGenerator*     WaterGenerator;                     // +0x5a4
    CStaticMapBankFile*  StaticMapBankFile;                  // +0x5a8
    CCharStringData*     StaticMapSeaEntryPrefix_PStringData; // +0x5ac CCharString::PStringData
    CBankFileAsyncEntry* LoadingSeaEntry_Data;               // +0x5b0 CCountedPointer<CBankFileAsyncEntry>::Data
    CCPPointerInfo*      LoadingSeaEntry_Info;               // +0x5b4 CCountedPointer<CBankFileAsyncEntry>::Info
    CBankFileAsyncData*  LoadingSea_Data;                    // +0x5b8 CCountedPointer<CBankFileAsyncData>::Data
    CCPPointerInfo*      LoadingSea_Info;                    // +0x5bc CCountedPointer<CBankFileAsyncData>::Info
    float                WaterBumpFactor;                    // +0x5c0
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineWaterRenderer) == 0x5c4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, ForegroundIndexBuffers) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, ForegroundTextureIndexBuffers) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, ForegroundTextureVertexBuffers) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaBackgroundIndexBuffer) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaBackgroundVertexBuffer) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaBackgroundVertexCount) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaBackgroundPolyCount) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, WaterForegroundPolyCount) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaBackgroundVertexShader) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaBackgroundPixelShader) == 0xc4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaForegroundReflectiveVertexShader) == 0xcc);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaForegroundReflectivePixelShader) == 0xd4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaForegroundNonReflectiveVertexShader) == 0xdc);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaForegroundNonReflectivePixelShader) == 0xe4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, ForegroundVertexShader) == 0xec);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, ForegroundPixelShader) == 0xf4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, RefractionForegroundPixelShader) == 0xfc);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, BackgroundVertexShader) == 0x104);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, BackgroundPixelShader) == 0x10c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaBackgroundLowEndVertexShader) == 0x114);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, WaterBackgroundLowEndVertexShader) == 0x11c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, WaterLowEndPixelShader) == 0x124);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, IceForegroundVertexShader) == 0x12c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, IceForegroundShadowedVertexShader) == 0x138);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, IceForegroundColbuffShadowedVertexShader) == 0x144);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, IceBackgroundVertexShader) == 0x150);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, IceForegroundPixelShader) == 0x158);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, IceForegroundShadowedPixelShader) == 0x160);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, IceForegroundColbuffShadowedPixelShader) == 0x168);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, IceBackgroundPixelShader) == 0x170);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, BumpmapVertexShader) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, BumpmapPixelShader) == 0x180);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, IceBumpmapVertexShader) == 0x188);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, IceBumpmapPixelShader) == 0x190);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, BumpmapRainVertexShader) == 0x198);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, BumpmapRainPixelShader) == 0x1a0);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, EnvironmentMapVertexShader) == 0x1a8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, EnvironmentMapPixelShader) == 0x1b0);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SkyMapVertexShader) == 0x1b8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SkyMapPixelShader) == 0x1c0);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, EnvMapOverlayVertexShader) == 0x1c8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, EnvMapOverlayPixelShader) == 0x1d0);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, DynamicTexturePageHandle) == 0x1d8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, EnvironmentMapHandle) == 0x1dc);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, EnvironmentMap) == 0x1e0);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaBumpMapHandle) == 0x1e4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaBumpMap) == 0x1e8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, WaterBumpMapHandle) == 0x1ec);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, WaterBumpMap) == 0x1f0);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, IceBumpMapHandle) == 0x1f4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, IceBumpMap) == 0x1f8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, ForegroundRenderList) == 0x1fc);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, BackgroundRenderList) == 0x208);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, AlphaedBackgroundRenderList) == 0x214);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, IceForegroundRenderList) == 0x220);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, IceBackgroundRenderList) == 0x22c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, IceAlphaedBackgroundRenderList) == 0x238);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaReflectiveForegroundRenderList) == 0x244);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaNonReflectiveForegroundRenderList) == 0x250);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaMidgroundRenderList) == 0x25c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaAlphaedMidgroundRenderList) == 0x268);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, HighendWaterEnabled) == 0x274);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, HighendWaterSupported) == 0x275);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, HaveBackgroundSea) == 0x276);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, LoadedOrLoadingSeaBodyID_PStringData) == 0x278);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, RequestedSeaBodyID_PStringData) == 0x27c);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, Settings) == 0x280);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, Tesselator) == 0x564);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, SeaTesselator) == 0x584);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, WaterGenerator) == 0x5a4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, StaticMapBankFile) == 0x5a8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, StaticMapSeaEntryPrefix_PStringData) == 0x5ac);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, LoadingSeaEntry_Data) == 0x5b0);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, LoadingSeaEntry_Info) == 0x5b4);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, LoadingSea_Data) == 0x5b8);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, LoadingSea_Info) == 0x5bc);
FABLE_STATIC_ASSERT(offsetof(CEngineWaterRenderer, WaterBumpFactor) == 0x5c0);
#endif // FABLE_RETAIL_CENGINEWATERRENDERER

#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_RENDER
#include "engine/CRenderManager.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
FABLE_FORCEINLINE void CEngineWaterRenderer::SetCommonRenderStates()
{
    NEngineInternal::GPRenderManager->SetSpecularEnable(false);
    NEngineInternal::GPRenderManager->SetZBufferEnable(true);
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(true);
    if (Settings.EnableBumpMap) {
        NEngineInternal::GPRenderManager->SetTextureBumpEnvMatrix(1, Settings.BumpMatFactor, 0.0f, 0.0f, Settings.BumpMatFactor);
        NEngineInternal::GPRenderManager->SetTextureBumpEnvMatrix(2, Settings.BumpMatFactor, 0.0f, 0.0f, Settings.BumpMatFactor);
    } else {
        NEngineInternal::GPRenderManager->SetTextureBumpEnvMatrix(1, 0.0f, 0.0f, 0.0f, 0.0f);
        NEngineInternal::GPRenderManager->SetTextureBumpEnvMatrix(2, 0.0f, 0.0f, 0.0f, 0.0f);
    }
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER);
    NEngineInternal::GPRenderManager->SetAlphaRef(1);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
}
#endif
#endif // FABLE_ENGINE_CENGINEWATERRENDERER_H
