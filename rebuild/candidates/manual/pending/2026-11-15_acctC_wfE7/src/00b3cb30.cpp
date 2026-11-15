// CEngineResourceManager::CEngineResourceManager(const CEngineInit& init, const CEngineMemoryInit& memory_init) -- construct the engine resource manager: the mesh / shader banks, counted memory pools, VS constant layouts and texture members, then open the shader banks and the vertex / pixel shader resources, set up the texture manager pools, the alpha stipple and 1x1 zero / saturated textures, and register the async loading callback
// Leaf: ??0CEngineResourceManager@@QAE@ABVCEngineInit@@ABVCEngineMemoryInit@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3CB30, 5399 bytes, thiscall `ret 8`, frame 0xC8, no EH, no /GS (TU without /EHsc, as the
//   landed same-TU siblings: no Flags line).
// Identity: Ego_r.pdb / FableWin.pdb ??0CEngineResourceManager@@QAE@ABVCEngineInit@@ABVCEngineMemoryInit@@@Z (public); vtables stored
//   0x012A1AF8 (CAsyncLoadingProcessCallback, inline base ctor) then 0x012A1B3C / 0x012A1B34 (the landed dtor 0x00B3B8E0 stores the
//   same pair); sole caller CEngine::Initialise 0x00B2656E. Members: the typed overlay opt-in FABLE_RETAIL_CENGINERESOURCEMANAGER_CTOR.
// Spellings EVIDENCED by retail bytes: implicit const char* -> CCharString / const unsigned short* -> CWideString conversions for the
//   PS constant names, "MBANK_ENGINE" and L"editmesh.bbb" (the conversion temporary is built before the other arguments, 0x00B3CEA2 /
//   0x00B3CE1D / 0x00B3D747); the named `platform` CWideString in its own block (built ahead of the `false` push, destroyed last,
//   0x00B3CD5F / 0x00B3CDB0); the named bool for AllocatePools (byte home + dword reload, 0x00B3CD24..0x00B3CD2B); pointer-walk
//   loops clearing MeshBanks / GraphicBanks (0x00B3D72C / 0x00B3DD81); CPixelFormat(_D3DFORMAT) ctors (no -1 store, 0x00B3D7D0);
//   the A8R8G8B8 format a temporary (shares the frame word 0x14 with texture_count, 0x00B3DADB); the uncompressed-opaque format read
//   into a named local before the first push_back (0x00B3DA5F) and texture_count / 16 named right after the division (0x00B3D841);
//   SetTextureBank(MainGraphicBank) reuses the stored member (0x00B3CD10); (init.InitFlags & 4) != 0 gives `shr ; and dl,bl`
//   (0x00B3DDA8); the GetSurfaceLevel temporaries destroyed in place (FABLE_RETAIL_CSURFACE_FORCEINLINE_DTOR, 0x00B3DE49).
//   FITTED: none. Descriptive names (no original found): has_mesh_bank, platform, unused_path, opaque_format, scratch_texture_count,
//   the vertex declaration arrays g_Declaration* (only g_Declaration2D 0x0129D708 is bound).
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_CTOR
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_STIPPLE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_EXTERNALTEXTURES
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CMESHDATABANK_CTOR
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_CBANKFILEASYNC_SETLOADINGMEMORY
#define FABLE_RETAIL_CSHADERDATABANK_CTOR
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#define FABLE_RETAIL_CENGINEVSCONSTANTLAYOUTBASIC
#define FABLE_RETAIL_CENGINEVSCONSTANTLAYOUTLIGHTS
#define FABLE_RETAIL_CENGINEVSCONSTANTLAYOUTLIGHTSBONES
#define FABLE_RETAIL_CENGINEVSCONSTANTLAYOUTREPEATEDMESH
#define FABLE_RETAIL_CENGINEVSCONSTANTLAYOUTPARTICLES
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_CTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE_FASTCALL
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DEXTENTSI_INITIALISE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_FORCEINLINE_DTOR
#define FABLE_RETAIL_CSURFACE_CLEARCOLOUR
#define FABLE_RETAIL_CTEXTUREMANAGER_INITIALISE
#define FABLE_RETAIL_CTEXTUREMANAGER_ALLOCATETEXTURE
#define FABLE_RETAIL_CGRAPHICDATABANK_TEXTUREMANAGERACCESS
#define FABLE_RETAIL_CGRAPHICDATABANK_LOADGRAPHICFRAME
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CBANKFILEMANAGER
#define FABLE_RETAIL_CBANKFILEMANAGER_HEADERPATHS
#define FABLE_RETAIL_CBANKFILEMANAGER_ISUSINGRETAILBANKS
#define FABLE_RETAIL_CAFILE_CONVERTDIRECTORYANDFILENAMETOPATH
#define FABLE_RETAIL_CENGINE_PEEKVIDEOOPTIONSDEF
#define FABLE_RETAIL_CENGINEVIDEOOPTIONSDEF_HIRESTEXTUREMEMORY
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
#include "fable_string.h"
#include "engine/CBaseClass.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEngineInit.h"
#include "engine/CEngineMemoryInit.h"
#include "engine/CMeshDataBank.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CTextureManager.h"
#include "engine/CTexture.h"
#include "engine/CSurface.h"
#include "engine/CPixelFormat.h"
#include "engine/C2DExtentsI.h"
#include "engine/CRGBColour.h"
#include "engine/C4DVector.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CShaderResource.h"
#include "engine/CDisplayManager.h"
#include "engine/CEngine.h"
#include "engine/CEngineVideoOptionsDef.h"
#include "engine/CAFile.h"
#include "fable_retail_banks.h"

