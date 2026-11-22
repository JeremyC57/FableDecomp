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

#include <stdio.h>
#include <stdlib.h>

#include "fable_string.h"
#include "engine/CMovableResourceMemoryPool.h"

static unsigned char FileBytes[512];
static unsigned long FileLength;
static unsigned long NumberStaticRead, NumberAnimatedRead, NumberClothRead, NumberClothFreed;
static unsigned long NumberIndexBuffersFreed, NumberIndexLocks, NumberIndexUnlocks, NumberCompressedReads;
static unsigned long VertexCountSeen, VertexStrideSeen, VertexTypeSeen, VertexFlagsSeen;
static unsigned long IndexBytesSeen, IndexUsageSeen, IndexPoolSeen, IndexFormatSeen, PrimitiveTypeSeen;
static unsigned long OriginalFlagsSeen, InputVertexStrideSeen, ExpandedVertexStrideSeen;
static unsigned long LayoutFlagsSeen;
static bool LayoutAnimatedSeen, LayoutRepeatedSeen, VertexLoadSeen, AllowBuffer;
static long FirstClothFailure;
static const CMovableResourceMemoryPool* ClothPoolSeen;
static CVertexCompressionParams CompressionSeen;
static unsigned char IndexPayload[64];
static bool CallsInOrder;

static void Write(const void* data, unsigned long size)
{
    memcpy(FileBytes + FileLength, data, size);
    FileLength += size;
}

static void WriteWord(unsigned long value) { Write(&value, sizeof(value)); }
static void WriteFloat(float value) { Write(&value, sizeof(value)); }

CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::CBaseClassNonCopyable() { }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
CDataStream::~CDataStream() { }

CDataInputStream::CDataInputStream()
    : StreamPos(0), StreamSize(FileLength), CurrentSourcePtr(FileBytes),
      SourceChunkStreamPos(0), SourceChunkBytesRemaining(FileLength) { }
void CDataInputStream::Close() { }
void CDataInputStream::SetPosition(unsigned long pos)
{
    StreamPos = pos;
    CurrentSourcePtr = FileBytes + pos;
    SourceChunkBytesRemaining = FileLength - pos;
}
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char*, long) { abort(); }
void CDataInputStream::InternalReadNextSourceChunk(const unsigned char*&, long&) { abort(); }
bool CDataInputStream::NeedsBufferedRead(long) { return false; }
void CDataInputStream::InternalReadUnbuffered(unsigned char*, long) { abort(); }
void CDataInputStream::ReadCompressed(void*, long) { abort(); }
void CDataInputStream::ReadZlibCompressed(void*, long) { abort(); }
CCharString CDataInputStream::ReadNullTerminatedString() { abort(); }
CWideString CDataInputStream::ReadNullTerminatedWString() { abort(); }
CCharString CMemoryDataInputStream::ReadNullTerminatedString() { abort(); }
CWideString CMemoryDataInputStream::ReadNullTerminatedWString() { abort(); }
void CMemoryDataInputStream::ReadCompressed(void* output, long size)
{
    CallsInOrder &= NumberIndexLocks == NumberCompressedReads + 1;
    ++NumberCompressedReads;
    Read(output, size);
}
C3DVector CDataInputStream::Read3DVector()
{
    C3DVector out;
    Read(&out, sizeof(out));
    return out;
}

C3DPrimitive2::~C3DPrimitive2() { }
void C3DPrimitive2::RelocateData(long, void*, void*, long) { }
bool CMovableResource::CanRelocateData(long) const { return true; }
unsigned long CMovableResource::Verify() const { return 0x7b; }

