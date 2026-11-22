// C3DPrimitive2::LoadBinary
// Leaf: ?LoadBinary@C3DPrimitive2@@QAE_NAAVCMemoryDataInputStream@@KPAPAEPAVCMovableResourceMemoryPool@@@Z
// Flags:
#define FABLE_RETAIL_C3DMESH2_LOADBINARY
#define FABLE_RETAIL_C3DPRIMITIVE2_LOADBINARY_PASS27
#define FABLE_RETAIL_C3DPRIMITIVE2_LOADER
#define FABLE_RETAIL_C3DPRIMITIVE2_HIERARCHY
#define FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
#define FABLE_RETAIL_CANIMATEDBLOCK_DATA
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_UPLOADTOINDEXBUFFER
#define FABLE_RETAIL_C3DCLOTHPRIMITIVE_VIRTUAL_DTOR
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#define FABLE_RETAIL_STLP_UNINIT_FILL_N_WRAPPER_EXPANSION_PASS27
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_STLP_COPY_BACKWARD_NOINLINE
#define FABLE_RETAIL_STLP_FILL_NOINLINE
#define FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_NOINLINE
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#include <new>
#include "engine/EPrimitiveType.h"
#include "engine/EIndexBufferFormat.h"
#include "engine/EIndexBufferPool.h"
#include "engine/EVertexType.h"
#include "engine/C3DClothPrimitive.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CVertexCompressionParams.h"
static const EIndexBufferFormat IndexFormat16 = IB_FORMAT_16;
static const EIndexBufferPool IndexPoolManaged = IB_POOL_MANAGED;
#include "engine/C3DPrimitive2.h"

template<class T> void FABLE_FASTCALL GFExplicitNew(T* blocks, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i)
        new (&blocks[i]) T;
}

bool C3DPrimitive2::LoadBinary(CMemoryDataInputStream& is, unsigned long init_flags, unsigned char** buffer_dest, CMovableResourceMemoryPool* pool)
{
    MaterialIndex = is.ReadSLONG();
    RepeatingMeshReps = is.ReadSLONG();
    BoundingSphere.Center = is.Read3DVector();
    BoundingSphere.Radius = is.ReadFloat();
    AverageTextureStretch = is.ReadFloat();
    StaticBlockCount = is.ReadULONG();
    AnimatedBlockCount = is.ReadULONG();
    VertexCount = is.ReadULONG();
    TriangleCount = is.ReadULONG();
    IndexCount = is.ReadULONG();
    unsigned long src_init_flags = is.ReadULONG();
    InitFlags = src_init_flags & ~4;
    SetupVertexStreamFlags(InitFlags, IsAnimated());

    StaticBlockCount = is.ReadULONG();
    AnimatedBlockCount = is.ReadULONG();
    StaticBlocks = (N3DPrimitive::CStaticBlock*)*buffer_dest;
    GFExplicitNew(StaticBlocks, StaticBlockCount);
    *buffer_dest += StaticBlockCount * sizeof(N3DPrimitive::CStaticBlock);
    for (unsigned long c_block = 0; c_block < StaticBlockCount; ++c_block)
        StaticBlocks[c_block].LoadBinary(is);
    AnimatedBlocks = (N3DPrimitive::CAnimatedBlock*)*buffer_dest;
    GFExplicitNew(AnimatedBlocks, AnimatedBlockCount);
    *buffer_dest += AnimatedBlockCount * sizeof(N3DPrimitive::CAnimatedBlock);
    for (unsigned long c_block = 0; c_block < AnimatedBlockCount; ++c_block)
        AnimatedBlocks[c_block].LoadBinary(is);

    bool animated = AnimatedBlockCount > 0;
    unsigned long reps = RepeatingMeshReps > 1 ? RepeatingMeshReps : 1;
    CVertexCompressionParams vertex_comp;
    is.Read(&vertex_comp, sizeof(vertex_comp));
    bool repeated = false;
    if (RepeatingMeshReps > 0)
        repeated = true;
    unsigned long src_vb_stride = is.ReadULONG();
    EVertexType vb_type = (EVertexType)is.ReadULONG();
    unsigned long dest_vb_stride = CalculateVertexSize(InitFlags, animated, repeated);
    if (!SetupVertexBuffer(vb_type, dest_vb_stride, VertexCount * reps, InitFlags))
        return false;
    LoadVertexBuffer(is, dest_vb_stride, src_vb_stride, src_init_flags, vertex_comp);

    unsigned long ib_size = IndexCount * reps * sizeof(unsigned short);
    IndexBuffer.reset(new CIndexBuffer);
    if (!InitIndexBuffer(*IndexBuffer, ib_size, init_flags))
        return false;
    unsigned char* ib_data = IndexBuffer->Lock(0, 0, 0);
    is.ReadCompressed(ib_data, ib_size);
    IndexBuffer->Unlock();

    unsigned long cloth_count = is.ReadULONG();
    ClothPrimitives.resize(cloth_count);
    for (unsigned long c_cloth = 0; c_cloth < cloth_count; ++c_cloth) {
        CCountedPointer<C3DClothPrimitive>& cloth_prim = ClothPrimitives[c_cloth];
        cloth_prim.Reset(new C3DClothPrimitive);
        if (!cloth_prim->LoadBinary(is, pool)) {
            ClothPrimitives.clear();
            break;
        }
    }
    VertexBufferStatus = 1;
    IndexBufferStatus = 1;
    AnimatedBlocksStatus = 1;
    RepeatedBlocksStatus = 1;
    UploadedFlag = true;
    return true;
}