// CVertexShaderInput and its two N3DPrimitive inputs have no engine header: modelled as the landed
// N3DPrimitive::CVertexShaderInputStatic rows 0x00A98640 / 0x00A98760 do (Ego_r.pdb CVertexShaderInput: DataStreams +0x04
// CArray<CStreamInfo>, D3DDeclaration +0x10 CArray<_D3DVERTEXELEMENT9>; sizeof 0x1C). The derived ctors are the FableWin.pdb
// inline ??0CVertexShaderInputStatic@N3DPrimitive@@QAE@K_N@Z / ??0CVertexShaderInputAnimated@N3DPrimitive@@QAE@K_N@Z.
struct _D3DVERTEXELEMENT9 { unsigned short Stream, Offset; unsigned char Type, Method, Usage, UsageIndex; };   // d3d9types.h, 8 bytes
enum EVertexStreamDataType { VSDT_FLOAT1 = 0 };
class CVertexStreamFormat : public CBaseClass {
public:
    class CDataElement { public: unsigned long Usage; EVertexStreamDataType Type; };
    CArray<CDataElement> DataElements;                      // +0x04
};
class CVertexShaderInput : public CBaseClass {
public:
    struct CStreamInfo {
        unsigned long       Stream;                         // +0x00
        CVertexStreamFormat Format;                         // +0x04
    };
    CVertexShaderInput();                                   // 0x00AB3B10
    CArray<CStreamInfo>        DataStreams;                 // +0x04
    CArray<_D3DVERTEXELEMENT9> D3DDeclaration;              // +0x10
};
namespace N3DPrimitive {
class CVertexShaderInputStatic : public CVertexShaderInput {
public:
    CVertexShaderInputStatic(unsigned long flags, bool b) { Initialise(flags, b); }
    void Initialise(unsigned long flags, bool b);           // 0x00A90560
};
class CVertexShaderInputAnimated : public CVertexShaderInput {
public:
    CVertexShaderInputAnimated(unsigned long flags, bool b) { Initialise(flags, b); }
    void Initialise(unsigned long flags, bool b);           // 0x00A90630
};
}

