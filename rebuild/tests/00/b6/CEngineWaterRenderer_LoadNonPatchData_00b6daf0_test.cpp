#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
typedef int LONG32;   // the retail 4-byte long (the g++ host is LP64)
#define TEST_ADDR "00b6daf0"
// Copy of the candidate's inline CMemoryDataInputStream::Read model (member access only).
struct CMemoryDataInputStream {
    void* m_pVtable;
    unsigned long position;
    long sourceLength;
    const unsigned char* chunkBase;
    long chunkPosition;
    long chunkLength;
    int overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++overflowCalls;
        for (long i = 0; i < size; ++i) buffer[i] = (unsigned char)(0xA0 + i);
        position += size;
    }
    static __forceinline bool IsValidPosition(__int64 newPosition) { return newPosition >= 0 && newPosition <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(unsigned long)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            memcpy(buffer, chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
static void InitStream(CMemoryDataInputStream& s, const unsigned char* data, long available, unsigned long pos)
{
    s.m_pVtable = 0; s.position = pos; s.sourceLength = 0; s.chunkBase = data; s.chunkPosition = 0;
    s.chunkLength = available; s.overflowCalls = 0;
}
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

static int g_loadCalls = 0; static LONG32 g_loadSize = 0; static CMemoryDataInputStream* g_loadStream = 0;
struct CWaterSeaStaticMapInfoBlock {
    long SeaStaticMapVersionNumber;
    long DataStreamPosition;
    bool Load(CMemoryDataInputStream& stream, LONG32 blockSize) { ++g_loadCalls; g_loadSize = blockSize; g_loadStream = &stream; return blockSize == 8; }
};
struct CEngineWaterRendererLoader {
    void LoadNonPatchData(CMemoryDataInputStream& data, CMemoryDataInputStream& header)
    {
        LONG32 blockSize;
        header.Read(&blockSize, sizeof(blockSize));
        CWaterSeaStaticMapInfoBlock info;
        info.Load(header, blockSize);
    }
};
int main()
{
    unsigned char hdr[12] = { 8, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    CMemoryDataInputStream data, header;
    InitStream(data, 0, 0, 0);
    InitStream(header, hdr, 12, 0);
    CEngineWaterRendererLoader r;
    r.LoadNonPatchData(data, header);
    CHECK(g_loadCalls == 1 && g_loadSize == 8 && g_loadStream == &header && header.position == 4 && data.position == 0);
    InitStream(header, hdr, 2, 0);
    r.LoadNonPatchData(data, header);
    CHECK(g_loadCalls == 2 && header.overflowCalls == 1 && g_loadSize == (LONG32)0xA3A2A1A0);
    std::printf("00b6daf0_TEST PASS\n");
    return 0;
}