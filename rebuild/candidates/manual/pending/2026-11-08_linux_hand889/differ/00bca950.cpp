// CEnginePrimitiveManagerSpriteTrails::RenderSpriteTrail(const CEngineInternalPrimitiveSpriteTrail& trail, bool premultiplied) -- draw a sprite trail as one camera-facing triangle strip
// Grade: DIFFER(3751v3749) -- 1030 normalized / 205 register+frame-blind diff lines after 55 grades: register allocation (retail trail=esi, points then the tangent-loop n=ebx, trail_size in memory [ebp+0x40] with `idiv [mem]`; ours trail=ecx, trail_size=ebx, points/n in memory -- c2prio: points prio 89 allowed ebx only, split esi #196; trail_size prio -5 takes ebx piece #171), so the late shrink-wrapped `push edi`, StartCaptureBlock's `shl dword ptr [eax+0x2814],1`, frame 0x1b4 vs 0x1b0 and every slot follow; the segment loop's per-component strength-reduced tangent/position pointers (retail `fsub [edx]`,`[eax]`,`[ecx]` vs ours `[edx]`,`[edx+4]`,`[edx+8]`); the TransformPoint X-row inner pair order; the premultiply `fadd st0,st0` scheduling.
// Next: c2prio --trace the tangent loop to see why the strength-reduced n (temp 279, prio 146, .c.sib.) loses ebx to trail_size; try a separate pointer-walk spelling of the segment loop's tangent/Trail accesses (retail keeps size*12 at [ebp-0x30] to wrap the pointers) once the allocation matches.
// Leaf: ?RenderSpriteTrail@CEnginePrimitiveManagerSpriteTrails@@QAEXABVCEngineInternalPrimitiveSpriteTrail@@_N@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CVERTEXBUFFERBASE_ISBUSY_SIZEOF
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM
#define FABLE_RETAIL_CENGINECAMERA_INVERSENOSCALEVIEWTRANSFORM
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CArray.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CVertexShader.h"
#include "engine/CTVertexColTex1.h"
#include "engine/EVertexType.h"
#include "engine/EPrimitiveType.h"
#include "engine/CEngineCamera.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFloatToLongNear.h"

template <class T> FABLE_FORCEINLINE T Min(T a, T b) { return b < a ? b : a; }
template <class T> FABLE_FORCEINLINE T Max(T a, T b) { return b < a ? a : b; }
extern const float DELTA_NOTIONAL_ZERO;   // 0x0129BA3C (1e-4f)

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer;                             // 0x01436E78
extern CEngineCamera* g_pEngineCamera;                                             // 0x01436EA0

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

// The engine's dynamic-resource tree filer (Ego_r.pdb CEngineTreeFilerBase, 8 bytes; CNodeHeader 0x1C bytes, the node's key
// words follow it). No header: local model of the members the pool's inline lookups use.
class CEngineTreeFilerBase {
public:
    struct CNodeHeader {
        unsigned char IsLeaf;                      // +0x00
        unsigned char KeyOffset;                   // +0x01
        unsigned char KeyBitStart;                 // +0x02
        unsigned char KeyBitCount;                 // +0x03
        void*         Entry;                       // +0x04
        CNodeHeader*  PrevSibling;                 // +0x08
        CNodeHeader*  NextSibling;                 // +0x0c
        CNodeHeader*  ChildListHead;               // +0x10
        CNodeHeader*  ChildListTail;               // +0x14
        CNodeHeader*  Parent;                      // +0x18
    };
    CNodeHeader* BaseNode;                         // +0x00
    long         KeySize;                          // +0x04
    FABLE_FORCEINLINE unsigned long FindID(unsigned long* key)
    {
        CNodeHeader* node = FindNode(BaseNode, key);
        if (!node)
            return 0;
        unsigned long* node_key = (unsigned long*)(node + 1);
        for (long i = 0; i < KeySize; ++i)
            if (key[i] != node_key[i])
                return 0;
        return (unsigned long)node;
    }
    FABLE_FORCEINLINE unsigned long AddKey(unsigned long* key)
    {
        unsigned long id = FindID(key);
        if (id)
            return id;
        return AddNewKey(key);
    }
protected:
    void* GetGenericObject(unsigned long id)
    {
        void* object = 0;
        if (id)
            object = ((CNodeHeader*)id)->Entry;
        return object;
    }
    void SetGenericObject(unsigned long id, void* object)
    {
        if (id)
            ((CNodeHeader*)id)->Entry = object;
    }
private:
    CNodeHeader* FindNode(CNodeHeader* node, unsigned long* key);   // 0x00BC9B30
    unsigned long AddNewKey(unsigned long* key);                    // 0x00BF3030
};
FABLE_FORCEINLINE CEngineTreeFilerBase::CNodeHeader* CEngineTreeFilerBase::FindNode(CNodeHeader* node, unsigned long* key)
{
    if (node->IsLeaf)
        return node;
    CNodeHeader* child = ((CNodeHeader**)(node + 1))[(key[node->KeyOffset] >> node->KeyBitStart) & ((1 << node->KeyBitCount) - 1)];
    if (child)
        return FindNode(child, key);
    return 0;
}
template <class T> class CEngineTreeFiler : public CEngineTreeFilerBase {
public:
    T* GetEntry(unsigned long id) { return (T*)GetGenericObject(id); }
    void SetEntry(unsigned long id, T* entry) { SetGenericObject(id, entry); }
};