// CAsyncLoadingMemoryPool has no engine header: Ego_r.pdb / FableWin.pdb
// ?AddLoadingCallback@CAsyncLoadingMemoryPool@@SIXPAVCAsyncLoadingProcessCallback@@@Z (static __fastcall, retail 0x00A14990).
class CAsyncLoadingMemoryPool {
public:
    static void __fastcall AddLoadingCallback(CAsyncLoadingProcessCallback* callback);   // 0x00A14990
};

// CEngineVideoOptions has no engine header: Ego_r.pdb layout as the landed CEngine::SetVideoOptions 0x00B24690 models it.
class CEngineVideoOptions {
public:
    C2DExtentsI               ScreenResolution;             // +0x00
    float                     ScreenAspectRatio[2];         // +0x08 C2DExtentsF
    long                      BitsPerPixel;                 // +0x10
    long                      RefreshRate;                  // +0x14
    bool                      VerticalSync;                 // +0x18
    EEngineVideoOptionQuality LandscapeDetail;              // +0x1c
    EEngineVideoOptionQuality MeshDetail;                   // +0x20
    EEngineVideoOptionQuality TextureDetail;                // +0x24
    EEngineVideoOptionQuality ShadowQuality;                // +0x28
    EEngineVideoOptionQuality EffectsQuality;               // +0x2c
    long                      Antialiasing;                 // +0x30 ESurfaceMultisampleType
};

namespace NEngineInternal {
extern CShaderRenderManager* GPShaderRenderManager;         // 0x01436E14
extern CDisplayManager*      GPDisplayManager;              // 0x01436E1C
extern CEngine*              GPEngine;                      // 0x01436EA4
}
extern CEngineVideoOptions* g_pVideoOptions;                // 0x01436EB0 (no PDB name found; landed binding)

// Vertex declarations (no PDB names found: descriptive, named after the first shader bank that uses each).
extern const _D3DVERTEXELEMENT9 g_DeclarationStatic[];                 // 0x0129D6B0
extern const _D3DVERTEXELEMENT9 g_DeclarationPosColTex1[];             // 0x0129D6D0
extern const _D3DVERTEXELEMENT9 g_Declaration2D[];                     // 0x0129D708
extern const _D3DVERTEXELEMENT9 g_DeclarationVertexPos[];              // 0x0129D748
extern const _D3DVERTEXELEMENT9 g_DeclarationDebugging[];              // 0x0129D788
extern const _D3DVERTEXELEMENT9 g_DeclarationRepeatedMesh[];           // 0x0129D8C0
extern const _D3DVERTEXELEMENT9 g_DeclarationZSprite[];                // 0x0129D8E8
extern const _D3DVERTEXELEMENT9 g_DeclarationWeather[];                // 0x0129D900
extern const _D3DVERTEXELEMENT9 g_DeclarationDecalGroup[];             // 0x0129D920
extern const _D3DVERTEXELEMENT9 g_DeclarationSpriteGroup[];            // 0x0129D940
extern const _D3DVERTEXELEMENT9 g_DeclarationLandscapeBackground[];    // 0x0129D958
extern const _D3DVERTEXELEMENT9 g_DeclarationLandscapeForeground[];    // 0x0129D980
extern const _D3DVERTEXELEMENT9 g_DeclarationWaterForeground[];        // 0x0129D9A8
extern const _D3DVERTEXELEMENT9 g_DeclarationWaterBackground[];        // 0x0129D9E8
extern const _D3DVERTEXELEMENT9 g_DeclarationSeaBackground[];          // 0x0129DA18
extern const _D3DVERTEXELEMENT9 g_DeclarationText[];                   // 0x0129DA28

