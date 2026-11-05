// CWaterPatchMesh::Load(CDataInputStream& stream) -- read the patch's water body span and water type, allocate its 17x17 foreground vertex buffer from the landscape memory manager, then range-decompress the stored vertex block and unpack each CTVertexWaterForeground into the locked buffer
// Grade: DIFFER(1592v1592) -- residue: esi / edi role swap. Retail keeps the locked vertex pointer in edi and the loop's memory stream `this` in esi through the 289-vertex loop (and `this` in esi before it); ours colours them the other way round, so every instruction of the loop that touches either register differs while the opcode stream and the frame are the same (236 diff lines, all register names). 14 grades (a..u).
// Next idea: the swap starts at the vertex-buffer Lock call (retail `mov edi,eax`): try taking the vertex pointer as `CTVertexWaterForeground* vertices` before constructing the memory stream (declare the stream after Lock), or index the buffer (`vertices[i]`) with the stream held in a reference local, so that the stream's `this` is the later-defined value.
// Leaf: ?Load@CWaterPatchMesh@@AAE_NAAVCDataInputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BEAE50, 1592 bytes, thiscall, `ret 4`; private (Ego_r.pdb
//   ?Load@CWaterPatchMesh@@AAE_NAAVCDataInputStream@@@Z), called from the ctor 0x00BEB490.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (slow path of the inline reads),
//   CEngineResourceManager::GetVertexBufferUsage 0x00B3B030 on the resource manager @ 0x01436E98,
//   CEngineLandscapeMemoryManager::AllocateVertexBuffer 0x00BDA3D0 on the landscape renderer's MemoryManager (+0x6B0 of
//   @ 0x01436EA8), std::vector<unsigned char>::vector(size_type) 0x00514E20 (CArray<unsigned char> locals),
//   CRangeCompressor::Decompress 0x00F39ED0, CMemoryDataInputStream(const void*, unsigned long) 0x00994930 and its
//   compiler-generated destructor (a direct call of ~CDataStream 0x00993B10), CVertexBufferBase::Lock 0x00AC20A0 /
//   Unlock 0x00AC20B0, free 0x00BFEA14 (the vector destructors).
// Shape: the float / long / count reads land in the dead `stream` argument slot (the stream lives in ebp); the stored
//   water type goes through a jump table whose cases 8, 2, 4, 5, 3, 6 copy the value and everything else (0, 1, 7, >8)
//   sets 1 (default block; enumerator names of EWaterType are not recovered, so the values are written as numbers);
//   the two byte arrays are declared decompressed (289 * 66 bytes) first, compressed second; the vertex loop walks a
//   pointer over the 289 locked vertices reading each field through the memory stream, the depth short scaled by
//   1/16384 and clamped to [0, 2] in the inlined SetTargetVertexDepth; the 48-byte shore table is read in place.
#define FABLE_RETAIL_CWATERPATCHMESH
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#include "engine/CWaterPatchMesh.h"
#include "engine/CTVertexWaterForeground.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CArray.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CEngineResourceManager.h"

class CRangeCompressor {
public:
    void Decompress(const unsigned char* data, long count, long stride, void* output);   // 0x00F39ED0 ?Decompress@CRangeCompressor@@QAEXPBEJJPAX@Z
};

extern CEngineLandscapeRenderer* g_LandscapeRenderer_01436EA8;          // 0x01436EA8
extern CEngineResourceManager*   g_pResourceManager;                     // 0x01436E98

enum { WATER_PATCH_VERTEX_COUNT = 17 * 17, WATER_PATCH_PACKED_VERTEX_SIZE = 0x42 };

inline void CWaterPatchMesh::SetTargetVertexDepth(CTVertexWaterForeground& vertex, short depth)
{
    float value = (float)depth * (1.0f / 16384.0f);
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
    case 8: WaterType = (EWaterType)8; break;
    case 2: WaterType = (EWaterType)2; break;
    case 4: WaterType = (EWaterType)4; break;
    case 5: WaterType = (EWaterType)5; break;
    case 3: WaterType = (EWaterType)3; break;
    case 6: WaterType = (EWaterType)6; break;
    case 0:
    case 1:
    case 7: WaterType = (EWaterType)1; break;
    default: WaterType = (EWaterType)1; break;
    }

    CEngineLandscapeMemoryManager* memory = g_LandscapeRenderer_01436EA8->MemoryManager;
    VertexBuffer = memory->AllocateVertexBuffer(this, VERTEX_NULL, WATER_PATCH_VERTEX_COUNT, sizeof(CTVertexWaterForeground),
                                                g_pResourceManager->GetVertexBufferUsage());
    if (!VertexBuffer)
        return false;

    long compressedSize = stream.ReadSLONG();
    CArray<unsigned char> decompressed(WATER_PATCH_VERTEX_COUNT * WATER_PATCH_PACKED_VERTEX_SIZE);
    CArray<unsigned char> compressed(compressedSize);
    stream.Read(compressed.GetAsCArray(), compressedSize);
    CRangeCompressor compressor;
    compressor.Decompress(compressed.GetAsCArray(), WATER_PATCH_VERTEX_COUNT, WATER_PATCH_PACKED_VERTEX_SIZE, decompressed.GetAsCArray());

    CMemoryDataInputStream vertexStream(decompressed.GetAsCArray(), decompressed.size());
    CTVertexWaterForeground* vertices = (CTVertexWaterForeground*)VertexBuffer->Lock(0, 0, 0);
    for (long i = 0; i < WATER_PATCH_VERTEX_COUNT; i++) {
        vertices[i].X = vertexStream.ReadType<unsigned short>();
        vertices[i].Y = vertexStream.ReadType<unsigned short>();
        vertices[i].Z = vertexStream.ReadFloat();
        vertices[i].SwellData0 = vertexStream.ReadType<unsigned short>();
        vertices[i].SwellData1 = vertexStream.ReadType<unsigned short>();
        SetTargetVertexDepth(vertices[i], vertexStream.ReadType<short>());
        vertices[i].ShoreUCoord = vertexStream.ReadFloat();
        vertexStream.Read(vertices[i].ShoreLookUpArray, sizeof(vertices[i].ShoreLookUpArray));
    }
    VertexBuffer->Unlock();
    return true;
}