void N3DPrimitive::CStaticBlock::LoadBinary(CMemoryDataInputStream& is)
{
    CallsInOrder &= PrimitiveCount == 0 && StartIndex == 0 && !IsStrip &&
                    !DegenerateTriangles && !ChangeFlags && MaterialIndex == -1 &&
                    DiffuseTextureFixupOffset == 0 && BumpTextureFixupOffset == 0 &&
                    DiffuseTexture == 0 && BumpTexture == 0;
    ++NumberStaticRead;
    MaterialIndex = is.ReadSLONG();
}
void N3DPrimitive::CAnimatedBlock::LoadBinary(CMemoryDataInputStream& is)
{
    CallsInOrder &= PrimitiveCount == 0 && StartIndex == 0 && !IsStrip &&
                    !DegenerateTriangles && !ChangeFlags;
    ++NumberAnimatedRead;
    VertexCount = is.ReadULONG();
}

unsigned long FABLE_FASTCALL C3DPrimitive2::CalculateVertexSize(unsigned long flags, bool animated, bool repeated)
{
    LayoutFlagsSeen = flags;
    LayoutAnimatedSeen = animated;
    LayoutRepeatedSeen = repeated;
    return animated ? 36 : 20;
}
bool C3DPrimitive2::SetupVertexBuffer(EVertexType type, unsigned long stride, unsigned long count, unsigned long flags)
{
    VertexCountSeen = count;
    VertexStrideSeen = stride;
    VertexTypeSeen = type;
    VertexFlagsSeen = flags;
    return AllowBuffer;
}
void C3DPrimitive2::LoadVertexBuffer(CMemoryDataInputStream& is, unsigned long dest_stride, unsigned long input_stride,
                                    unsigned long src_flags, CVertexCompressionParams& params)
{
    CallsInOrder &= VertexCountSeen == 6 || VertexCountSeen == 3;
    VertexLoadSeen = is.ReadULONG() == 0x3f41;
    OriginalFlagsSeen = src_flags;
    InputVertexStrideSeen = input_stride;
    ExpandedVertexStrideSeen = dest_stride;
    CompressionSeen = params;
}

CIndexBuffer::CIndexBuffer() : PD3DIndexBuffer(0), ByteLength(0), PAllocatedBuffer(0) { }
void CIndexBuffer::RelocateData(long, void*, void*, long) { }
void CIndexBuffer::Initialise(unsigned long size, unsigned long usage, EIndexBufferFormat format,
                              EIndexBufferPool pool, EPrimitiveType type)
{
    CallsInOrder &= VertexLoadSeen;
    ByteLength = size;
    IndexBytesSeen = size;
    IndexUsageSeen = usage;
    IndexPoolSeen = pool;
    IndexFormatSeen = format;
    PrimitiveTypeSeen = type;
}
void CIndexBuffer::Uninitialise() { ++NumberIndexBuffersFreed; }
unsigned char* CIndexBuffer::Lock(unsigned long offset, unsigned long size, unsigned long flags)
{
    CallsInOrder &= !offset && !size && !flags && ByteLength && ByteLength <= sizeof(IndexPayload);
    ++NumberIndexLocks;
    return IndexPayload;
}
void CIndexBuffer::Unlock()
{
    CallsInOrder &= NumberCompressedReads == NumberIndexUnlocks + 1;
    ++NumberIndexUnlocks;
}

C3DClothPrimitive::~C3DClothPrimitive() { ++NumberClothFreed; }
bool C3DClothPrimitive::LoadBinary(CMemoryDataInputStream& is, CMovableResourceMemoryPool* pool)
{
    CallsInOrder &= NumberIndexUnlocks == 1;
    ClothPoolSeen = pool;
    PrimitiveIndex = is.ReadULONG();
    return (long)NumberClothRead++ != FirstClothFailure;
}

namespace std {
template<> void FABLE_FASTCALL __destroy_aux(CCountedPointer<C3DClothPrimitive>* first,
                                              CCountedPointer<C3DClothPrimitive>* last,
                                              const __false_type&)
{
    for (; first != last; ++first)
        first->~CCountedPointer();
}
}

