// CWaterPatchMesh::Load @ 0x00BEAE50.
// Worker: full1592-byte relocation-masked match including switch table; independent main verification pending.
// Retail: Fable.exe TLC @0x00BEAE50..0x00BEB488,1556 code bytes +36 switch-table bytes;thiscall,ret4.
// Leaf: ?Load@CWaterPatchMesh@@AAE_NAAVCDataInputStream@@@Z
// Inline-budget: genuine vector(n) body is called via scoped MS noinline after constructor CPP/budget/depth trials.
// Read span/type; allocate289 native68-byte vertices;decompress66 packed bytes per vertex;unpack and release arrays.
// Original PDB enum/manager names and const Offset restored. Signed depth uses native float(2/32767),bits38800100.
// Original positive Read scope restores the native full-body code; native2/32767 depth scale retained.
#define FABLE_RETAIL_PASS22_WATER_PATCH_LOAD
#define FABLE_RETAIL_CWATERPATCHMESH
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_SIZE_CTOR_OUTOFLINE      // NF93 (q11297): CArray<unsigned char>(n) 0x00514E20 (FableWin ??0?$CArray@E@@QAE@I@Z), out of line
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#include "engine/EWaterType.h"
#include "engine/CWaterPatchMesh.h"
#include "engine/CTVertexWaterForeground.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CArray.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CEngineResourceManager.h"

#include "engine/CRangeCompressor.h"

namespace NEngineInternal {
extern CEngineLandscapeRenderer* GPLandscapeRenderer; // Original private/global PDB identity, native01436EA8.
extern CEngineResourceManager* GPResourceManager; // Existing canonical binding01436E98.
}

enum { WATER_PATCH_VERTEX_COUNT = 17 * 17, WATER_PATCH_PACKED_VERTEX_SIZE = 0x42 };

inline void CWaterPatchMesh::SetTargetVertexDepth(CTVertexWaterForeground& vertex, short depth)
{
    float value = (float)depth * (2.0f / 32767.0f);
    if (value < 0.0f)
        value = 0.0f;
    if (value > 2.0f)
        value = 2.0f;
    vertex.Depth = value;
}

bool CWaterPatchMesh::Load(CDataInputStream& stream)
{
    WaterBodySpan = stream.ReadFloat();
    long type = stream.ReadSLONG();
    switch (type) {
    case WATER_TYPE_ICE: WaterType = WATER_TYPE_ICE; break;
    case WATER_TYPE_RIVER: WaterType = WATER_TYPE_RIVER; break;
    case WATER_TYPE_REFLECTIVE_SEA: WaterType = WATER_TYPE_REFLECTIVE_SEA; break;
    case WATER_TYPE_NON_REFLECTIVE_SEA: WaterType = WATER_TYPE_NON_REFLECTIVE_SEA; break;
    case WATER_TYPE_SEA: WaterType = WATER_TYPE_SEA; break;
    case WATER_TYPE_OLD: WaterType = WATER_TYPE_OLD; break;
    case WATER_TYPE_NULL:
    case WATER_TYPE_LAKE:
    case WATER_TYPE_DUMMY_SHORE_POINT: WaterType = WATER_TYPE_LAKE; break;
    default: WaterType = WATER_TYPE_LAKE; break;
    }

    CEngineLandscapeMemoryManager* memory = NEngineInternal::GPLandscapeRenderer->GetMemoryManager();
    VertexBuffer = memory->AllocateVertexBuffer(this, VERTEX_NULL, WATER_PATCH_VERTEX_COUNT, sizeof(CTVertexWaterForeground),
                                                NEngineInternal::GPResourceManager->GetVertexBufferUsage());
    if (!VertexBuffer)
        return false;

    long compressedSize = stream.ReadSLONG();
    CArray<unsigned char> uncomp_buffer(WATER_PATCH_VERTEX_COUNT * WATER_PATCH_PACKED_VERTEX_SIZE);
    CArray<unsigned char> comp_buffer(compressedSize);
    stream.Read(comp_buffer.GetAsCArray(), compressedSize);
    CRangeCompressor compressor;
    compressor.Decompress(comp_buffer.GetAsCArray(), WATER_PATCH_VERTEX_COUNT, WATER_PATCH_PACKED_VERTEX_SIZE, uncomp_buffer.GetAsCArray());

    CMemoryDataInputStream vertex_stream(uncomp_buffer.GetAsCArray(), uncomp_buffer.size());
    CTVertexWaterForeground* vertices = (CTVertexWaterForeground*)VertexBuffer->Lock(0, 0, 0);
    for (long i = 0; i < WATER_PATCH_VERTEX_COUNT; i++) {
        vertices[i].X = vertex_stream.ReadType<unsigned short>();
        vertices[i].Y = vertex_stream.ReadType<unsigned short>();
        vertices[i].Z = vertex_stream.ReadFloat();
        vertices[i].SwellData0 = vertex_stream.ReadType<unsigned short>();
        vertices[i].SwellData1 = vertex_stream.ReadType<unsigned short>();
        SetTargetVertexDepth(vertices[i], vertex_stream.ReadType<short>());
        vertices[i].ShoreUCoord = vertex_stream.ReadFloat();
        vertex_stream.Read(vertices[i].ShoreLookUpArray, sizeof(vertices[i].ShoreLookUpArray));
    }
    VertexBuffer->Unlock();
    return true;
}