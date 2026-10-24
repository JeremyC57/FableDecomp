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
#define TEST_ADDR "00479369"
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

static int g_ctors = 0;
class COpinionTransientOffset {
public:
    unsigned char m_data[0x18];
    COpinionTransientOffset() { ++g_ctors; std::memset(m_data, 0, sizeof(m_data)); }
    void TransferBinaryIn(CMemoryDataInputStream* stream) { stream->Read(m_data, sizeof(m_data)); }
};
struct COffsetList {
    int clears; int count; unsigned char first[8];
    void clear() { ++clears; count = 0; }
    void push_back(const COpinionTransientOffset& v) { first[count++] = v.m_data[0]; }
};
template <class T> struct CPersistTraits {
    void TransferBinaryIn(CMemoryDataInputStream* stream, T* out) const;
};
template <> __forceinline void CPersistTraits<LONG32>::TransferBinaryIn(CMemoryDataInputStream* stream, LONG32* out) const
{
    LONG32 value;
    stream->Read(&value, sizeof(value));
    *out = value;
}
void TransferBinaryIn(CMemoryDataInputStream* stream, COffsetList* out)
{
    out->clear();
    LONG32 count;
    CPersistTraits<LONG32>().TransferBinaryIn(stream, &count);
    for (LONG32 i = count; i > 0; --i) {
        COpinionTransientOffset value;
        value.TransferBinaryIn(stream);
        out->push_back(value);
    }
}
int main()
{
    unsigned char data[4 + 2 * 0x18];
    std::memset(data, 0, sizeof(data));
    data[0] = 2; data[4] = 0x11; data[4 + 0x18] = 0x22;
    CMemoryDataInputStream s;
    InitStream(s, data, sizeof(data), 0);
    COffsetList l; l.clears = 0; l.count = 9;
    TransferBinaryIn(&s, &l);
    CHECK(l.clears == 1 && l.count == 2 && l.first[0] == 0x11 && l.first[1] == 0x22 && g_ctors == 2 && s.position == sizeof(data));
    std::printf("00479369_TEST PASS\n");
    return 0;
}