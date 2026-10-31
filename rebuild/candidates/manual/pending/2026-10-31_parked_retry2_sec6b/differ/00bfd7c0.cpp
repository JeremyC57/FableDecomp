// Grade: DIFFER(1241v1231) 22 diff lines (parked 1203v1231, 152 lines): the index-buffer `resize` is the inlined STLport form (`__forceinline`, fill value by const& -> retail's frame byte temp + both arms inline) and the this/stream registers now match; left: the vertex-block size stays in memory (reloaded from [esp+28h]) where retail keeps it in esi across the CByteArray ctor and ReadBlock
// CEngineWaterBackgroundSubPatch::Load(CDataInputStream* stream, unsigned short width, unsigned short height, long mapX, long mapY, float minZ, float maxZ) -- read a sub-patch written by Save: store the placement arguments, read the vertex/poly counts and water type, reallocate the vertex buffer and decompress the range-compressed vertices into it, then reallocate the index buffer and decompress the indices; false when a buffer cannot be allocated
// Leaf: Load
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFD7C0, length 1232 bytes (body 0x4cf), `ret 0x1c`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inlined Read of the landed ReadCompressed 0x00995FB0),
//   CEngineLandscapeMemoryManager (engine landscape object @ 0x01436EA8 + 0x6b0) FreeVertexBuffer @ 0x00BDA0D0 /
//   AllocateVertexBuffer @ 0x00BDA3D0 / FreeIndexBuffer @ 0x00BDA090 / AllocateIndexBuffer @ 0x00BDA360 (the 0xBDA0D0 and
//   0xBDA090 labels are propagated), the resource manager @ 0x01436E98 GetVertexBufferUsage @ 0x00B3B030 /
//   GetIndexBufferUsage @ 0x00B3B060, CVertexBufferWin32::Lock @ 0x00AC20A0 / Unlock @ 0x00AC20B0, CIndexBuffer::Lock
//   @ 0x00AC1FB0 / Unlock @ 0x00AC1F00, vector<unsigned char>(n) @ 0x00514E20, its insert(pos, n, x) @ 0x00485F3A,
//   CRangeCompressor::Decompress(data, count, stride, dest) @ 0x00F39ED0, free @ 0x00BFEA14.
// Layout: engine/CEngineWaterBackgroundSubPatch.h (IndexBuffer +0x08, VertexBuffer +0x0c, PolyCount +0x30, VertexCount +0x32,
//   WaterType +0x40, OcclusionID +0x48, MapX +0x4c, MapY +0x50, Width +0x54, Height +0x56, MinZ +0x58, MaxZ +0x5c);
//   engine/CDataInputStream.h. The empty compressor object is homed in the dead `stream` slot, as in the landed Save 0x00BFD670.
#include "engine/CEngineWaterBackgroundSubPatch.h"
#include "engine/CDataInputStream.h"

extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CByteArray {
    unsigned char* m_begin; unsigned char* m_end; unsigned char* m_capacity;
    CByteArray(unsigned long n);                             // 0x00514E20
    ~CByteArray() { if (m_begin) free(m_begin); }
    unsigned long size() const { return m_end - m_begin; }
    unsigned char* GetData() { return m_end - m_begin != 0 ? m_begin : (unsigned char*)this; }
    void insert(unsigned char* position, unsigned long n, const unsigned char& x);   // 0x00485F3A
    __forceinline void resize(unsigned long n, const unsigned char& x = 0)
    {
        if (n < size())
            m_end = m_begin + n;
        else
            insert(m_end, n - size(), x);
    }
};

struct CDataInputStream_Methods : CDataInputStream {
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
    __forceinline unsigned short ReadUShort() { unsigned short value; Read(&value, sizeof(value)); return value; }
    __forceinline void ReadBlock(void* buffer, long size)
    {
        if (size <= 0)
            return;
        Read(buffer, size);
    }
};

class CRangeCompressor {
public:
    void Decompress(unsigned char* data, long count, long stride, void* dest);   // 0x00F39ED0
};