// CEngineDynamic3DResource<CVertexBufferWin32, 3> (Ego_r.pdb): the buffer, its pool key and the free-list link.
template <class T, int N> class CEngineDynamic3DResource : public T {
public:
    unsigned long             Key[N];              // +0x34
    CEngineDynamic3DResource* PoolNext;            // +0x40
};

template <class TResource> class CEngineDynamic3DResourceManager {
public:
    struct CListHeader {
        TResource* First;                          // +0x00
        TResource* Last;                           // +0x04
        CListHeader() : First(0), Last(0) { }
    };
    virtual long GetTotalBytesAllocated() const;   // vtable 0x012A69DC
    long AllocCount;                               // +0x04
    long FreePoolMem;                              // +0x08
    long TotalMemAllocated;                        // +0x0c
    CEngineTreeFiler<CListHeader> ResourceMappingTree;   // +0x10
    // ?ReturnResource@?$CEngineDynamic3DResourceManager@...@@QAEXPAV?$CEngineDynamic3DResource@VCVertexBufferWin32@@$02@@@Z
    // (out-of-line copy 0x00BCA140, expanded here).
    FABLE_FORCEINLINE void ReturnResource(TResource* resource)
    {
        unsigned long id = ResourceMappingTree.FindID(resource->Key);
        if (id > 0) {
            CListHeader* header = ResourceMappingTree.GetEntry(id);
            if (!header) {
                header = new CListHeader;
                ResourceMappingTree.SetEntry(id, header);
            }
            if (header->Last)
                header->Last->PoolNext = resource;
            else
                header->First = resource;
            header->Last = resource;
            FreePoolMem += resource->Sizeof();
        }
    }
protected:
    // ?GetResource@...@@IAEPAV?$CEngineDynamic3DResource@VCVertexBufferWin32@@$02@@PAK@Z (out-of-line copy 0x00BCA210).
    FABLE_FORCEINLINE TResource* GetResource(unsigned long* key)
    {
        unsigned long id = ResourceMappingTree.AddKey(key);
        CListHeader* header = ResourceMappingTree.GetEntry(id);
        if (!header) {
            header = new CListHeader;
            ResourceMappingTree.SetEntry(id, header);
        }
        if (header->First && !header->First->IsBusy()) {
            TResource* resource = header->First;
            header->First = resource->PoolNext;
            resource->PoolNext = 0;
            if (!header->First)
                header->Last = 0;
            FreePoolMem -= resource->Sizeof();
            return resource;
        }
        return 0;
    }
    // ?AddNewResource@...@@IAEXPAV?$CEngineDynamic3DResource@VCVertexBufferWin32@@$02@@PAK@Z
    FABLE_FORCEINLINE void AddNewResource(TResource* resource, unsigned long* key)
    {
        for (long i = 0; i < 3; ++i)
            resource->Key[i] = key[i];
        TotalMemAllocated += resource->Sizeof();
    }
};

