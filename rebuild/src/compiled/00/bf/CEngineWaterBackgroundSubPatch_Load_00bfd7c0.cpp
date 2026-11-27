// 00bfd7c0 CEngineWaterBackgroundSubPatch::Load
// Leaf: ?Load@CEngineWaterBackgroundSubPatch@@QAE_NAAVCDataInputStream@@GGJJMM@Z
// Flags: /Gr
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEINDEXBUFFER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINEWATERBACKGROUNDSUBPATCH_ACCESSORS
#define FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22
#define FABLE_RETAIL_PASS22_WATER_PATCH_LOAD
#define FABLE_RETAIL_CDATASTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_WATER_SUBPATCH_LOAD_PASS29
#define FABLE_RETAIL_CARRAY_SIZE_CTOR_OUTOFLINE
#define FABLE_RETAIL_VECTOR_BYTE_RESIZE_VALUE_OWNER_PASS29
#define FABLE_RETAIL_VECTOR_BYTE_RESIZE_ONE_OWNER_PASS29
#define FABLE_RETAIL_STLP_VECTOR_ORIGINAL_RESIZE_INSERT_CHAIN_PASS22
#define FABLE_RETAIL_WATER_BYTE_RESIZE_SPELLINGS_PASS29
#define FABLE_RETAIL_STLP_COPY
#define FABLE_RETAIL_WATER_BYTE_ERASE_OWNER_PASS29
#include "engine/CDataInputStream.h"
#include "engine/CEngineWaterBackgroundSubPatch.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CIndexBuffer.h"
#include "engine/EPrimitiveType.h"
#include "engine/EIndexBufferFormat.h"
#include "engine/CRangeCompressor.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }
using namespace NEngineInternal;

bool CEngineWaterBackgroundSubPatch::Load(CDataInputStream& stream, unsigned short width, unsigned short height, long world_x, long world_y, float min_z, float max_z)
{
    MapX = world_x;
    MapY = world_y;
    Width = width;
    Height = height;
    MinZ = min_z;
    MaxZ = max_z;
    VertexCount = stream.ReadUWORD();
    PolyCount = stream.ReadUWORD();
    if (VertexBuffer) GPLandscapeRenderer->GetMemoryManager()->DeleteVertexBuffer(VertexBuffer);
    WaterType = static_cast<EWaterType>(stream.ReadSLONG());
    long vertex_stride = stream.ReadSLONG();
    VertexBuffer = GPLandscapeRenderer->GetMemoryManager()->AllocateVertexBuffer(this, VERTEX_NULL, VertexCount, vertex_stride, GPResourceManager->GetVertexBufferUsage());
    if (!VertexBuffer) return false;
    unsigned char* vbdata = VertexBuffer->Lock(0,0,0);
    long comp_size = stream.ReadSLONG();
    CArray<unsigned char> comp_buffer(comp_size);
    stream.Read(comp_buffer.GetAsCArray(),comp_size);
    CRangeCompressor compressor;
    compressor.Decompress(comp_buffer.GetAsCArray(),VertexCount,vertex_stride,vbdata);
    VertexBuffer->Unlock();
    if (IndexBuffer) GPLandscapeRenderer->GetMemoryManager()->DeleteIndexBuffer(IndexBuffer);
    const long index_count = PolyCount * 3;
    IndexBuffer = GPLandscapeRenderer->GetMemoryManager()->AllocateIndexBuffer(this,index_count*2,GPResourceManager->GetIndexBufferUsage(),IB_FORMAT_16,PT_TRIANGLE_LIST);
    if (!IndexBuffer) return false;
    if (index_count) {
        unsigned char* ibdata = IndexBuffer->Lock(0,0,0);
        comp_size = stream.ReadSLONG();
        comp_buffer.resize(comp_size);
        stream.Read(comp_buffer.GetAsCArray(),comp_size);
        compressor.Decompress(comp_buffer.GetAsCArray(),index_count,2,ibdata);
        IndexBuffer->Unlock();
    }
    OcclusionID = 0;
    return true;
}