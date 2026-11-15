// CEnginePrimitiveManagerSpriteTrails::RenderSpriteTrail(const CEngineInternalPrimitiveSpriteTrail& trail, bool premultiplied) -- draw a sprite trail as one camera-facing triangle strip
// Grade: DIFFER(3757v3749) -- 9 structural hunks (wfE5 successor; was 27): vertex_count computed before the transform copy (retail interleaves it with the rep movsd, 0x00BCAA4F), first_step declared before current (store order 0x00BCAF41) and loop bound `segment < segments`. Left: (1) retail rematerialises points-1 (`lea eax,[ebx-1]` 0x00BCAA4F) where we reuse ecx (CSE of segments); (2) the outer-loop pointer IVs for tangents[current].Y/.Z at 0x00BCB215 (we use [edx+4]/[edx+8] for the subtraction, the IVs for the add); (3) three alignment pads (lea esp,[esp] 0x00BCAA89 in retail; ours elsewhere) = the 8-byte gap. Tried: k=1..6 unused locals (no move), Min / vertex_count / loop spellings with and without `segments`, segments at four positions, unsigned / const / reordered vertex_count, four direction spellings.
// Leaf: ?RenderSpriteTrail@CEnginePrimitiveManagerSpriteTrails@@QAEXABVCEngineInternalPrimitiveSpriteTrail@@_N@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CLASS
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM
#define FABLE_RETAIL_CENGINECAMERA_PEEKNONHOMOGENOUSVIEWTRANSFORM
#define FABLE_RETAIL_CENGINECAMERA_PEEKINVERSENONHOMOGENOUSVIEWTRANSFORM
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
#define FABLE_RETAIL_CENGINETREEFILER_FINDNODE_INDEX
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
#include "engine/CMatrix3x4.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CEngineCamera.h"
class CEnginePrimitive;
#include "engine/CEngineInternalPrimitiveSpriteTrail.h"
#include "engine/CEnginePrimitiveManagerSpriteTrails.h"

template <class T> FABLE_FORCEINLINE T Min(T a, T b) { return b < a ? b : a; }
template <class T> FABLE_FORCEINLINE T Max(T a, T b) { return b < a ? a : b; }

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
extern const float DELTA_NOTIONAL_ZERO;                                            // 0x0129BA3C (1e-4f)
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                      // 0x01436EA0
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