static bool Run(long reps, unsigned long outer_flags, unsigned long leading_animated,
                unsigned long static_blocks, unsigned long animated_blocks, unsigned long cloth,
                long fail_cloth, bool allow_buffer, unsigned long old_cloth, bool old_index)
{
    FileLength = 0;
    NumberStaticRead = NumberAnimatedRead = NumberClothRead = NumberClothFreed = 0;
    NumberIndexBuffersFreed = NumberIndexLocks = NumberIndexUnlocks = NumberCompressedReads = 0;
    VertexCountSeen = VertexStrideSeen = VertexTypeSeen = VertexFlagsSeen = 0;
    IndexBytesSeen = IndexUsageSeen = IndexPoolSeen = IndexFormatSeen = PrimitiveTypeSeen = 0;
    OriginalFlagsSeen = InputVertexStrideSeen = ExpandedVertexStrideSeen = 0;
    LayoutFlagsSeen = 0;
    LayoutAnimatedSeen = LayoutRepeatedSeen = VertexLoadSeen = false;
    AllowBuffer = allow_buffer;
    FirstClothFailure = fail_cloth;
    ClothPoolSeen = 0;
    CallsInOrder = true;
    const unsigned long rep_count = reps > 1 ? reps : 1;
    const unsigned long index_size = 10 * rep_count;
    const unsigned long src_flags = 0x1e;

    WriteWord(-7);
    WriteWord(reps);
    WriteFloat(1.5f);
    WriteFloat(-2.0f);
    WriteFloat(3.5f);
    WriteFloat(4.0f);
    WriteFloat(5.25f);
    WriteWord(9);
    WriteWord(leading_animated);
    WriteWord(3);
    WriteWord(2);
    WriteWord(5);
    WriteWord(src_flags);
    WriteWord(static_blocks);
    WriteWord(animated_blocks);
    for (unsigned long i = 0; i < static_blocks; ++i)
        WriteWord(70 + i);
    for (unsigned long j = 0; j < animated_blocks; ++j)
        WriteWord(90 + j);
    for (unsigned long k = 0; k < 8; ++k)
        WriteFloat(1.25f + k);
    WriteWord(12);
    WriteWord(VERTEX_NORM_TEX1);
    const unsigned long vertex_payload = FileLength;
    WriteWord(0x3f41);
    const unsigned long index_payload = FileLength;
    for (unsigned long n = 0; n < index_size; ++n) {
        const unsigned char value = (unsigned char)(n + 1);
        Write(&value, sizeof(value));
    }
    WriteWord(cloth);
    const unsigned long cloth_payload = FileLength;
    for (unsigned long c = 0; c < cloth; ++c)
        WriteWord(300 + c);

    union { unsigned long align; unsigned char data[256]; } block;
    memset(block.data, 0xab, sizeof(block.data));
    unsigned char* dest = block.data;
    CMemoryDataInputStream is;
    CMovableResourceMemoryPool pool;
    C3DPrimitive2 prim;
    prim.VertexBufferStatus = prim.IndexBufferStatus = prim.AnimatedBlocksStatus = prim.RepeatedBlocksStatus = 7;
    prim.UploadedFlag = false;
    if (old_index)
        prim.IndexBuffer.reset(new CIndexBuffer);
    prim.ClothPrimitives.resize(old_cloth);
    for (unsigned long old = 0; old < old_cloth; ++old)
        prim.ClothPrimitives[old].Reset(new C3DClothPrimitive);

    const bool result = prim.LoadBinary(is, outer_flags, &dest, &pool);
    const unsigned long flags = src_flags & ~4ul;
    bool ok = result == allow_buffer && CallsInOrder;
    ok &= prim.MaterialIndex == -7 && prim.RepeatingMeshReps == reps &&
          prim.BoundingSphere.Center.X == 1.5f && prim.BoundingSphere.Center.Y == -2.0f &&
          prim.BoundingSphere.Center.Z == 3.5f && prim.BoundingSphere.Radius == 4.0f &&
          prim.AverageTextureStretch == 5.25f;
    ok &= prim.StaticBlockCount == static_blocks && prim.AnimatedBlockCount == animated_blocks &&
          prim.VertexCount == 3 && prim.TriangleCount == 2 && prim.IndexCount == 5;
    ok &= prim.InitFlags == flags && prim.VertexStreamFlags == (6 + (leading_animated > 0)) &&
          (unsigned char*)prim.StaticBlocks == block.data &&
          (unsigned char*)prim.AnimatedBlocks == block.data + 32 * static_blocks &&
          dest == block.data + 32 * static_blocks + 48 * animated_blocks;
    for (unsigned long a = 0; a < static_blocks; ++a)
        ok &= prim.StaticBlocks[a].MaterialIndex == (long)(70 + a);
    for (unsigned long b = 0; b < animated_blocks; ++b)
        ok &= prim.AnimatedBlocks[b].VertexCount == 90 + b;
    ok &= NumberStaticRead == static_blocks && NumberAnimatedRead == animated_blocks &&
          LayoutFlagsSeen == flags && LayoutAnimatedSeen == (animated_blocks > 0) && LayoutRepeatedSeen == (reps > 0) &&
          VertexCountSeen == 3 * rep_count && VertexStrideSeen == (animated_blocks ? 36 : 20) &&
          VertexFlagsSeen == flags && VertexTypeSeen == VERTEX_NORM_TEX1;

    if (!allow_buffer)
        return ok && !VertexLoadSeen && !IndexBytesSeen && !NumberClothRead && !NumberIndexLocks &&
               !prim.UploadedFlag && prim.VertexBufferStatus == 7 && prim.IndexBufferStatus == 7 &&
               prim.AnimatedBlocksStatus == 7 && prim.RepeatedBlocksStatus == 7 && is.GetPosition() == vertex_payload;

    ok &= VertexLoadSeen && OriginalFlagsSeen == src_flags && InputVertexStrideSeen == 12 &&
          ExpandedVertexStrideSeen == VertexStrideSeen && CompressionSeen.PosScaleX == 1.25f &&
          CompressionSeen.PosBiasW == 8.25f && IndexBytesSeen == index_size &&
          IndexFormatSeen == IB_FORMAT_16 && PrimitiveTypeSeen == PT_TRIANGLE_LIST;
    ok &= IndexUsageSeen == (outer_flags & 1 ? 0x18 : 8) &&
          IndexPoolSeen == (outer_flags & 1 ? IB_POOL_SYSTEM_MEM : IB_POOL_MANAGED) &&
          NumberIndexBuffersFreed == (old_index ? 1u : 0u) && NumberIndexLocks == 1 && NumberIndexUnlocks == 1 &&
          NumberCompressedReads == 1 && !memcmp(IndexPayload, FileBytes + index_payload, index_size);
    const bool failed = fail_cloth >= 0 && fail_cloth < (long)cloth;
    const unsigned long loaded = failed ? fail_cloth + 1 : cloth;
    ok &= prim.ClothPrimitives.size() == (failed ? 0 : cloth) && NumberClothRead == loaded &&
          NumberClothFreed == old_cloth + (failed ? loaded : 0) && ClothPoolSeen == (loaded ? &pool : 0);
    if (!failed)
        for (unsigned long i = 0; i < cloth; ++i)
            ok &= prim.ClothPrimitives[i]->PrimitiveIndex == 300 + i;
    ok &= is.GetPosition() == cloth_payload + 4 * loaded && prim.UploadedFlag &&
          prim.VertexBufferStatus == 1 && prim.IndexBufferStatus == 1 &&
          prim.AnimatedBlocksStatus == 1 && prim.RepeatedBlocksStatus == 1;
    return ok;
}

int main()
{
    bool ok = Run(2, 1, 0, 2, 1, 3, -1, true, 1, true) &&
        Run(0, 0, 1, 1, 0, 2, 1, true, 4, false) &&
        Run(1, 2, 1, 0, 1, 0, -1, true, 2, false) &&
        Run(-3, 3, 0, 0, 0, 4, 0, true, 0, true) &&
        Run(2, 1, 1, 0, 1, 2, -1, false, 2, true) &&
        Run(0, 0, 0, 0, 0, 0, -1, true, 0, false);
    puts(ok ? "00a92900_TEST PASS" : "00a92900_TEST FAIL");
    return ok ? 0 : 1;
}