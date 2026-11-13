// CEnginePrimitiveManagerWeaponTrails::RenderWeaponTrail(const CEngineInternalPrimitiveWeaponTrail& trail) -- draw a weapon trail (two point strips, blade base and tip) as one Hermite-interpolated triangle strip, faded per segment by age
// Grade: DIFFER(3207v3205) -- 968/1078 exact (1043 with frame offsets masked): frame slot permutation (GFFloatToLongFloor `one` + vertex_count slot retail 0x34, ours 0x54; 0x38..0x54 shifted 4; 0x5c<->0x60), `lea edx` v ours `lea ecx` at 0x00BCCDA9, ReturnResource tail 0x00BCD32D..0x00BCD3DF regalloc (retail tree ebx / key edi / node in the dead arg word [ebp+7Ch]; ours tree edi / key spilled to [ebp+7Ch] / node ebx spilled to [ebp+54h] -- that spill shares and promotes the `one`/vertex_count slot)
// Leaf: ?RenderWeaponTrail@CEnginePrimitiveManagerWeaponTrails@@AAEXABVCEngineInternalPrimitiveWeaponTrail@@@Z
// Retail-asm: 0x00BCC96A..0x00BCC992 `mov [ebp+68h],0.5f ; mov [ebp+34h],1.0f ; fld [ebp+58h] ; fsub [ebp+68h] ; fistp dword ptr [ebp+70h] ; fild [ebp+70h] ; fadd [ebp+34h] ; fstp [ebp+60h] ; mov eax,[ebp+58h] ; cmp eax,[ebp+60h] ; jne ; inc dword ptr [ebp+70h]` -- the engine's inline-assembly floor helper (engine/GFFloatToLongFloor.h) expanded in place for SkipPoints; VC7.1 emits no bare dword fistp from C++.
// Retail-asm: 0x00BCCF70..0x00BCCF79 `fld [ebp+68h] ; fistp dword ptr [ebp-2Ch] ; ... mov eax,[ebp-2Ch]` -- engine/GFFloatToLongNear.h (round-to-nearest fistp of the faded alpha) expanded in the vertex loop.
// Callees: GetFrame -> 0x009FDE80 / 0x009FE380 / 0x009FCBA0, CResource list 0x009FBFB0 / 0x009FBF90, ApplyVertexShader 0x00988020, ResetWorldTransform 0x00988290, CArray<C3DVector>(n) 0x007C2AB0, PrepareForRender set-up 0x0098B930 / 0x00988A20 / 0x00989BF0 / 0x00989760 / 0x009897C0 / 0x0098A760 / 0x00988A50, FindNode 0x00BC9B30, AddNewKey 0x00BF3030, BuildNewBuffer 0x00BF3380, IsBusy 0x00AC20C0, Sizeof 0x00AC20D0, Lock 0x00AC20A0, Unlock 0x00AC20B0, RealiseRenderState 0x00A058C0, RestoreCaptureBlock 0x00A05840, operator new 0x00BFEA1A / delete 0x00BFE9BC / free 0x00BFEA14. Retail 0x00BCC800, 3205 bytes; no flags.
#include "fable_compiler.h"
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CVERTEXBUFFERBASE_ISBUSY_SIZEOF
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_DYNAMICVB
#define FABLE_RETAIL_CTVERTEXCOLTEX1_SETTERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DYNAMICVB
#define FABLE_RETAIL_CSHADERRENDERMANAGER_DRAWNONINDEXED_TEXTURES
#define FABLE_RETAIL_CENGINETREEFILER_INLINES
#define FABLE_RETAIL_CENGINEVERTEXBUFFERPOOL_INLINES
#include "engine/C3DVector.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CArray.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CVertexShader.h"
#include "engine/CTVertexColTex1.h"
#include "engine/EVertexType.h"
#include "engine/EPrimitiveType.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CCountedPointer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/CEngineVertexBufferPool.h"
#include "engine/CEnginePrimitiveWeaponTrail.h"

template <class T> FABLE_FORCEINLINE T Min(T a, T b) { return b < a ? b : a; }
template <class T> FABLE_FORCEINLINE T Max(T a, T b) { return b < a ? a : b; }

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; }   // 0x01436E78

