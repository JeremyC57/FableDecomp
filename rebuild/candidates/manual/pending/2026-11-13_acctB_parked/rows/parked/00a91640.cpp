// C3DPrimitive2::InitialiseFromMeshFileData
// Grade: DIFFER(4324v4344) -- /d2inlT108; call list + inline decisions + FPU + block layout match; residue = frame packer: 0x9c v 0xa0, verts/FindNext temp slot (retail lowest B+0x10), xapr slot, C3DVector() resize temp own slot B+0xA4, regs esi/edi swap for xapr
// Flags: /d2inlT108
// Leaf: ?InitialiseFromMeshFileData@C3DPrimitive2@@QAE_NABVC3DMeshFilePrimitiveChunk@@_NKKAAVCScopedMemoryHandle@@@Z
#define FABLE_RETAIL_C3DPRIMITIVE2_LOADER
#define FABLE_RETAIL_C3DPRIMITIVE2_HIERARCHY
#define FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILEPRIMITIVECHUNK
#define FABLE_RETAIL_C3DMESHFILEXVERTEXDATACHUNK
#define FABLE_RETAIL_C3DMESHFILEVERTICESCHUNK
#define FABLE_RETAIL_C3DMESHFILETRIANGLESCHUNK
#define FABLE_RETAIL_C3DMESHFILEXANIMATEDPRIMITIVECHUNK
#define FABLE_RETAIL_C3DMESHFILEGROUPCHUNK
#define FABLE_RETAIL_C3DMESHFILEXUVTANGENTCHUNK
#define FABLE_RETAIL_C3DMESHFILEXBOUNDINGSPHERECHUNK
#define FABLE_RETAIL_C3DMESHFILEXMAPPINGINFOCHUNK
#define FABLE_RETAIL_C3DMESHFILECLOTHCHUNK
#define FABLE_RETAIL_CMIRROREDTEXTUREHIGHLIGHTSCHUNK
#define FABLE_RETAIL_C3DGROUP2
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_NV
#define FABLE_NV_ISNONZERO_BODY float squared=Z*Z; squared+=X*X+Y*Y; return squared>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;
#define FABLE_NV_NORMALISE_BODY float factor=GFFastOneOverSqrt(Z*Z+X*X+Y*Y); X*=factor; Y*=factor; Z*=factor; return *this;
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#include "engine/C3DPrimitive2.h"
#include "engine/CMovableResourceMemoryPool.h"
#include "engine/C3DMeshFilePrimitiveChunk.h"
#include "engine/C3DMeshFileXVertexDataChunk.h"
#include "engine/C3DMeshFileVerticesChunk.h"
#include "engine/C3DMeshFileTrianglesChunk.h"
#include "engine/C3DMeshFileXAnimatedPrimitiveChunk.h"
#include "engine/C3DMeshFileXAnimatedPrimitiveBlockChunk.h"
#include "engine/C3DMeshFileGroupChunk.h"
#include "engine/C3DMeshFileXUVTangentChunk.h"
#include "engine/C3DMeshFileXBoundingSphereChunk.h"
#include "engine/C3DMeshFileXMappingInfoChunk.h"
#include "engine/C3DMeshFileClothChunk.h"
#include "engine/CMirroredTextureHighlightsChunk.h"
#include "engine/C3DGroup2.h"
#include <string.h>

class C3DClothPrimitive {                                   // retail 0x14 (new(0x14) at 0x00A92260)
public:
    C3DClothPrimitive(const C3DMeshFileClothChunk* chunk);  // 0x00AC3E00
    ~C3DClothPrimitive();
private:
    unsigned char Body[0x14];
};

class CScopedMemoryHandle {                                 // Pool +0, Handle +4 (landed 0x00A039F0 models it locally)
public:
    void Initialise(CHandleBasedMemoryPool* pool, long handle);   // 0x00A039F0
    void* GetPointer() const { return Pool->GetPointerFromHandle(Handle); }   // FableWin ?GetPointer@CScopedMemoryHandle@@QBEPAXXZ
    CHandleBasedMemoryPool* Pool;
    long Handle;
};

namespace std {
template<> CIVCountedPointer<N3DPrimitive::CTriangleList>* __fastcall
__copy(CIVCountedPointer<N3DPrimitive::CTriangleList>* first, CIVCountedPointer<N3DPrimitive::CTriangleList>* last,
       CIVCountedPointer<N3DPrimitive::CTriangleList>* result, const random_access_iterator_tag&, int*);   // 0x00A964C0
}

