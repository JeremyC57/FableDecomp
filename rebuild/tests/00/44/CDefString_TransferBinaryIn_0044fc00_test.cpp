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
#define TEST_ADDR "0044fc00"
// Copy of the candidate's inline CMemoryDataInputStream::Read model (member access only).
class CMemoryDataInputStream { public:
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

class CDefString {
public:
    LONG32 TablePos;
    void TransferBinaryIn(CMemoryDataInputStream& stream)
    {
        LONG32 tablePos;
        stream.Read(&tablePos, sizeof(tablePos));
        TablePos = tablePos;
    }
};
int main()
{
    unsigned char data[8] = { 0x2A, 0, 0, 0, 0x07, 0, 0, 0 };
    CMemoryDataInputStream s;
    InitStream(s, data, 8, 0);
    CDefString a, b;
    a.TablePos = -1; b.TablePos = -1;
    a.TransferBinaryIn(s);
    b.TransferBinaryIn(s);
    CHECK(a.TablePos == 42 && b.TablePos == 7 && s.position == 8 && s.chunkLength == 0);
    InitStream(s, data, 3, 0);
    a.TransferBinaryIn(s);
    CHECK(s.overflowCalls == 1 && a.TablePos == (LONG32)0xA3A2A1A0);
    std::printf("0044fc00_TEST PASS\n");
    return 0;
}