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
#define TEST_ADDR "00405940"
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

template <class T> struct CPersistTraits {
    void TransferBinaryIn(CMemoryDataInputStream* stream, T* out) const;
};
template <> void CPersistTraits<unsigned char>::TransferBinaryIn(CMemoryDataInputStream* stream, unsigned char* out) const
{
    LONG32 value;
    stream->Read(&value, sizeof(value));
    *out = (unsigned char)value;
}
int main()
{
    CPersistTraits<unsigned char> traits;
    unsigned char data[16];
    for (int i = 0; i < 16; ++i) data[i] = 0;
    LONG32 src = (LONG32)0x1234567F;
    memcpy(data, &src, sizeof(src));
    CMemoryDataInputStream s;
    // fast path: enough bytes in the chunk
    InitStream(s, data, 16, 100);
    unsigned char out = 0;
    traits.TransferBinaryIn(&s, &out);
    CHECK(out == 0x7F);
    CHECK(s.position == 100 + sizeof(LONG32) && s.chunkLength == (long)(16 - sizeof(LONG32)) && s.chunkBase == data + sizeof(LONG32) && s.overflowCalls == 0);
    // overflow path: chunk too short, the overflow reader fills 0xA0, 0xA1, ...
    InitStream(s, data, 2, 100);
    traits.TransferBinaryIn(&s, &out);
    CHECK(s.overflowCalls == 1 && s.chunkBase == data && s.chunkLength == 2);
    CHECK(out == 0xA0);
    // range failure: position + size beyond 0x7FFFFFFF reads nothing
    InitStream(s, data, 16, 0x7ffffffeUL);
    traits.TransferBinaryIn(&s, &out);
    CHECK(s.position == 0x7ffffffeUL && s.chunkBase == data && s.overflowCalls == 0);
    std::printf("00405940_TEST PASS\n");
    return 0;
}