FABLE_FORCEINLINE void CShaderRenderManager::SelectVertexBuffer(CVertexBufferWin32& vertexBuffer)
{
    D3DDevice->SetStreamSource(0, vertexBuffer.PVertexBuffer, 0, vertexBuffer.Stride);
}
FABLE_FORCEINLINE void CShaderRenderManager::DrawBuffersNonIndexed(CVertexBufferWin32& vertexBuffer, unsigned long startVertex,
                                                               unsigned long primitiveCount, EPrimitiveType primitiveType)
{
    SelectVertexBuffer(vertexBuffer);
    RenderManager->RealiseRenderState();
    D3DDevice->DrawPrimitive(primitiveType, startVertex, primitiveCount);
}
FABLE_FORCEINLINE void CShaderRenderManager::DrawBuffersNonIndexed(const CTexture** const textures, unsigned long textureCount,
                                                               CVertexBufferWin32& vertexBuffer, unsigned long startVertex,
                                                               unsigned long primitiveCount, EPrimitiveType primitiveType)
{
    for (unsigned long i = 0; i < textureCount; ++i)
        RenderManager->AttachTextureToStage(i, textures[i]);
    DrawBuffersNonIndexed(vertexBuffer, startVertex, primitiveCount, primitiveType);
}
FABLE_FORCEINLINE void CRenderManagerCore::FlushRender()
{
    SelectVertexBuffer(0, 0);
    PD3DDevice->SetIndices(0);
    for (long stage = 0; stage < TextureStagesInUse; ++stage) {
        if (CurrentlySelectedTextures[stage]) {
            CurrentlySelectedTextures[stage] = 0;
            PD3DDevice->SetTexture(stage, 0);
        }
    }
    TextureStagesInUse = 0;
}

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }    // 0x01436E98
FABLE_FORCEINLINE const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic, unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}

FABLE_FORCEINLINE unsigned long __fastcall CEnginePrimitiveRenderUtil::GetDynamicVBUsage() { return NEngineInternal::GPPrimitiveRendererSharedData->DynamicVBUsage; }
FABLE_FORCEINLINE EVertexBufferPool __fastcall CEnginePrimitiveRenderUtil::GetDynamicVBPool() { return NEngineInternal::GPPrimitiveRendererSharedData->DynamicVBPool; }
FABLE_FORCEINLINE unsigned long __fastcall CEnginePrimitiveRenderUtil::GetDynamicVBLockFlags() { return NEngineInternal::GPPrimitiveRendererSharedData->DynamicVBLockFlags; }

// CEngineInternalPrimitiveWeaponTrail (Ego_r.pdb; retail 4 below the PDB past the 12-byte Trail vector; 0x7c bytes as the landed
// CreateInternalPrimitive 0x00BCD600 allocates).
class CEngineInternalPrimitiveWeaponTrail : public CEngineInternalPrimitiveBase {
public:
    CEnginePrimitiveWeaponTrail::EWeaponTrailType TrailType; // +0x48
    unsigned long              GraphicIndex;             // +0x4c
    unsigned long              GraphicFrame;             // +0x50
    unsigned char              Alpha;                    // +0x54
    float                      SkipPoints;               // +0x58
    CArray<CArray<C3DVector> > Trail;                    // +0x5c
    CArray<float>              TrailElementCreationTime; // +0x68
    float                      CurrentTime;              // +0x74
    float                      FallOffTime;              // +0x78
};

class CEnginePrimitiveManagerWeaponTrails : public CEnginePrimitiveManager {
public:
    CVertexShader           VertexShader;               // +0x0c
    CEngineVertexBufferPool VertexBufferPool;           // +0x14
private:
    void RenderWeaponTrail(const CEngineInternalPrimitiveWeaponTrail& trail);
};