CEngineResourceManager::CEngineResourceManager(const CEngineInit& init, const CEngineMemoryInit& memory_init)
{
    MaxLoadingTime = 0.001;
    EngineFont = init.Font;
    MainGraphicBank = init.MainGraphicBank;
    MainMeshBank = init.MeshBank;
    if (MainMeshBank && MainMeshBank->GetTextureBank() == 0)
        MainMeshBank->SetTextureBank(MainGraphicBank);
    bool has_mesh_bank = MainMeshBank != 0;
    AllocatePools(has_mesh_bank, memory_init);
    EngineCachePath = init.EngineCacheResourcePath;
    WriteableEngineCachePath = init.WriteableEngineCacheResourcePath;
    if (NBankFileManager::GBankFileManager.IsUsingRetailBanks()) {
        CWideString platform(L"pc\\");
        NBankFileManager::GBankFileManager.OpenRetailBank(init.ShaderResourcePath + platform + L"shaders.big", false);
    }
    NEngineInternal::GPShaderRenderManager->RegisterVertexShaderConstantLayout(1, &VSConstantLayoutBasic);
    NEngineInternal::GPShaderRenderManager->RegisterVertexShaderConstantLayout(2, &VSConstantLayoutLights);
    NEngineInternal::GPShaderRenderManager->RegisterVertexShaderConstantLayout(3, &VSConstantLayoutLightsBones);
    NEngineInternal::GPShaderRenderManager->RegisterVertexShaderConstantLayout(4, &VSConstantLayoutRepeatedMesh);
    NEngineInternal::GPShaderRenderManager->RegisterVertexShaderConstantLayout(5, &VSConstantLayoutParticles);
    if (init.InitFlags & 0x20) {
        EditorMeshBank.OpenFileReadWrite(CAFile::ConvertDirectoryAndFilenameToPath(init.MeshResourcePath, L"editmesh.bbb"), 1, 0x200);
        EditorMeshBank.SetTextureBank(MainGraphicBank);
        EditorMeshBank.SetClothMemoryPool(ClothMemoryPool);
    }
    CCharString suffix;
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant("PSCONST_ZERO", C4DVector(0.0f, 0.0f, 0.0f, 0.0f));
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant("PSCONST_ONE", C4DVector(1.0f, 1.0f, 1.0f, 1.0f));
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant("PSCONST_HALF", C4DVector(0.5f, 0.5f, 0.5f, 0.5f));
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant("PSCONST_1_0_0_0", C4DVector(1.0f, 0.0f, 0.0f, 0.0f));
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant("PSCONST_0_1_0_0", C4DVector(0.0f, 1.0f, 0.0f, 0.0f));
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant("PSCONST_0_0_1_0", C4DVector(0.0f, 0.0f, 1.0f, 0.0f));
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant("PSCONST_0_0_0_1", C4DVector(0.0f, 0.0f, 0.0f, 1.0f));
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant("PSCONST_SHADOW_FADE_COLOUR", C4DVector(0.5f, 0.5f, 0.5f, 0.5f));
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant("PSCONST_MAX_FOG_ALPHA", C4DVector(0.5f, 0.5f, 0.5f, 0.5f));
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant("PSCONST_TFACTOR", C4DVector(1.0f, 1.0f, 1.0f, 1.0f));
    N3DPrimitive::CVertexShaderInputStatic static_input(0, false);
    N3DPrimitive::CVertexShaderInputAnimated animated_input(0, false);
    SetupVertexShaderResource((EShaderBankNo)1, "SHADERS_STATIC" + suffix, static_input);
    SetupVertexShaderResource((EShaderBankNo)3, "SHADERS_PALSKIN" + suffix, animated_input);
    SetupVertexShaderResource((EShaderBankNo)0, "SHADERS_STATIC" + suffix, g_DeclarationStatic, (EVertexType)0x112);
    SetupVertexShaderResource((EShaderBankNo)12, "SHADERS_LANDSCAPE_FOREGROUND" + suffix, g_DeclarationLandscapeForeground, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)11, "SHADERS_LANDSCAPE_BACKGROUND" + suffix, g_DeclarationLandscapeBackground, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)13, "SHADERS_POS_COL_TEX1" + suffix, g_DeclarationPosColTex1, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)7, "SHADERS_WATER_FOREGROUND" + suffix, g_DeclarationWaterForeground, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)8, "SHADERS_WATER_BACKGROUND" + suffix, g_DeclarationWaterBackground, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)9, "SHADERS_SEA_BACKGROUND" + suffix, g_DeclarationSeaBackground, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)5, "SHADERS_SKY" + suffix, g_DeclarationPosColTex1, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)6, "SHADERS_SKY_SCREEN_SPACE" + suffix, g_Declaration2D, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)14, "SHADERS_REPEATED_MESH" + suffix, g_DeclarationRepeatedMesh, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)16, "SHADERS_POINT_SPRITE1" + suffix, g_DeclarationPosColTex1, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)24, "SHADERS_PARTICLE_SPRITE_TRAIL" + suffix, g_DeclarationPosColTex1, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)23, "SHADERS_MESH_GROUP" + suffix, g_DeclarationRepeatedMesh, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)21, "SHADER_SPRITE_GROUP" + suffix, g_DeclarationSpriteGroup, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)22, "SHADERS_DECAL_GROUP" + suffix, g_DeclarationDecalGroup, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)17, "SHADERS_ZSPRITE" + suffix, g_DeclarationZSprite, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)26, "SHADERS_TEXT" + suffix, g_DeclarationText, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)20, "SHADERS_VERTEX_POS" + suffix, g_DeclarationVertexPos, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)10, "SHADERS_WEATHER" + suffix, g_DeclarationWeather, (EVertexType)0);
    SetupVertexShaderResource((EShaderBankNo)25, "SHADERS_DEBUGGING" + suffix, g_DeclarationDebugging, (EVertexType)0);
    N3DPrimitive::CVertexShaderInputStatic static_bump_input(2, false);
    N3DPrimitive::CVertexShaderInputAnimated animated_bump_input(2, false);
    SetupVertexShaderResource((EShaderBankNo)2, "SHADERS_STATIC_BUMP" + suffix, static_bump_input);
    SetupVertexShaderResource((EShaderBankNo)4, "SHADERS_PALSKIN_BUMP" + suffix, animated_bump_input);
    ShaderBankPixel.OpenReadOnly("PIXEL_SHADERS" + suffix, 0x48);
    ShaderResourcePixel = NEngineInternal::GPShaderRenderManager->CreateResource();
    ShaderResourcePixel->OpenShaderBank(&ShaderBankPixel);
    CWideString unused_path;
    for (CMeshDataBank** mesh_bank = MeshBanks; mesh_bank != MeshBanks + 3; ++mesh_bank)
        *mesh_bank = 0;
    if (MainMeshBank) {
        EngineMeshBank.OpenReadOnly("MBANK_ENGINE", 0x48);
        MeshBanks[0] = MainMeshBank;
        MeshBanks[1] = &EditorMeshBank;
        MeshBanks[2] = &EngineMeshBank;
        MainMeshBank->SetMeshDataMemoryPool(MeshDataMemoryPool, true);
        EngineMeshBank.SetMeshDataMemoryPool(MeshDataMemoryPool, false);
    }
    CPixelFormat dxt1_format((_D3DFORMAT)0x31545844);
    CPixelFormat dxt3_format((_D3DFORMAT)0x33545844);
    long texture_size = CTextureManager::CalculateTextureSize(0x200, 0x200, true, dxt1_format);
    long hires_texture_memory = memory_init.HiresTextureMemory;
    if (g_pVideoOptions)
        hires_texture_memory = NEngineInternal::GPEngine->PeekVideoOptionsDef(g_pVideoOptions->TextureDetail).HiresTextureMemory;
    long texture_count = (hires_texture_memory << 10) / texture_size;
    long scratch_texture_count = texture_count / 16;
    CArray<CTextureManagerPoolInfo> pools(10);
    pools[0] = CTextureManagerPoolInfo(dxt1_format, 32, 32, 128, true);
    pools[1] = CTextureManagerPoolInfo(dxt3_format, 32, 32, 64, true);
    pools[2] = CTextureManagerPoolInfo(dxt1_format, 64, 64, 64, true);
    pools[3] = CTextureManagerPoolInfo(dxt3_format, 64, 64, 64, true);
    pools[4] = CTextureManagerPoolInfo(dxt1_format, 128, 128, 64, true);
    pools[5] = CTextureManagerPoolInfo(dxt3_format, 128, 128, 64, true);
    pools[6] = CTextureManagerPoolInfo(dxt1_format, 256, 256, 128, true);
    pools[7] = CTextureManagerPoolInfo(dxt3_format, 256, 256, 24, true);
    pools[8] = CTextureManagerPoolInfo(dxt1_format, 512, 512, texture_count, true);
    pools[9] = CTextureManagerPoolInfo(dxt3_format, 512, 512, 0, true);
    CPixelFormat opaque_format = MainGraphicBank->GetPixelFormat(CGraphicDataBank::TEXTURE_TYPE_UNCOMPRESSED_OPAQUE);
    pools.push_back(CTextureManagerPoolInfo(MainGraphicBank->GetPixelFormat(CGraphicDataBank::TEXTURE_TYPE_UNCOMPRESSED_ALPHA), 32, 32, 64, true));
    pools.push_back(CTextureManagerPoolInfo(CPixelFormat((_D3DFORMAT)0x15), 512, 512, 1, false));
    pools.push_back(CTextureManagerPoolInfo(opaque_format, 512, 512, scratch_texture_count, true));
    if (!MainGraphicBank->GetTextureManager().IsInitialised())
        MainGraphicBank->GetTextureManager().Initialise(pools);
    if (MainMeshBank) {
        MainMeshBank->SetAnimationMemoryPool(AnimationMemoryPool, true);
        MainMeshBank->SetClothMemoryPool(ClothMemoryPool);
        MainMeshBank->SetLoadingMemoryPool(LoadingMemoryPool);
        MainMeshBank->SetLoadingMemoryFailureHandler(LoadingMemoryFailureHandler);
    }
    MainGraphicBank->SetLoadingMemoryPool(LoadingMemoryPool);
    MainGraphicBank->SetLoadingMemoryFailureHandler(LoadingMemoryFailureHandler);
    if (!MeshDataMemoryPool.IsNull())
        EditorMeshBank.SetMeshDataMemoryPool(MeshDataMemoryPool, false);
    if (!AnimationMemoryPool.IsNull())
        EditorMeshBank.SetAnimationMemoryPool(AnimationMemoryPool, false);
    if (!ClothMemoryPool.IsNull())
        EditorMeshBank.SetClothMemoryPool(ClothMemoryPool);
    if (!LoadingMemoryPool.IsNull())
        EditorMeshBank.SetLoadingMemoryPool(LoadingMemoryPool);
    if (!LoadingMemoryFailureHandler.IsNull())
        EditorMeshBank.SetLoadingMemoryFailureHandler(LoadingMemoryFailureHandler);
    for (CGraphicDataBank** graphic_bank = GraphicBanks; graphic_bank != GraphicBanks + 1; ++graphic_bank)
        *graphic_bank = 0;
    GraphicBanks[0] = MainGraphicBank;
    SetupAlphaStippleTexture((init.InitFlags & 4) != 0);
    CPixelFormat alpha_format;
    NEngineInternal::GPDisplayManager->GetBestAlphaTextureFormat(32, alpha_format);
    ZeroRGBATexture = CTexture(C2DExtentsI(1, 1), 1, alpha_format, 0, (ESurfacePool)1);
    ZeroRGBATexture.GetSurfaceLevel(0).Clear(CRGBColour(0, 0, 0, 0));
    SatRGBATexture = CTexture(C2DExtentsI(1, 1), 1, alpha_format, 0, (ESurfacePool)1);
    SatRGBATexture.GetSurfaceLevel(0).Clear(CRGBColour(255, 255, 255, 255));
    GPUScratchBufferTextureManager.AddScratchPool(0);
    CAsyncLoadingMemoryPool::AddLoadingCallback(this);
}
