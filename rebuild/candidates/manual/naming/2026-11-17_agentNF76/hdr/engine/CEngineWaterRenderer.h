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
#ifdef FABLE_RETAIL_PASS22_WATER_ORIGINAL_COMPLETE_OWNER
// Both original PDB signatures agree; compgenx methods stay implicit.
// Field-list order is a reconstruction hypothesis, not proof of textual source order.
#include "engine/EWaterArrayIndexAliases.h"
class CMovableResource;
class CEngineLandscapeMap;
class CDataInputStream;
class CMemoryDataInputStream;
class CMemoryDataOutputStream;
class CDataOutputStream;
class CWaterPatchDescriptors;
class C2DBoxI;
class CWaterMemoryStats;
class CRenderTarget;
class CSurface;
class CEngineWaterRenderer : public CEngineComponent {
public:
    CEngineWaterRenderer(const CEngineInit &);
public:
    virtual ~CEngineWaterRenderer();
public:
    virtual void FrameStart(const CEngineComponentParams &);
public:
    virtual void FrameEnd(const CEngineComponentParams &);
public:
    virtual void Render(const CEngineComponentRenderParams &);
public:
    virtual void WorldFrameUpdate(bool);
public:
    virtual unsigned long PeekSceneFilterFlags() const;
public:
    virtual bool SetEffectQuality(const CEngineVideoOptionsDef &);
public:
    virtual CCharString GetConsoleEnableFunctionName();
public:
    CWaterPatchMesh * LoadPatch(CMovableResource *, const CEngineLandscapeMap *, CDataInputStream &);
public:
    void LoadSea(CMemoryDataInputStream &);
public:
    void ReleaseSeaBuffers();
public:
    void LoadNonPatchData(CMemoryDataInputStream &, CMemoryDataInputStream &);
public:
    void AddForegroundWaterPatchToRenderList(CWaterPatchMesh *, C3DBoundingBox &);
public:
    void AddMidgroundSeaPatchToRenderList(CEngineWaterBackgroundSubPatch *, bool);
public:
    void AddBackgroundWaterPatchToRenderList(CEngineWaterBackgroundSubPatch *, bool);
public:
    void DeleteForegroundPatch(CWaterPatchMesh * *);
public:
    CWaterPatchTesselatorBase<CTVertexWaterBackground> & GetTesselator();
public:
    CWaterPatchTesselatorBase<CTVertexSeaBackground> & GetSeaTesselator();
public:
    CWaterMemoryStats GetMemoryStats();
public:
    void SetSeaBodyToRender(CCharString);
public:
    void OpenStaticMapForUse(CStaticMapBankFile *, CCharString);
public:
    void CloseStaticMap();
public:
    CWaterPatchMesh * AddForegroundPatch(CMovableResource *, const CEngineLandscapeMap *, const long, const long, const CWaterPatchDescriptors &);
public:
    void SavePatch(CDataOutputStream &, CWaterPatchMesh *);
public:
    void BuildAndSaveSea(const CArray<C2DBoxI> &, CMemoryDataOutputStream &, CMemoryDataOutputStream &);
public:
    void BuildAndSaveSea(const CArray<C2DBoxI> &, CStaticMapBankFile &, long, CCharString);
public:
    bool SeaMapNeedsUpdate(const CArray<C2DBoxI> &, CMemoryDataInputStream &);
public:
    bool NonPatchDataNeedsUpdate(CMemoryDataInputStream &);
public:
    void BuildAndSaveNonPatchData(CMemoryDataOutputStream &, CMemoryDataOutputStream &);
public:
    void BuildAndSaveNonPatchData(CStaticMapBankFile &, long);
public:
    void CreateWaterGenerator();
public:
    void DestroyWaterGenerator();
public:
    CWaterGenerator * GetWaterGenerator();
private:
    bool SetBackgroundTexturesAndUVModes();
private:
    bool SetForegroundTexturesAndUVModes();
private:
    bool SetIceBackgroundTexturesAndUVModes();
private:
    bool SetIceForegroundTexturesAndUVModes(bool);
private:
    bool SetSeaNonReflectiveForegroundTexturesAndUVModes();
private:
    bool SetSeaReflectiveForegroundTexturesAndUVModes();
private:
    bool SetSeaMidgroundTexturesAndUVModes();
private:
    bool SetSeaBackgroundTexturesAndUVModes();
private:
    void SetupBumpMapShaderConstants();
private:
    void SetupForegroundShaderConstantsAndFog();
private:
    void SetupBackgroundShaderConstantsAndFog();
private:
    void SetupIceForegroundShaderConstantsAndFog();
private:
    void SetupIceBackgroundShaderConstantsAndFog();
private:
    void SetupSeaReflectiveForegroundShaderConstantsAndFog();
private:
    void SetupSeaNonReflectiveForegroundShaderConstantsAndFog();
private:
    void SetupSeaMidgroundShaderConstantsAndFog();
private:
    void SetupSeaBackgroundShaderConstantsAndFog();
private:
    void SetupFresnelPixelShaderConstants();
private:
    void SetupSpecularPixelShaderConstants();
private:
    void SetCommonRenderStates();
private:
    void RenderForeground(const float);
private:
    void RenderBackground(const float);
private:
    void RenderIceForeground(const float);
private:
    void RenderIceBackground(const float);
private:
    void RenderSea(const float);
private:
    void RenderSeaReflectiveForeground(const float);
private:
    void RenderSeaNonReflectiveForeground(const float);
private:
    void RenderSeaMidground(const float);
private:
    void RenderSeaBackground(const float);
private:
    void RenderBackgroundPatch(CEngineWaterBackgroundSubPatch *);
private:
    void RenderBackgroundList(CArray<CEngineWaterBackgroundSubPatch *> &, const float);
private:
    void RenderEnvironmentMap();
private:
    void RenderBumpMap(EWaterArrayIndexAliases);
private:
    void RenderIceBumpMap();
private:
    void BuildConstantBuffers();
private:
    void DeleteConstantBuffers();
private:
    void InitialiseDynamicTextures();
private:
    void UninitialiseDynamicTextures();
private:
    bool CreateDynamicTextures(const bool, const bool, const bool);
private:
    void FinishUsingDynamicTextures();
private:
    void LoadCompressedBuffer(CMemoryDataInputStream &, CVertexBufferWin32 &, long *);
private:
    void LoadCompressedBuffer(CMemoryDataInputStream &, CIndexBuffer &, long *);
private:
    void SaveBufferCompressed(CMemoryDataOutputStream &, CVertexBufferWin32 &);
private:
    void SaveBufferCompressed(CMemoryDataOutputStream &, CIndexBuffer &, long);
private:
    void SetRenderTargetToSurface(CRenderTarget &, const CSurface &);
private:
    bool ProcessSeaLoading();
public:
    CIndexBuffer         ForegroundIndexBuffers;                         // +0x00c
public:
    CIndexBuffer         ForegroundTextureIndexBuffers;                  // +0x020
public:
    CVertexBufferWin32   ForegroundTextureVertexBuffers;                 // +0x034
public:
    CIndexBuffer         SeaBackgroundIndexBuffer;                       // +0x068
public:
    CVertexBufferWin32   SeaBackgroundVertexBuffer;                      // +0x07c
public:
    long                 SeaBackgroundVertexCount;                       // +0x0b0
public:
    long                 SeaBackgroundPolyCount;                         // +0x0b4
public:
    long                 WaterForegroundPolyCount;                       // +0x0b8
public:
    CVertexShader        SeaBackgroundVertexShader;                      // +0x0bc
public:
    CPixelShader         SeaBackgroundPixelShader;                       // +0x0c4
public:
    CVertexShader        SeaForegroundReflectiveVertexShader;            // +0x0cc
public:
    CPixelShader         SeaForegroundReflectivePixelShader;             // +0x0d4
public:
    CVertexShader        SeaForegroundNonReflectiveVertexShader;         // +0x0dc
public:
    CPixelShader         SeaForegroundNonReflectivePixelShader;          // +0x0e4
public:
    CVertexShader        ForegroundVertexShader;                         // +0x0ec
public:
    CPixelShader         ForegroundPixelShader;                          // +0x0f4
public:
    CPixelShader         RefractionForegroundPixelShader;                // +0x0fc
public:
    CVertexShader        BackgroundVertexShader;                         // +0x104
public:
    CPixelShader         BackgroundPixelShader;                          // +0x10c
public:
    CVertexShader        SeaBackgroundLowEndVertexShader;                // +0x114
public:
    CVertexShader        WaterBackgroundLowEndVertexShader;              // +0x11c
public:
    CPixelShader         WaterLowEndPixelShader;                         // +0x124
private:
    CVertexShader & GetIceForegroundVertexShader(long);
private:
    CVertexShader & GetIceForegroundShadowedVertexShader(long);
private:
    CVertexShader & GetIceForegroundColbuffShadowedVertexShader(long);
public:
    CArray<CVertexShader> IceForegroundVertexShader;                     // +0x12c
public:
    CArray<CVertexShader> IceForegroundShadowedVertexShader;             // +0x138
public:
    CArray<CVertexShader> IceForegroundColbuffShadowedVertexShader;      // +0x144
public:
    CVertexShader        IceBackgroundVertexShader;                      // +0x150
public:
    CPixelShader         IceForegroundPixelShader;                       // +0x158
public:
    CPixelShader         IceForegroundShadowedPixelShader;               // +0x160
public:
    CPixelShader         IceForegroundColbuffShadowedPixelShader;        // +0x168
public:
    CPixelShader         IceBackgroundPixelShader;                       // +0x170
public:
    CVertexShader        BumpmapVertexShader;                            // +0x178
public:
    CPixelShader         BumpmapPixelShader;                             // +0x180
public:
    CVertexShader        IceBumpmapVertexShader;                         // +0x188
public:
    CPixelShader         IceBumpmapPixelShader;                          // +0x190
public:
    CVertexShader        BumpmapRainVertexShader;                        // +0x198
public:
    CPixelShader         BumpmapRainPixelShader;                         // +0x1a0
public:
    CVertexShader        EnvironmentMapVertexShader;                     // +0x1a8
public:
    CPixelShader         EnvironmentMapPixelShader;                      // +0x1b0
public:
    CVertexShader        SkyMapVertexShader;                             // +0x1b8
public:
    CPixelShader         SkyMapPixelShader;                              // +0x1c0
public:
    CVertexShader        EnvMapOverlayVertexShader;                      // +0x1c8
public:
    CPixelShader         EnvMapOverlayPixelShader;                       // +0x1d0
public:
    long                 DynamicTexturePageHandle;                       // +0x1d8
public:
    long                 EnvironmentMapHandle;                           // +0x1dc
public:
    CTexture*            EnvironmentMap;                                 // +0x1e0
public:
    long                 SeaBumpMapHandle;                               // +0x1e4
public:
    CTexture*            SeaBumpMap;                                     // +0x1e8
public:
    long                 WaterBumpMapHandle;                             // +0x1ec
public:
    CTexture*            WaterBumpMap;                                   // +0x1f0
public:
    long                 IceBumpMapHandle;                               // +0x1f4
public:
    CTexture*            IceBumpMap;                                     // +0x1f8
public:
    CArray<CWaterPatchMesh*> ForegroundRenderList;                       // +0x1fc
public:
    CArray<CEngineWaterBackgroundSubPatch*> BackgroundRenderList;        // +0x208
public:
    CArray<CEngineWaterBackgroundSubPatch*> AlphaedBackgroundRenderList; // +0x214
public:
    CArray<std::pair<C3DBoundingBox, CWaterPatchMesh*> > IceForegroundRenderList;   // +0x220
public:
    CArray<CEngineWaterBackgroundSubPatch*> IceBackgroundRenderList;     // +0x22c
public:
    CArray<CEngineWaterBackgroundSubPatch*> IceAlphaedBackgroundRenderList;   // +0x238
public:
    CArray<CWaterPatchMesh*> SeaReflectiveForegroundRenderList;          // +0x244
public:
    CArray<CWaterPatchMesh*> SeaNonReflectiveForegroundRenderList;       // +0x250
public:
    CArray<CEngineWaterBackgroundSubPatch*> SeaMidgroundRenderList;      // +0x25c
public:
    CArray<CEngineWaterBackgroundSubPatch*> SeaAlphaedMidgroundRenderList;   // +0x268
public:
    bool                 HighendWaterEnabled;                            // +0x274
public:
    bool                 HighendWaterSupported;                          // +0x275
public:
    bool                 HaveBackgroundSea;                              // +0x276
public:
    CCharString          LoadedOrLoadingSeaBodyID;                       // +0x278
public:
    CCharString          RequestedSeaBodyID;                             // +0x27c
public:
    CEngineWaterSettings Settings;                                       // +0x280
public:
    CWaterPatchTesselatorBase<CTVertexWaterBackground> Tesselator;       // +0x564
public:
    CWaterPatchTesselatorBase<CTVertexSeaBackground> SeaTesselator;      // +0x584
public:
    CWaterGenerator*     WaterGenerator;                                 // +0x5a4
public:
    CStaticMapBankFile*  StaticMapBankFile;                              // +0x5a8
public:
    CCharString          StaticMapSeaEntryPrefix;                        // +0x5ac
public:
    CCountedPointer<CBankFileAsyncEntry> LoadingSeaEntry;                // +0x5b0
public:
    CCountedPointer<CBankFileAsyncData> LoadingSea;                      // +0x5b8
public:
    float                WaterBumpFactor;                                // +0x5c0
};
#else
class CEngineWaterRenderer : public CEngineComponent {
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_SEAMIDGROUND_BODIES
private:
    bool SetSeaMidgroundTexturesAndUVModes();
    void SetupSeaMidgroundShaderConstantsAndFog();
#endif

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
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_SEABACKGROUND_BODIES
    // Original PDB private members, expanded in native00B70230.
    bool SetSeaBackgroundTexturesAndUVModes();
    void SetupSeaBackgroundShaderConstantsAndFog();
#endif
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_SETUPSEANONREFLECTIVEFOREGROUND
    // NF68 opt-in (row 0x00B7FA20): FableWin.pdb 0x02D501F0 ?SetupSeaNonReflectiveForegroundShaderConstantsAndFog@CEngineWaterRenderer@@AAEXXZ
    // (private; retail's out-of-line copy 0x00B7FA20 has no direct caller, RenderSeaNonReflectiveForeground 0x00B77E20 expands it).
    void SetupSeaNonReflectiveForegroundShaderConstantsAndFog();         // 0x00B7FA20
#endif
    void RenderSeaBackground(float time);                                // ?RenderSeaBackground@CEngineWaterRenderer@@AAEXM@Z
    void RenderEnvironmentMap();                                         // ?RenderEnvironmentMap@CEngineWaterRenderer@@AAEXXZ
    void RenderIceForeground(float time);                                // ?RenderIceForeground@CEngineWaterRenderer@@AAEXM@Z
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_FOREGROUND_BODIES_PASS22
    // Both original private method descriptors and actual donor helper calls.
    bool SetForegroundTexturesAndUVModes();
    void SetupForegroundShaderConstantsAndFog();
    void SetupFresnelPixelShaderConstants();
#endif
    void RenderForeground(float time);                                   // ?RenderForeground@CEngineWaterRenderer@@AAEXM@Z
    void RenderSeaReflectiveForeground(float time);                      // ?RenderSeaReflectiveForeground@CEngineWaterRenderer@@AAEXM@Z
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_CREATEDYNAMICTEXTURES
    // Opt-in (bwf2, row 0x00B72180): Ego_r.pdb 0x00A1EE70 / FableWin.pdb ?CreateDynamicTextures@CEngineWaterRenderer@@AAE_N_N00@Z,
    // ?RenderBumpMap@CEngineWaterRenderer@@AAEXW4EWaterArrayIndexAliases@@@Z (0x00B6E890), ?RenderIceBumpMap@CEngineWaterRenderer@@AAEXXZ
    // (0x00B6E3E0) and FableWin ?SetRenderTargetToSurface@CEngineWaterRenderer@@AAEXAAVCRenderTarget@@ABVCSurface@@@Z (inline; retail's
    // unreferenced out-of-line copy 0x00B7AD20). Parameter names descriptive.
    bool CreateDynamicTextures(bool render_water_bump, bool render_sea_bump, bool render_ice_bump);   // 0x00B72180
    void RenderBumpMap(enum EWaterArrayIndexAliases index);              // 0x00B6E890
    void RenderIceBumpMap();                                             // 0x00B6E3E0
    void SetRenderTargetToSurface(class CRenderTarget& target, const class CSurface& surface);   // 0x00B7AD20
#endif
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_SEAMIDGROUND
    // Opt-in (chunk 892, row 0x00B6FA40): Ego_r.pdb / FableWin.pdb ?RenderSeaMidground@CEngineWaterRenderer@@AAEXM@Z and
    // ?RenderBackgroundList@CEngineWaterRenderer@@AAEXAAV?$CArray@PAVCEngineWaterBackgroundSubPatch@@@@M@Z (landed 0x00B6E260).
    void RenderSeaMidground(float time);                                 // 0x00B6FA40
#ifdef FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22
    void RenderBackgroundPatch(CEngineWaterBackgroundSubPatch* patch);
#endif
    void RenderBackgroundList(CArray<CEngineWaterBackgroundSubPatch*>& list, float time);   // 0x00B6E260
#endif
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_RENDER
    // Opt-in (bwf2 / agentC8, row 0x00B783F0 Render): Ego_r.pdb private members Render calls (?RenderBackground / ?RenderIceBackground /
    // ?RenderSeaNonReflectiveForeground@CEngineWaterRenderer@@AAEXM@Z) and the FableWin.pdb inline ?SetCommonRenderStates@CEngineWaterRenderer@@AAEXXZ
    // (0x02D4CC20 debug; no retail copy, expanded three times by Render; body in the row).
    void RenderBackground(float time);                                   // 0x00B76BC0
    void RenderIceBackground(float time);                                // 0x00B75CA0
    void RenderSeaNonReflectiveForeground(float time);                   // 0x00B77E20
    void SetCommonRenderStates();
#endif
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_CTOR
    // Opt-in (wfE6, row 0x00B73760 ctor): Ego_r.pdb 0x00A1AA10 / FableWin.pdb ?InitialiseDynamicTextures@CEngineWaterRenderer@@AAEXXZ (private;
    // retail 0x00B6D510, landed under the wrong leaf ?InitialiseDynamicTextures@CWaterDynamicTextures@@QAEXXZ: NAME_FIXES.tsv).
    void InitialiseDynamicTextures();                                    // 0x00B6D510
#endif
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_FOREGROUNDHELPERS
    // Opt-in (wfA4, rows 0x00B77370 RenderSeaReflectiveForeground / 0x00B751F0 RenderIceForeground): the private helpers both rows expand
    // (FableWin.pdb ?SetSeaReflectiveForegroundTexturesAndUVModes@CEngineWaterRenderer@@AAE_NXZ 0x02D4F600, ?SetupSeaReflectiveForeground
    // ShaderConstantsAndFog@CEngineWaterRenderer@@AAEXXZ 0x02D4FAD0, ?SetIceForegroundTexturesAndUVModes@CEngineWaterRenderer@@AAE_N_N@Z
    // 0x02D4D210, ?SetupIceForegroundShaderConstantsAndFog@CEngineWaterRenderer@@AAEXXZ 0x02D4D690 (retail copy 0x00B7F3B0)); bodies in the rows.
    bool SetSeaReflectiveForegroundTexturesAndUVModes();
    void SetupSeaReflectiveForegroundShaderConstantsAndFog();
    bool SetIceForegroundTexturesAndUVModes(bool shadows);
    void SetupIceForegroundShaderConstantsAndFog();
#ifndef FABLE_RETAIL_CENGINEWATERRENDERER_FOREGROUND_BODIES_PASS22   // (that opt-in declares it)
    void SetupFresnelPixelShaderConstants();                             // FableWin.pdb ?SetupFresnelPixelShaderConstants@CEngineWaterRenderer@@AAEXXZ 0x02D4D810
#endif
    // Ego_r.pdb / FableWin.pdb ?GetIceForeground{,Shadowed,ColbuffShadowed}VertexShader@CEngineWaterRenderer@@AAEAAVCVertexShader@@J@Z
    // (out of line in retail: 0x00B6DF50 / 0x00B6DF90 / 0x00B6DFD0, called by RenderIceForeground).
    CVertexShader& GetIceForegroundVertexShader(long light_count);
    CVertexShader& GetIceForegroundShadowedVertexShader(long light_count);
    CVertexShader& GetIceForegroundColbuffShadowedVertexShader(long light_count);
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
#endif

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
FABLE_STATIC_ASSERT_AT(226, sizeof(CEngineWaterRenderer) == 0x5c4);
FABLE_STATIC_ASSERT_AT(227, offsetof(CEngineWaterRenderer, ForegroundIndexBuffers) == 0xc);
FABLE_STATIC_ASSERT_AT(228, offsetof(CEngineWaterRenderer, SeaBackgroundVertexShader) == 0xbc);
FABLE_STATIC_ASSERT_AT(229, offsetof(CEngineWaterRenderer, IceForegroundVertexShader) == 0x12c);
FABLE_STATIC_ASSERT_AT(230, offsetof(CEngineWaterRenderer, DynamicTexturePageHandle) == 0x1d8);
FABLE_STATIC_ASSERT_AT(231, offsetof(CEngineWaterRenderer, ForegroundRenderList) == 0x1fc);
FABLE_STATIC_ASSERT_AT(232, offsetof(CEngineWaterRenderer, HighendWaterEnabled) == 0x274);
FABLE_STATIC_ASSERT_AT(233, offsetof(CEngineWaterRenderer, LoadedOrLoadingSeaBodyID) == 0x278);
FABLE_STATIC_ASSERT_AT(234, offsetof(CEngineWaterRenderer, Settings) == 0x280);
FABLE_STATIC_ASSERT_AT(235, offsetof(CEngineWaterRenderer, Tesselator) == 0x564);
FABLE_STATIC_ASSERT_AT(236, offsetof(CEngineWaterRenderer, SeaTesselator) == 0x584);
FABLE_STATIC_ASSERT_AT(237, offsetof(CEngineWaterRenderer, StaticMapSeaEntryPrefix) == 0x5ac);
FABLE_STATIC_ASSERT_AT(238, offsetof(CEngineWaterRenderer, LoadingSea) == 0x5b8);
FABLE_STATIC_ASSERT_AT(239, offsetof(CEngineWaterRenderer, WaterBumpFactor) == 0x5c0);
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
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_OPENSTATICMAPFORUSE_DECL
    // NF65 overlay opt-in (row 0x00B42750): Ego_r.pdb 0x00A1A9C0 ?OpenStaticMapForUse@CEngineWaterRenderer@@QAEXPAVCStaticMapBankFile@@VCCharString@@@Z
    // (public, out of line at 0x00B6D4D0) on the flattened default layout, for callers that reach the renderer through a pointer only.
    // The includer provides CCharString. Declaration only, no layout change.
    void OpenStaticMapForUse(CStaticMapBankFile* file, CCharString symbol);   // 0x00B6D4D0
#endif
#ifdef FABLE_RETAIL_CENGINEWATERRENDERER_LOADNONPATCHDATA_DECL
    // NF76 opt-in (row 0x00B41FA0 CEngineWorldMap::LoadWaterData): Ego_r.pdb ?LoadNonPatchData@CEngineWaterRenderer@@QAEXAAVCMemoryDataInputStream@@0@Z
    // (public, out of line at 0x00B6DAF0) on the flattened default layout, for callers that reach the renderer through a pointer only.
    // Declaration only, no layout change.
    void LoadNonPatchData(class CMemoryDataInputStream& data, class CMemoryDataInputStream& header);   // 0x00B6DAF0
#endif
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

FABLE_STATIC_ASSERT_AT(345, sizeof(CEngineWaterRenderer) == 0x5c4);
FABLE_STATIC_ASSERT_AT(346, offsetof(CEngineWaterRenderer, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(347, offsetof(CEngineWaterRenderer, ForegroundIndexBuffers) == 0xc);
FABLE_STATIC_ASSERT_AT(348, offsetof(CEngineWaterRenderer, ForegroundTextureIndexBuffers) == 0x20);
FABLE_STATIC_ASSERT_AT(349, offsetof(CEngineWaterRenderer, ForegroundTextureVertexBuffers) == 0x34);
FABLE_STATIC_ASSERT_AT(350, offsetof(CEngineWaterRenderer, SeaBackgroundIndexBuffer) == 0x68);
FABLE_STATIC_ASSERT_AT(351, offsetof(CEngineWaterRenderer, SeaBackgroundVertexBuffer) == 0x7c);
FABLE_STATIC_ASSERT_AT(352, offsetof(CEngineWaterRenderer, SeaBackgroundVertexCount) == 0xb0);
FABLE_STATIC_ASSERT_AT(353, offsetof(CEngineWaterRenderer, SeaBackgroundPolyCount) == 0xb4);
FABLE_STATIC_ASSERT_AT(354, offsetof(CEngineWaterRenderer, WaterForegroundPolyCount) == 0xb8);
FABLE_STATIC_ASSERT_AT(355, offsetof(CEngineWaterRenderer, SeaBackgroundVertexShader) == 0xbc);
FABLE_STATIC_ASSERT_AT(356, offsetof(CEngineWaterRenderer, SeaBackgroundPixelShader) == 0xc4);
FABLE_STATIC_ASSERT_AT(357, offsetof(CEngineWaterRenderer, SeaForegroundReflectiveVertexShader) == 0xcc);
FABLE_STATIC_ASSERT_AT(358, offsetof(CEngineWaterRenderer, SeaForegroundReflectivePixelShader) == 0xd4);
FABLE_STATIC_ASSERT_AT(359, offsetof(CEngineWaterRenderer, SeaForegroundNonReflectiveVertexShader) == 0xdc);
FABLE_STATIC_ASSERT_AT(360, offsetof(CEngineWaterRenderer, SeaForegroundNonReflectivePixelShader) == 0xe4);
FABLE_STATIC_ASSERT_AT(361, offsetof(CEngineWaterRenderer, ForegroundVertexShader) == 0xec);
FABLE_STATIC_ASSERT_AT(362, offsetof(CEngineWaterRenderer, ForegroundPixelShader) == 0xf4);
FABLE_STATIC_ASSERT_AT(363, offsetof(CEngineWaterRenderer, RefractionForegroundPixelShader) == 0xfc);
FABLE_STATIC_ASSERT_AT(364, offsetof(CEngineWaterRenderer, BackgroundVertexShader) == 0x104);
FABLE_STATIC_ASSERT_AT(365, offsetof(CEngineWaterRenderer, BackgroundPixelShader) == 0x10c);
FABLE_STATIC_ASSERT_AT(366, offsetof(CEngineWaterRenderer, SeaBackgroundLowEndVertexShader) == 0x114);
FABLE_STATIC_ASSERT_AT(367, offsetof(CEngineWaterRenderer, WaterBackgroundLowEndVertexShader) == 0x11c);
FABLE_STATIC_ASSERT_AT(368, offsetof(CEngineWaterRenderer, WaterLowEndPixelShader) == 0x124);
FABLE_STATIC_ASSERT_AT(369, offsetof(CEngineWaterRenderer, IceForegroundVertexShader) == 0x12c);
FABLE_STATIC_ASSERT_AT(370, offsetof(CEngineWaterRenderer, IceForegroundShadowedVertexShader) == 0x138);
FABLE_STATIC_ASSERT_AT(371, offsetof(CEngineWaterRenderer, IceForegroundColbuffShadowedVertexShader) == 0x144);
FABLE_STATIC_ASSERT_AT(372, offsetof(CEngineWaterRenderer, IceBackgroundVertexShader) == 0x150);
FABLE_STATIC_ASSERT_AT(373, offsetof(CEngineWaterRenderer, IceForegroundPixelShader) == 0x158);
FABLE_STATIC_ASSERT_AT(374, offsetof(CEngineWaterRenderer, IceForegroundShadowedPixelShader) == 0x160);
FABLE_STATIC_ASSERT_AT(375, offsetof(CEngineWaterRenderer, IceForegroundColbuffShadowedPixelShader) == 0x168);
FABLE_STATIC_ASSERT_AT(376, offsetof(CEngineWaterRenderer, IceBackgroundPixelShader) == 0x170);
FABLE_STATIC_ASSERT_AT(377, offsetof(CEngineWaterRenderer, BumpmapVertexShader) == 0x178);
FABLE_STATIC_ASSERT_AT(378, offsetof(CEngineWaterRenderer, BumpmapPixelShader) == 0x180);
FABLE_STATIC_ASSERT_AT(379, offsetof(CEngineWaterRenderer, IceBumpmapVertexShader) == 0x188);
FABLE_STATIC_ASSERT_AT(380, offsetof(CEngineWaterRenderer, IceBumpmapPixelShader) == 0x190);
FABLE_STATIC_ASSERT_AT(381, offsetof(CEngineWaterRenderer, BumpmapRainVertexShader) == 0x198);
FABLE_STATIC_ASSERT_AT(382, offsetof(CEngineWaterRenderer, BumpmapRainPixelShader) == 0x1a0);
FABLE_STATIC_ASSERT_AT(383, offsetof(CEngineWaterRenderer, EnvironmentMapVertexShader) == 0x1a8);
FABLE_STATIC_ASSERT_AT(384, offsetof(CEngineWaterRenderer, EnvironmentMapPixelShader) == 0x1b0);
FABLE_STATIC_ASSERT_AT(385, offsetof(CEngineWaterRenderer, SkyMapVertexShader) == 0x1b8);
FABLE_STATIC_ASSERT_AT(386, offsetof(CEngineWaterRenderer, SkyMapPixelShader) == 0x1c0);
FABLE_STATIC_ASSERT_AT(387, offsetof(CEngineWaterRenderer, EnvMapOverlayVertexShader) == 0x1c8);
FABLE_STATIC_ASSERT_AT(388, offsetof(CEngineWaterRenderer, EnvMapOverlayPixelShader) == 0x1d0);
FABLE_STATIC_ASSERT_AT(389, offsetof(CEngineWaterRenderer, DynamicTexturePageHandle) == 0x1d8);
FABLE_STATIC_ASSERT_AT(390, offsetof(CEngineWaterRenderer, EnvironmentMapHandle) == 0x1dc);
FABLE_STATIC_ASSERT_AT(391, offsetof(CEngineWaterRenderer, EnvironmentMap) == 0x1e0);
FABLE_STATIC_ASSERT_AT(392, offsetof(CEngineWaterRenderer, SeaBumpMapHandle) == 0x1e4);
FABLE_STATIC_ASSERT_AT(393, offsetof(CEngineWaterRenderer, SeaBumpMap) == 0x1e8);
FABLE_STATIC_ASSERT_AT(394, offsetof(CEngineWaterRenderer, WaterBumpMapHandle) == 0x1ec);
FABLE_STATIC_ASSERT_AT(395, offsetof(CEngineWaterRenderer, WaterBumpMap) == 0x1f0);
FABLE_STATIC_ASSERT_AT(396, offsetof(CEngineWaterRenderer, IceBumpMapHandle) == 0x1f4);
FABLE_STATIC_ASSERT_AT(397, offsetof(CEngineWaterRenderer, IceBumpMap) == 0x1f8);
FABLE_STATIC_ASSERT_AT(398, offsetof(CEngineWaterRenderer, ForegroundRenderList) == 0x1fc);
FABLE_STATIC_ASSERT_AT(399, offsetof(CEngineWaterRenderer, BackgroundRenderList) == 0x208);
FABLE_STATIC_ASSERT_AT(400, offsetof(CEngineWaterRenderer, AlphaedBackgroundRenderList) == 0x214);
FABLE_STATIC_ASSERT_AT(401, offsetof(CEngineWaterRenderer, IceForegroundRenderList) == 0x220);
FABLE_STATIC_ASSERT_AT(402, offsetof(CEngineWaterRenderer, IceBackgroundRenderList) == 0x22c);
FABLE_STATIC_ASSERT_AT(403, offsetof(CEngineWaterRenderer, IceAlphaedBackgroundRenderList) == 0x238);
FABLE_STATIC_ASSERT_AT(404, offsetof(CEngineWaterRenderer, SeaReflectiveForegroundRenderList) == 0x244);
FABLE_STATIC_ASSERT_AT(405, offsetof(CEngineWaterRenderer, SeaNonReflectiveForegroundRenderList) == 0x250);
FABLE_STATIC_ASSERT_AT(406, offsetof(CEngineWaterRenderer, SeaMidgroundRenderList) == 0x25c);
FABLE_STATIC_ASSERT_AT(407, offsetof(CEngineWaterRenderer, SeaAlphaedMidgroundRenderList) == 0x268);
FABLE_STATIC_ASSERT_AT(408, offsetof(CEngineWaterRenderer, HighendWaterEnabled) == 0x274);
FABLE_STATIC_ASSERT_AT(409, offsetof(CEngineWaterRenderer, HighendWaterSupported) == 0x275);
FABLE_STATIC_ASSERT_AT(410, offsetof(CEngineWaterRenderer, HaveBackgroundSea) == 0x276);
FABLE_STATIC_ASSERT_AT(411, offsetof(CEngineWaterRenderer, LoadedOrLoadingSeaBodyID_PStringData) == 0x278);
FABLE_STATIC_ASSERT_AT(412, offsetof(CEngineWaterRenderer, RequestedSeaBodyID_PStringData) == 0x27c);
FABLE_STATIC_ASSERT_AT(413, offsetof(CEngineWaterRenderer, Settings) == 0x280);
FABLE_STATIC_ASSERT_AT(414, offsetof(CEngineWaterRenderer, Tesselator) == 0x564);
FABLE_STATIC_ASSERT_AT(415, offsetof(CEngineWaterRenderer, SeaTesselator) == 0x584);
FABLE_STATIC_ASSERT_AT(416, offsetof(CEngineWaterRenderer, WaterGenerator) == 0x5a4);
FABLE_STATIC_ASSERT_AT(417, offsetof(CEngineWaterRenderer, StaticMapBankFile) == 0x5a8);
FABLE_STATIC_ASSERT_AT(418, offsetof(CEngineWaterRenderer, StaticMapSeaEntryPrefix_PStringData) == 0x5ac);
FABLE_STATIC_ASSERT_AT(419, offsetof(CEngineWaterRenderer, LoadingSeaEntry_Data) == 0x5b0);
FABLE_STATIC_ASSERT_AT(420, offsetof(CEngineWaterRenderer, LoadingSeaEntry_Info) == 0x5b4);
FABLE_STATIC_ASSERT_AT(421, offsetof(CEngineWaterRenderer, LoadingSea_Data) == 0x5b8);
FABLE_STATIC_ASSERT_AT(422, offsetof(CEngineWaterRenderer, LoadingSea_Info) == 0x5bc);
FABLE_STATIC_ASSERT_AT(423, offsetof(CEngineWaterRenderer, WaterBumpFactor) == 0x5c0);
#endif // FABLE_RETAIL_CENGINEWATERRENDERER

#endif // FABLE_ENGINE_CENGINEWATERRENDERER_H