template<class T> void FABLE_FASTCALL GFExplicitNew(T* objects, unsigned long count);   // 0x00A94150 / 0x00A96270

template<> void std::vector<C3DVector>::resize(std::vector<C3DVector>::size_type n, const C3DVector& x);   // 0x0075C040 (out of line)

extern CMovableResourceMemoryPool* GMeshDataMemoryPool;      // 0x013D2D6C (descriptive)
extern bool GLoadMirroredTextureHighlights;                  // 0x013D2D74 (descriptive)

static inline bool IsValidSize(__int64 size) { return size >= 0 && size <= 0x7fffffff; }

bool C3DPrimitive2::InitialiseFromMeshFileData(const C3DMeshFilePrimitiveChunk& chunk, bool b, unsigned long reps,
                                               unsigned long initFlags, CScopedMemoryHandle& memoryHandle)
{
    FreeData();
    Data.reset(new N3DPrimitive::CDeviceIndependent);
    InitFlags = initFlags;
    RepeatingMeshReps = reps;
    MaterialIndex = chunk.GetMaterialIndex(0);
    bool hasVertexBuffer = false;
    CCountedPointer<const C3DMeshFileXVertexDataChunk> vbd;
    vbd.SetStaticCast(chunk.FindFirstSubChunkOfType(C3DMeshFileXVertexDataChunk::GetStaticChunkType()));
    if (vbd.Get()) {
        VertexCount = vbd->GetVertexCount();
        unsigned long vertexSize = vbd->GetVertexSize();
        InitFlags = vbd->GetInitFlags();
        unsigned long bufferVertexSize = CalculateVertexSize(InitFlags, b, false);
        if (!SetupVertexBuffer((EVertexType)0, bufferVertexSize, VertexCount, InitFlags))
            return false;
        VertexBuffer->CompressionParams = vbd->GetVBCompressionParams();
        const void* src = vbd->PeekVertexData();
        void* dst = VertexBuffer->Lock(0, 0, 0);
        memcpy(dst, src, VertexCount * vertexSize);
        VertexBuffer->Unlock();
        SetVertexBufferStatus(1);
        hasVertexBuffer = true;
    } else {
        CCountedPointer<const C3DMeshFileVerticesChunk> verts = chunk.PeekVertices();
        const C3DVertex2* list = verts->GetList();
        unsigned long count = verts->GetCount();
        Data->Vertices.resize(count);
        for (unsigned long i = 0; i < count; ++i) {
            C3DVertex2 v = list[i];
            if (v.Normal.IsNonZero())
                v.Normal.Normalise();
            Data->SetVertex(i, v);
        }
        VertexCount = count;
    }
    CCountedPointer<const C3DMeshFileTrianglesChunk> tris = chunk.FindFirstTriangles();
    TriangleCount = 0;
    while (tris.Get()) {
        Data->AddTriangleList(*tris);
        tris = chunk.FindNextTriangles(tris);
    }
    AnimatedBlockCount = 0;
    CCountedPointer<const C3DMeshFileXAnimatedPrimitiveChunk> xapr;
    xapr.SetStaticCast(chunk.FindFirstSubChunkOfType(C3DMeshFileXAnimatedPrimitiveChunk::GetStaticChunkType()));
    if (xapr.Get())
        AnimatedBlockCount = xapr->CountSubChunksOfType(C3DMeshFileXAnimatedPrimitiveBlockChunk::GetStaticChunkType());
    StaticBlockCount = Data->GetTriangleListCount();
    unsigned long blockBytes = AnimatedBlockCount * sizeof(N3DPrimitive::CAnimatedBlock)
                             + StaticBlockCount * sizeof(N3DPrimitive::CStaticBlock);
    long handle;
    if (IsValidSize((__int64)blockBytes)) {
        if (!GMeshDataMemoryPool->AllocateResource(&handle, this, blockBytes, 0))
            return false;
        memoryHandle.Initialise(GMeshDataMemoryPool, handle);
        StaticBlocks = (N3DPrimitive::CStaticBlock*)memoryHandle.GetPointer();
        GFExplicitNew(StaticBlocks, StaticBlockCount);
        if (AnimatedBlockCount) {
            AnimatedBlocks = (N3DPrimitive::CAnimatedBlock*)(StaticBlocks + StaticBlockCount);
            GFExplicitNew(AnimatedBlocks, AnimatedBlockCount);
        }
        for (unsigned long i = 0; i < StaticBlockCount; ++i)
            GetStaticBlock(i)->MaterialIndex = chunk.GetMaterialIndex(i);
        CCountedPointer<const C3DMeshFileGroupChunk> group = chunk.FindFirstGroup();
        while (group.Get()) {
            CCountedPointer<C3DGroup2> newGroup(new C3DGroup2(*group));
            Data->Groups.push_back(newGroup);
            group = chunk.FindNextGroup(group);
        }
        CCountedPointer<const C3DMeshFileXUVTangentChunk> xuvt;
        xuvt.SetStaticCast(chunk.FindFirstSubChunkOfType(C3DMeshFileXUVTangentChunk::GetStaticChunkType()));
        if (xuvt.Get()) {
            unsigned long count = xuvt->GetCount();
            Data->UTangents.resize(count);
            Data->VTangents.resize(count);
            for (unsigned long i = 0; i < count; ++i) {
                C3DVector u = xuvt->GetUTangent(i);
                C3DVector v = xuvt->GetVTangent(i);
                Data->UTangents[i] = u;
                Data->VTangents[i] = v;
            }
        }
        if (xapr.Get()) {
            Data->ClearTriangleLists();
            CCountedPointer<const C3DMeshFileXAnimatedPrimitiveBlockChunk> xapb;
            xapb.SetStaticCast(xapr->FindFirstSubChunkOfType(C3DMeshFileXAnimatedPrimitiveBlockChunk::GetStaticChunkType()));
            for (unsigned long i = 0; xapb.Get(); ++i) {
                GetAnimatedBlock(i)->InitialiseFromMeshFileData(*xapb, hasVertexBuffer);
                CCountedPointer<const C3DMeshFileTrianglesChunk> blockTris;
                blockTris.SetStaticCast(xapb->FindFirstSubChunkOfType(C3DMeshFileTrianglesChunk::GetStaticChunkType()));
                Data->AddTriangleList(*blockTris);
                xapb.SetStaticCast(xapr->FindNextSubChunkOfType(xapb));
            }
        }
        CCountedPointer<const C3DMeshFileXBoundingSphereChunk> xbds;
        xbds.SetStaticCast(chunk.FindFirstSubChunkOfType(C3DMeshFileXBoundingSphereChunk::GetStaticChunkType()));
        if (xbds.Get()) {
            BoundingSphere.Center = xbds->GetCenter();
            BoundingSphere.Radius = xbds->GetRadius();
        }
        AverageTextureStretch = 0.0f;
        CCountedPointer<const C3DMeshFileXMappingInfoChunk> xmif;
        xmif.SetStaticCast(chunk.FindFirstSubChunkOfType(C3DMeshFileXMappingInfoChunk::GetStaticChunkType()));
        if (xmif.Get())
            AverageTextureStretch = xmif->GetAverageTextureStretch();
        TriangleCount = Data->GetTotalTriangleCount();
        IndexCount = Data->GetTotalIndexCount();
        ClothPrimitives.reserve(chunk.CountSubChunksOfType(C3DMeshFileClothChunk::GetStaticChunkType()));
        CCountedPointer<const C3DMeshFileClothChunk> cloth = chunk.FindFirstCloth();
        while (cloth.Get()) {
            CCountedPointer<C3DClothPrimitive> newCloth(new C3DClothPrimitive(cloth.Get()));
            ClothPrimitives.push_back(newCloth);
            cloth = chunk.FindNextCloth(cloth);
        }
        if (GLoadMirroredTextureHighlights) {
            CCountedPointer<CChunkedFileChunk> mteh = chunk.FindFirstSubChunkOfType(CMirroredTextureHighlightsChunk::GetStaticChunkType());
            while (mteh.Get()) {
                CMirroredTextureHighlightsChunk* highlights = static_cast<CMirroredTextureHighlightsChunk*>(mteh.Get());
                AddMirroredTextureVertexIndex(highlights->GetList(), highlights->GetCount());
                mteh = chunk.FindNextSubChunkOfType(mteh);
            }
        }
        SetupVertexStreamFlags(InitFlags, b);
        return true;
    }
    return false;
}
