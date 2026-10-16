#include <cstdio>
#include <cstring>
// Behaviour test for CContainmentVolume::LoadBinary @ 0x00A754E0.
struct CPlane { float X, Y, Z, D; };
static unsigned char g_poolMemory[256];
static float g_source[8] = {1, 2, 3, 4, 5, 6, 7, 8};
struct Stream {
    unsigned long StreamPos; void* CurrentSourcePtr; long SourceChunkBytesRemaining;
    int virtualReads; long lastSize;
    virtual void Read(void* buffer, long size) { std::memcpy(buffer, g_source, size); ++virtualReads; lastSize = size; }
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { std::memset(buffer, 0, size); }
    void ReadInline(void* buffer, long size)
    {
        long long newPosition = (long long)(StreamPos + size);
        if (!(newPosition >= 0 && newPosition <= 0x7fffffff)) return;
        if (SourceChunkBytesRemaining >= size) {
            std::memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    unsigned int ReadULong() { unsigned int v; ReadInline(&v, 4); return v; }
};
struct Pool {
    bool fail; unsigned long requested;
    bool AllocateResource(long* handle, void*, unsigned long size, unsigned long) { requested = size; if (fail) return false; *handle = 7; return true; }
    void* GetPointerFromHandle(long handle) { return handle == 7 ? g_poolMemory : 0; }
};
struct Volume {
    CPlane* ContainmentPlanes; unsigned long Size; Pool* MemoryPool;
    bool LoadBinary(Stream& stream, Pool* pool)
    {
        Size = stream.ReadULong();
        MemoryPool = pool;
        if (Size > 0) {
            long handle;
            if (!pool->AllocateResource(&handle, this, Size * 16, 0)) return false;
            ContainmentPlanes = (CPlane*)MemoryPool->GetPointerFromHandle(handle);
            stream.Read(ContainmentPlanes, Size * 16);
        } else ContainmentPlanes = 0;
        return true;
    }
};
static int Fail() { std::printf("00a754e0_TEST FAIL\n"); return 1; }
int main()
{
    unsigned int count = 2;
    Stream s; s.StreamPos = 0; s.CurrentSourcePtr = &count; s.SourceChunkBytesRemaining = 4; s.virtualReads = 0;
    Pool p; p.fail = false; Volume v;
    if (!v.LoadBinary(s, &p) || v.Size != 2 || p.requested != 32 || v.ContainmentPlanes != (CPlane*)g_poolMemory) return Fail();
    if (s.virtualReads != 1 || s.lastSize != 32 || v.ContainmentPlanes[1].D != 8 || s.StreamPos != 4) return Fail();
    unsigned int zero = 0; Stream s2 = s; s2.CurrentSourcePtr = &zero; s2.SourceChunkBytesRemaining = 4; s2.virtualReads = 0;
    if (!v.LoadBinary(s2, &p) || v.ContainmentPlanes != 0 || s2.virtualReads != 0) return Fail();
    Stream s3 = s; s3.CurrentSourcePtr = &count; s3.SourceChunkBytesRemaining = 4; p.fail = true;
    if (v.LoadBinary(s3, &p)) return Fail();
    std::printf("00a754e0_TEST PASS\n");
    return 0;
}