void CEnginePrimitiveManagerWeaponTrails::RenderWeaponTrail(const CEngineInternalPrimitiveWeaponTrail& trail)
{
    CGraphicFrame frame;
    if (!NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(trail.GraphicIndex, trail.GraphicFrame)
             ->GetFrame(frame, 0, 0))
        return;
    long trail_size = trail.Trail[0].size();
    long skip = GFFloatToLongFloor(trail.SkipPoints);
    if (skip < 0)
        skip = 0;
    long points = trail_size - skip;
    if (points < 2)
        return;
    const CTexture* texture = frame.GetTexture();
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShader, false);
    NEngineInternal::GPShaderRenderManager->ResetWorldTransform();
    float fraction = trail.SkipPoints - skip;
    if (fraction < 0.0f)
        fraction = 0.0f;
    long segments = points - 1;
    long vertex_count = segments * 16 + 2;

    CArray<C3DVector> tangents0(points);
    CArray<C3DVector> tangents1(points);
    for (long i = 0; i < points; ++i) {
        long n = points - i;
        long next = Min(trail_size - 1, n);
        long previous = Max(n - 2, 0L);
        float scale = next == previous + 2 ? 0.5f : 1.0f;
        tangents0[n - 1] = (trail.Trail[0][previous] - trail.Trail[0][next]) * scale;
        tangents1[n - 1] = (trail.Trail[1][previous] - trail.Trail[1][next]) * scale;
    }

    CRenderStateScopedCaptureBlock capture_block;
    NEngineInternal::GPRenderManager->SetFogEnable(false);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(1);
    CEngineDynamic3DResource<CVertexBufferWin32, 3>* buffer = VertexBufferPool.GetBuffer(VERTEX_NORM_COL_SPEC_TEX1, vertex_count, sizeof(CTVertexColTex1),
                                                                         GetDynamicVBUsage(), GetDynamicVBPool());
    CTVertexColTex1* vertices = (CTVertexColTex1*)buffer->Lock(GetDynamicVBLockFlags(), 0, 0);

    unsigned long first_step = 0;
    float current_time = trail.CurrentTime;
    float fall_off_time = trail.FallOffTime;
    for (long segment = 0; segment < segments; ++segment) {
        long n = points - segment;
        long current = n - 1;
        long previous = Max(n - 2, 0L);
        float alpha = (1.0f - (current_time - trail.TrailElementCreationTime[current]) / fall_off_time) * trail.Alpha;
        if (alpha < 0.0f)
            alpha = 0.0f;
        for (unsigned long step = first_step; step <= 8; ++step) {
            long a = GFFloatToLongNear(alpha);
            unsigned long colour = 0xff000000 | (a << 16) | (a << 8) | a;
            float t = step * 0.125f;
            if (segment == 0) {
                if (t < fraction)
                    t = fraction;
            }
            float t2 = t * t;
            float t3 = t2 * t;
            float h1 = 2.0f * t3 - 3.0f * t2 + 1.0f;
            float h2 = 3.0f * t2 - 2.0f * t3;
            float h3 = t3 - 2.0f * t2 + t;
            float h4 = t3 - t2;
            C3DVector position0 = trail.Trail[0][current] * h1 + trail.Trail[0][previous] * h2 + tangents0[current] * h3 + tangents0[previous] * h4;
            C3DVector position1 = trail.Trail[1][current] * h1 + trail.Trail[1][previous] * h2 + tangents1[current] * h3 + tangents1[previous] * h4;
            float v = (segment + t - fraction) * 0.25f;
            CTVertexColTex1* vertex0 = vertices++;
            CTVertexColTex1* vertex1 = vertices++;
            vertex0->SetPosition(position0);
            vertex0->SetDiffuseColour(colour);
            vertex0->SetU1V1(0.99f, v);
            vertex1->SetPosition(position1);
            vertex1->SetDiffuseColour(colour);
            vertex1->SetU1V1(0.01f, v);
        }
        first_step = 1;
    }

    buffer->Unlock();
    NEngineInternal::GPShaderRenderManager->DrawBuffersNonIndexed(&texture, 1, *buffer, 0, vertex_count - 2, PT_TRIANGLE_STRIP);
    NEngineInternal::GPRenderManager->FlushRender();
    VertexBufferPool.ReturnResource(buffer);
}