struct CVertexBufferWin32 {
    void* Lock(unsigned long offset, unsigned long size, unsigned long flags);   // 0x00AC20A0
    void Unlock();                                       // 0x00AC20B0
};
struct CIndexBuffer {
    void* Lock(unsigned long offset, unsigned long size, unsigned long flags);   // 0x00AC1FB0
    void Unlock();                                       // 0x00AC1F00
};

struct CEngineLandscapeMemoryManager {
    void FreeIndexBuffer(CIndexBuffer* buffer);          // 0x00BDA090
    void FreeVertexBuffer(CVertexBufferWin32* buffer);   // 0x00BDA0D0
    CIndexBuffer* AllocateIndexBuffer(void* owner, long size, unsigned long usage, long format, long primitiveType);   // 0x00BDA360
    CVertexBufferWin32* AllocateVertexBuffer(void* owner, long vertexType, long count, long stride, unsigned long usage);   // 0x00BDA3D0
};
struct CEngineLandscapeView {                            // @ 0x01436EA8
    unsigned char _pad000[0x6b0];
    CEngineLandscapeMemoryManager* MemoryManager;        // +0x6b0
};
extern CEngineLandscapeView* g_pEngineLandscape;         // 0x01436EA8

struct CEngineResourceManager {
    unsigned long GetVertexBufferUsage();                // 0x00B3B030
    unsigned long GetIndexBufferUsage();                 // 0x00B3B060
};
extern CEngineResourceManager* g_pResourceManager;       // 0x01436E98

struct CEngineWaterBackgroundSubPatch_Methods : CEngineWaterBackgroundSubPatch {
    bool Load(CDataInputStream_Methods* stream, unsigned short width, unsigned short height, long mapX, long mapY, float minZ, float maxZ);
};

bool CEngineWaterBackgroundSubPatch_Methods::Load(CDataInputStream_Methods* stream, unsigned short width, unsigned short height, long mapX, long mapY, float minZ, float maxZ)
{
    MapX = mapX;
    MapY = mapY;
    Width = width;
    Height = height;
    MinZ = minZ;
    MaxZ = maxZ;
    VertexCount = stream->ReadUShort();
    PolyCount = stream->ReadUShort();
    if (VertexBuffer)
        g_pEngineLandscape->MemoryManager->FreeVertexBuffer(VertexBuffer);
    *(long*)WaterType = stream->ReadLong();
    long stride = stream->ReadLong();
    CEngineLandscapeMemoryManager* memory = g_pEngineLandscape->MemoryManager;
    VertexBuffer = memory->AllocateVertexBuffer(this, 0, VertexCount, stride, g_pResourceManager->GetVertexBufferUsage());
    if (!VertexBuffer)
        return false;
    void* vertices = VertexBuffer->Lock(0, 0, 0);
    long size = stream->ReadLong();
    CByteArray buffer(size);
    stream->ReadBlock(buffer.GetData(), size);
    CRangeCompressor compressor;
    compressor.Decompress(buffer.GetData(), VertexCount, stride, vertices);
    VertexBuffer->Unlock();
    if (IndexBuffer)
        g_pEngineLandscape->MemoryManager->FreeIndexBuffer(IndexBuffer);
    long indexCount = PolyCount * 3;
    memory = g_pEngineLandscape->MemoryManager;
    IndexBuffer = memory->AllocateIndexBuffer(this, indexCount * 2, g_pResourceManager->GetIndexBufferUsage(), 0x65, 4);
    if (!IndexBuffer)
        return false;
    if (indexCount != 0) {
        void* indices = IndexBuffer->Lock(0, 0, 0);
        long indexSize = stream->ReadLong();
        buffer.resize(indexSize);
        stream->ReadBlock(buffer.GetData(), indexSize);
        compressor.Decompress(buffer.GetData(), indexCount, 2, indices);
        IndexBuffer->Unlock();
    }
    OcclusionID = 0;
    return true;
}
