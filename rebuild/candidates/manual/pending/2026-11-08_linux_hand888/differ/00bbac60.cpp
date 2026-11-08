// CEnginePrimitiveManagerDecal::RenderLandscapeDecal(CEngineInternalPrimitiveDecal& decal) -- draw a landscape decal: fetch its frame from graphic bank 0, open a render-state capture block with shaders on, set the blend state, the colour constant and the clamp-to-border sampler states, build a unit-grid mesh over the decal's footprint (alternating diagonals), light it and draw it as indexed triangles
// Residue: DIFFER(3823v3743), 87 normalized diff lines / 73 register+frame-blind after 22 graded sources (session 2): (1) the BlendOp SetBlendOp expansions: retail merges all three cases into one SetRenderState tail with the bookmark mask loaded first (`mov eax,[eax+2814h] ; mov esi,[edx+8] ; test esi,eax`); ours commutes the default/REVSUBTRACT copy (`mov esi,[edx+8] ; mov eax,[eax+2814h] ; test eax,esi`) so c2 cannot cross-jump it (+10 lines, the CRenderStateManager.h lane gap); (2) the colour constant: retail builds the CRGBFloatColour in place at [ebp-54h] and converts into the C4DVector at [ebp-64h] (premultiply keeps A in st and forwards R*A/G*A); ours assigns a CRGBFloatColour temporary at [ebp-64h] into the named colour (+4 copies per path, +80 bytes); the C4DVector-outer form (probe dc_c3) gets that region right but drops `vertex` from packer weight 3 to 2 (pk.sh), which re-packs the frame (vertex with channels) and moves vertex into edx; (3) the index loop else-arm leas take corner first (`lea eax,[ebx+eax+2]` v `[eax+ebx+2]`).
// Next: get the C4DVector-outer colour (dc_c3 / dc_b0 shape) without losing the `vertex` weight: try keeping the fill counter memory-resident through a different loop spelling (`vertices[vertex].X` via a `CTVertexNormTex1* first` re-read, or the counter updated by `vertex += width + 1` per row as retail strength-reduces it), then re-check `./slots2.sh` against retail (-0x64 C4DVector, -0x54 CRGBFloatColour, -0x2c vertex_count, -0x24 channels, -0x1c triangle_count/vertex); else-arm lea order: try `(width + 2) + corner`.
// Leaf: ?RenderLandscapeDecal@CEnginePrimitiveManagerDecal@@IAEXAAVCEngineInternalPrimitiveDecal@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBAC60, length 3743 bytes, `ret 4`. Ego_r.pdb / FableWin.pdb
//   ?RenderLandscapeDecal@CEnginePrimitiveManagerDecal@@IAEXAAVCEngineInternalPrimitiveDecal@@@Z (protected).
// Callees: CBankFile::EntryExists 0x009CC190, CGraphicInfo::GetFrame 0x009FF8D0 (?GetFrame@CGraphicInfo@@QBE_NAAVCGraphicFrame@@KKK@Z),
//   CGraphicFrameData::Uninitialise 0x009FCBA0, operator delete 0x00BFE9BC, vector<CTVertexNormTex1>(n) 0x00BB9FD0 (same TU),
//   vector<unsigned short>(n) 0x00A8D750, __ftol2 0x00BFEA70, CEngineLightingManager::GetLightsInSphereFast 0x00B46B90 /
//   SetupShaderPointLights 0x00B44F20 / UpdateGlobalLightingStates 0x00B46C80, CShaderRenderManager::ApplyPixelShader 0x00988140 /
//   ApplyVertexShader 0x00988020 / ResetWorldTransform 0x00988290 and the PrepareForRender(9) helpers, CRenderManagerCore::
//   DrawAsIndexedTris 0x00A0AF40, free 0x00BFEA14, the scoped shader enable (0x00987FE0 / 0x00988110 / 0x009880E0 / 0x00988190) and
//   CRenderStateManager::RestoreCaptureBlock 0x00A05840.
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_LIGHTSINSPHEREFAST
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS_USER
#define FABLE_RETAIL_CRENDERSTATEMANAGER_BORDERCOLOUR
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR
#include "fable_compiler.h"
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CArray.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/C4DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFloatToLongCeil.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
extern CEngineLightingManager* g_pEngineLightingManager;                           // 0x01436E9C
void __cdecl operator delete(void* block);                                         // 0x00BFE9BC