class CEngineVertexBufferPool : public CEngineDynamic3DResourceManager<CEngineDynamic3DResource<CVertexBufferWin32, 3> > {
public:
    typedef CEngineDynamic3DResource<CVertexBufferWin32, 3> CBuffer;
    long SizeGranularity;                          // +0x18
    FABLE_FORCEINLINE void GetKey(unsigned long* key, EVertexType type, long count, long stride, unsigned long usage, EVertexBufferPool pool)
    {
        key[0] = type;
        key[1] = (stride << 16) + count;
        key[2] = (pool << 16) + usage;
    }
    // ?GetBuffer@CEngineVertexBufferPool@@QAEPAV?$CEngineDynamic3DResource@VCVertexBufferWin32@@$02@@W4EVertexType@@JJKW4EVertexBufferPool@@@Z
    // (out-of-line copy 0x00BCA510, expanded here).
    FABLE_FORCEINLINE CBuffer* GetBuffer(EVertexType type, long count, long stride, unsigned long usage, EVertexBufferPool pool)
    {
        count = ((count - 1) / SizeGranularity + 1) * SizeGranularity;
        if (count >= 0x10000)
            count = 0x10000;
        unsigned long key[3];
        GetKey(key, type, count, stride, usage, pool);
        CBuffer* resource = GetResource(key);
        if (resource)
            return resource;
        resource = BuildNewBuffer(key, type, count, stride, usage, pool);
        if (resource)
            AddNewResource(resource, key);
        return resource;
    }
    CBuffer* BuildNewBuffer(unsigned long* key, EVertexType type, long count, long stride, unsigned long usage,
                            EVertexBufferPool pool);   // 0x00BF3380
};

// CEngineInternalPrimitiveSpriteTrail (Ego_r.pdb; retail 4 below the PDB past the 12-byte Trail vector, as the landed Update
// 0x00BCBC00 models it).
class CEngineInternalPrimitiveSpriteTrail : public CEngineInternalPrimitiveBase {
public:
    class CSpriteTrailEntry {
    public:
        C3DVector Position;
        float     Width;
    };
    unsigned long             MaxLength;           // +0x48
    unsigned long             GraphicIndex;        // +0x4c
    unsigned long             GraphicFrame;        // +0x50
    float                     SkipPoints;          // +0x54
    float                     Width;               // +0x58
    CRGBColour                HeadColour;          // +0x5c
    CRGBColour                MidColour;           // +0x60
    CRGBColour                TailColour;          // +0x64
    CArray<CSpriteTrailEntry> Trail;               // +0x68
    long                      NextPos;             // +0x74
    unsigned short            Flags;               // +0x78
    long                      BlendOp;             // +0x7c
    long                      BlendMode;           // +0x80
    CMatrix3x4                WorldMat;            // +0x84
};

class CEnginePrimitiveManagerSpriteTrails : public CEnginePrimitiveManager {
public:
    CVertexShader           VertexShaderZSortedUnlit;   // +0x0c
    CEngineVertexBufferPool VertexBufferPool;           // +0x14
    void RenderSpriteTrail(const CEngineInternalPrimitiveSpriteTrail& trail, bool premultiplied);
};

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

    CMatrix3x4 transform = trail.WorldMat;
    long vertex_count = segments * 16 + 2;
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
    CEngineVertexBufferPool::CBuffer* buffer = VertexBufferPool.GetBuffer(VERTEX_NORM_COL_SPEC_TEX1, vertex_count, sizeof(CTVertexColTex1),
                                                                         g_pPrimitiveRenderer->DynamicVBUsage, g_pPrimitiveRenderer->DynamicVBPool);
    CTVertexColTex1* vertices = (CTVertexColTex1*)buffer->Lock(g_pPrimitiveRenderer->DynamicVBLockFlags, 0, 0);

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
    long current = start - 1;
    long previous = current - 1;
    long first_step = 0;
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
            g_pEngineCamera->NoScaleViewTransform.TransformVector(direction);
            direction = C3DVector(direction.Y, -direction.X, 0.0f);
            g_pEngineCamera->InverseNoScaleViewTransform.TransformVector(direction);
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

            vertices->X = left.X;
            vertices->Y = left.Y;
            vertices->Z = left.Z;
            vertices->DiffuseColour = rgb.IntValue;
            vertices->U1 = 0.99f;
            vertices->V1 = v;
            ++vertices;
            vertices->X = right.X;
            vertices->Y = right.Y;
            vertices->Z = right.Z;
            vertices->DiffuseColour = rgb.IntValue;
            vertices->U1 = 0.01f;
            vertices->V1 = v;
            ++vertices;
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