void CEnginePrimitiveManagerSpriteTrails::RenderSpriteTrail(const CEngineInternalPrimitiveSpriteTrail& trail, bool premultiplied)
{
    CRenderStateScopedCaptureBlock capture_block;
    long trail_size = trail.Trail.size();
    long skip = GFFloatToLongFloor(trail.SkipPoints);
    if (skip < 0)
        skip = 0;
    long points = trail_size - skip;
    if (points < 2)
        return;
    long start = trail.NextPos % trail_size;
    float fraction = trail.SkipPoints - skip;
    if (fraction < 0.0f)
        fraction = 0.0f;
    long segments = points - 1;
    float length = Min((float)(trail.MaxLength - 2), segments - fraction);
    if (length < DELTA_NOTIONAL_ZERO)
        return;

    long vertex_count = segments * 16 + 2;
    CMatrix3x4 transform = trail.WorldMat;
    CArray<C3DVector> tangents(points);
    for (long i = 0; i < points; ++i) {
        long n = points - i;
        long next = Min(trail_size - 1, n);
        long previous = Max(n - 2, 0L);
        float scale = next == previous + 2 ? 0.5f : 1.0f;
        long index = (n + start - 1) % trail_size;
        tangents[index] = (trail.Trail[(previous + start) % trail_size].Position - trail.Trail[(next + start) % trail_size].Position) * scale;
        transform.TransformVector(tangents[index]);
    }

    NEngineInternal::GPShaderRenderManager->PrepareForRender(1);
    CEngineDynamic3DResource<CVertexBufferWin32, 3>* buffer = VertexBufferPool.GetBuffer(VERTEX_NORM_COL_SPEC_TEX1, vertex_count, sizeof(CTVertexColTex1),
                                                                         GetDynamicVBUsage(), GetDynamicVBPool());
    CTVertexColTex1* vertices = (CTVertexColTex1*)buffer->Lock(GetDynamicVBLockFlags(), 0, 0);

    CRGBFloatColour colours[3];
    colours[0].Initialise(trail.HeadColour.R * (1.0f / 255.0f), trail.HeadColour.G * (1.0f / 255.0f), trail.HeadColour.B * (1.0f / 255.0f),
                          trail.HeadColour.A * (1.0f / 255.0f));
    colours[0].A *= 2.0f;
    colours[1].Initialise(trail.MidColour.R * (1.0f / 255.0f), trail.MidColour.G * (1.0f / 255.0f), trail.MidColour.B * (1.0f / 255.0f),
                          trail.MidColour.A * (1.0f / 255.0f));
    colours[1].A *= 2.0f;
    colours[2].Initialise(trail.TailColour.R * (1.0f / 255.0f), trail.TailColour.G * (1.0f / 255.0f), trail.TailColour.B * (1.0f / 255.0f),
                          trail.TailColour.A * (1.0f / 255.0f));
    colours[2].A *= 2.0f;
    if (premultiplied) {
        colours[0].R *= colours[0].A;
        colours[0].G *= colours[0].A;
        colours[0].B *= colours[0].A;
        colours[1].R *= colours[1].A;
        colours[1].G *= colours[1].A;
        colours[1].B *= colours[1].A;
        colours[2].R *= colours[2].A;
        colours[2].G *= colours[2].A;
        colours[2].B *= colours[2].A;
    }

    float one_over_length = 1.0f / length;
    long first_step = 0;
    long current = start - 1;
    long previous = current - 1;
    for (long segment = 0; segment < segments; ++segment) {
        if (current < 0)
            current += trail_size;
        if (previous < 0)
            previous += trail_size;
        for (long step = first_step; step <= 8; ++step) {
            float t = step * 0.125f;
            if (segment == 0) {
                if (t < fraction)
                    t = fraction;
            } else if (segment == trail.MaxLength - 2) {
                if (t > fraction)
                    t = fraction;
            }
            float t2 = t * t;
            float t3 = t2 * t;
            float h1 = 2.0f * t3 - 3.0f * t2 + 1.0f;
            float h2 = 3.0f * t2 - 2.0f * t3;
            float h3 = t3 - 2.0f * t2 + t;
            float h4 = t3 - t2;
            C3DVector position = trail.Trail[current].Position * h1 + trail.Trail[previous].Position * h2 + tangents[current] * h3 +
                                 tangents[previous] * h4;
            transform.TransformPoint(position);
            float v = (segment + t - fraction) * one_over_length;

            C3DVector direction = (tangents[previous] - tangents[current]) * t + tangents[current];
            NEngineInternal::GPCamera->PeekNonHomogenousViewTransform().TransformVector(direction);
            direction = C3DVector(direction.Y, -direction.X, 0.0f);
            NEngineInternal::GPCamera->PeekInverseNonHomogenousViewTransform().TransformVector(direction);
            float side_length = direction.GetMagnitude();
            if (side_length > DELTA_NOTIONAL_ZERO)
                direction *= trail.Width / side_length;
            C3DVector left = position - direction;
            C3DVector right = position + direction;

            long colour_index = v < 0.5f ? 0 : 1;
            float blend = v * 2.0f - colour_index;
            CRGBFloatColour colour = colours[colour_index].Interpolate(colours[colour_index + 1], blend);
            long r = GFFloatToLongNear(colour.R * 255.0f);
            long g = GFFloatToLongNear(colour.G * 255.0f);
            long b = GFFloatToLongNear(colour.B * 255.0f);
            long a = GFFloatToLongNear(colour.A * 255.0f);
            CRGBColour rgb((unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a);

            CTVertexColTex1* vertex0 = vertices++;
            CTVertexColTex1* vertex1 = vertices++;
            vertex0->SetPosition(left);
            vertex0->SetDiffuseColour(rgb.IntValue);
            vertex0->SetU1V1(0.99f, v);
            vertex1->SetPosition(right);
            vertex1->SetDiffuseColour(rgb.IntValue);
            vertex1->SetU1V1(0.01f, v);
        }
        --current;
        --previous;
        first_step = 1;
    }

    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_CLAMP);
    buffer->Unlock();
    NEngineInternal::GPShaderRenderManager->DrawBuffersNonIndexed(*buffer, 0, vertex_count - 2, PT_TRIANGLE_STRIP);
    NEngineInternal::GPRenderManager->FlushRender();
    VertexBufferPool.ReturnResource(buffer);
}