// Graphic frames (the landed GetFrame 0x0041C260 / RenderSpritePrim models; Ego_r.pdb layouts).
struct CCPInfo { long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
struct CTexturePointer {                                                 // CCountedPointer<CTexture>
    CTexture* Data;
    CCPInfo*  Info;
    FABLE_FORCEINLINE ~CTexturePointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CGraphicFrameData { public:                                        // Ego_r.pdb, 27 bytes
    unsigned long   ReferenceCount;                                      // +0x00
    void*           Data;                                                // +0x04
    CTexturePointer PTexture;                                            // +0x08
    unsigned short  TLX, TLY, Width, Height, Pitch;                      // +0x10
    unsigned char   Alpha : 1;                                           // +0x1a
    unsigned char   BooleanAlpha : 1;
    unsigned char   UsesSurface : 1;
    unsigned char   SectionOfTPage : 1;
    unsigned char   IsInitialised : 1;
    unsigned char   ScreenGraphic : 1;
    void Uninitialise();                                                 // 0x009FCBA0
    FABLE_FORCEINLINE ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
    FABLE_FORCEINLINE void DecRef()
    {
        if (--ReferenceCount == 0) {
            this->~CGraphicFrameData();
            operator delete(this);
        }
    }
};
class CGraphicFrame { public:                                            // intrusive pointer to CGraphicFrameData
    CGraphicFrameData* FrameData;
    CGraphicFrame() : FrameData(0) { }
    FABLE_FORCEINLINE ~CGraphicFrame() { if (FrameData) FrameData->DecRef(); }
    CGraphicFrameData* operator->() const { return FrameData; }
};
class CGraphicInfo { public:                                             // Ego_r.pdb, 0x2c bytes
    unsigned char _pad00[0x2c];
    bool GetFrame(CGraphicFrame& frame, unsigned long frameNum, unsigned long mipmapLevel, unsigned long flags) const;   // 0x009FF8D0
};
template<class T> struct SVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    FABLE_FORCEINLINE unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    FABLE_FORCEINLINE const T& operator[](unsigned long n) const { return *(_M_start + n); }
};
class CGraphicDataBankFull { public:                                     // CGraphicDataBankFull : CGraphicDataBank : CBankFile (+0)
    bool EntryExists(unsigned long index) const;                         // 0x009CC190 ?EntryExists@CBankFile@@QBE_NK@Z
    unsigned char         _pad00[0x1e0];
    SVector<CGraphicInfo> GraphicList;                                   // +0x1e0
    FABLE_FORCEINLINE const CGraphicInfo* PeekGraphic(unsigned long index) const { return index < GraphicList.size() ? &GraphicList[index] : 0; }   // ?PeekGraphic@CGraphicDataBank@@QBEPBVCGraphicInfo@@K@Z
};
enum EGraphicBankNo { };                                                  // no enumerator name recovered (engine/CEngineResourceManager.h)
struct CDecalResourceManager {                                           // CEngineResourceManager 0x01436E98
    unsigned char         _pad00[0x954];
    CGraphicDataBankFull* GraphicBanks[1];                               // +0x954
    const CGraphicDataBankFull* PeekGraphicBank(EGraphicBankNo bank) const { return GraphicBanks[bank]; }   // ?PeekGraphicBank@CEngineResourceManager@@QBEPBVCGraphicDataBank@@W4EGraphicBankNo@@@Z
};
extern CDecalResourceManager* g_pResourceManager;                        // 0x01436E98

class CTVertexNormTex1Base { public:                                     // Ego_r.pdb, 0x20
    float X, Y, Z;
    float NormalX, NormalY, NormalZ;
    float U1, V1;
};
class CTVertexNormTex1 : public CTVertexNormTex1Base { public:
    CTVertexNormTex1() { }
};
class CEngineLocalLightArray { public:                                   // Ego_r.pdb, 0x1a4
    unsigned long LightCount;                                            // +0x00
};

enum EEnginePrimitiveBlendMode { };
enum EEnginePrimitiveBlendOp { };
class CEngineInternalPrimitiveDecal : public CEngineInternalPrimitiveBase { public:   // Ego_r.pdb, 0x94
    bool                      LandscapeOnlyDecal;                        // +0x48
    long                      DecalIndex;                                // +0x4c
    long                      DecalFrame;                                // +0x50
    C3DVector                 WorldPos;                                  // +0x54
    C2DVector                 Size;                                      // +0x60
    CRGBColour                Colour;                                    // +0x68
    CRightHandedSet           RHSet;                                     // +0x6c
    EEnginePrimitiveBlendMode BlendMode;                                 // +0x84
    EEnginePrimitiveBlendOp   BlendOp;                                   // +0x88
    bool                      Lit;                                       // +0x8c
    float                     StencilCubeHeight;                         // +0x90
};

class CEnginePrimitiveManagerDecal : public CEnginePrimitiveManager {
protected:
    void RenderLandscapeDecal(CEngineInternalPrimitiveDecal& decal);
    unsigned char _pad0c[0x54 - 0xc];                                    // +0x0c MeshDecalVertexBuffer, MeshDecalIndexBuffer
    unsigned long PointLightShaderIndex[6];                              // +0x54
    CVertexShader LandscapeDecalVertexShader[3];                         // +0x6c
    CVertexShader MeshDecalVertexShader[3];                              // +0x84
    CVertexShader MeshDecalTwoPassStencilVertexShader;                   // +0x9c
    CVertexShader MeshDecalOnePassStencilVertexShader;                   // +0xa4
    CVertexShader MeshDecalStencilClearVertexShader;                     // +0xac
    CPixelShader  LandscapeDecalPixelShader;                             // +0xb4
};

void CEnginePrimitiveManagerDecal::RenderLandscapeDecal(CEngineInternalPrimitiveDecal& decal)
{
    if (!g_pResourceManager->PeekGraphicBank((EGraphicBankNo)0)->EntryExists(decal.DecalIndex))
        return;
    const CGraphicInfo* info = g_pResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphic(decal.DecalIndex);
    CGraphicFrame frame;
    if (!info->GetFrame(frame, decal.DecalFrame, 0, 0))
        return;

    CRenderStateScopedCaptureBlock capture;
    CScopedShadersEnable shaders(true, true);
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, frame->PTexture.Data);
    switch (decal.BlendOp) {
    case 1:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_SUBTRACT);
        break;
    case 2:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_REVSUBTRACT);
        break;
    default:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_ADD);
        break;
    }
    bool premultiply = false;
    switch (decal.BlendMode) {
    case 3:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
        premultiply = true;
        break;
    case 4:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_COLOR);
        premultiply = true;
        break;
    default:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
        break;
    }
    CRGBFloatColour colour;
    if (premultiply) {
        colour = CRGBFloatColour(decal.Colour);
        colour.R *= colour.A;
        colour.G *= colour.A;
        colour.B *= colour.A;
    } else {
        colour = CRGBFloatColour(decal.Colour);
    }
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(2, C4DVector(colour));

    long min_x = GFFloatToLongFloor(decal.WorldPos.X - decal.Size.X * 0.5f);
    long min_y = GFFloatToLongFloor(decal.WorldPos.Y - decal.Size.Y * 0.5f);
    long max_x = GFFloatToLongCeil(decal.WorldPos.X + decal.Size.X * 0.5f);
    long max_y = GFFloatToLongCeil(decal.WorldPos.Y + decal.Size.Y * 0.5f);
    long width = max_x - min_x;
    long height = max_y - min_y;
    long vertex_count = (height + 1) * (width + 1);
    CArray<CTVertexNormTex1> vertices(vertex_count);
    long vertex = 0;
    for (long y = min_y; y <= max_y; ++y) {
        for (long x = min_x; x <= max_x; ++x) {
            vertices[vertex].X = (float)x;
            vertices[vertex].Y = (float)y;
            vertices[vertex].Z = decal.WorldPos.Z;
            vertices[vertex].NormalX = decal.RHSet.GetUp().X;
            vertices[vertex].NormalY = decal.RHSet.GetUp().Y;
            vertices[vertex].NormalZ = decal.RHSet.GetUp().Z;
            vertices[vertex].U1 = (vertices[vertex].X - decal.WorldPos.X) / decal.Size.X + 0.5f;
            vertices[vertex].V1 = (vertices[vertex].Y - decal.WorldPos.Y) / decal.Size.Y + 0.5f;
            ++vertex;
        }
    }
    long triangle_count = height * width * 2;
    CArray<unsigned short> indices(triangle_count * 3);
    long index = 0;
    long corner = 0;
    for (long triangle = 0; triangle < triangle_count; triangle += 2) {
        if ((long)(vertices[corner].Y + vertices[corner].X) & 1) {
            indices[index++] = (unsigned short)corner;
            indices[index++] = (unsigned short)(width + corner + 1);
            indices[index++] = (unsigned short)(corner + 1);
            indices[index++] = (unsigned short)(corner + 1);
            indices[index++] = (unsigned short)(width + corner + 1);
            indices[index++] = (unsigned short)(width + corner + 2);
        } else {
            indices[index++] = (unsigned short)corner;
            indices[index++] = (unsigned short)(width + corner + 2);
            indices[index++] = (unsigned short)(corner + 1);
            indices[index++] = (unsigned short)corner;
            indices[index++] = (unsigned short)(width + corner + 1);
            indices[index++] = (unsigned short)(width + corner + 2);
        }
        ++corner;
        if ((corner + 1) % (width + 1) == 0)
            ++corner;
    }

    long light_count;
    if (decal.Lit) {
        CEnginePrimitiveThemeChannelDesc channels;
        channels = CEnginePrimitiveThemeChannelDesc();
        CEngineLocalLightArray* lights = g_pEngineLightingManager->GetLightsInSphereFast(
            C3DBoundingSphere(decal.WorldPos, decal.Size.GetMagnitude()), 5, channels);

        light_count = lights->LightCount;
        g_pEngineLightingManager->SetupShaderPointLights(*lights);
    } else {
        light_count = 0;
        g_pEngineLightingManager->SetSelfIlluminationAmbient(100);
    }
    g_pEngineLightingManager->UpdateGlobalLightingStates();
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_BORDER);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_BORDER);
    NEngineInternal::GPRenderManager->SetTextureBorderColour(0, 0);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetZBufferEnable(true);
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaRef(1);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(LandscapeDecalPixelShader);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(LandscapeDecalVertexShader[PointLightShaderIndex[light_count]], false);
    NEngineInternal::GPShaderRenderManager->ResetWorldTransform();
    NEngineInternal::GPShaderRenderManager->PrepareForRender(9);
    NEngineInternal::GPRenderManager->DrawAsIndexedTris(vertices.GetAsCArray(), sizeof(CTVertexNormTex1), vertex_count,
                                                        indices.GetAsCArray(), triangle_count);
    g_pEngineLightingManager->RestoreDefaultPrimitiveParameters();
}
