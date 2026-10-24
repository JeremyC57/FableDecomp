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
#define TEST_ADDR "006d3b90"
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

#include <cstring>
class CCharString {
public:
    char m_text[32];
    CCharString() { m_text[0] = 0; }
    CCharString& operator=(const CCharString& o) { std::strcpy(m_text, o.m_text); return *this; }
};
// Copy of the candidate's inline CMemoryDataInputStream::Read model (member access only).
struct CMemoryDataInputStreamBase {
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
static void InitStream(CMemoryDataInputStreamBase& s, const unsigned char* data, long available, unsigned long pos)
{
    s.m_pVtable = 0; s.position = pos; s.sourceLength = 0; s.chunkBase = data; s.chunkPosition = 0;
    s.chunkLength = available; s.overflowCalls = 0;
}

struct CMemoryDataInputStream : CMemoryDataInputStreamBase {
    int stringReads;
    CCharString ReadString() { ++stringReads; CCharString s; std::strcpy(s.m_text, "Sword"); return s; }
};
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
__forceinline void TransferBinaryInElement(CMemoryDataInputStream* stream, CCharString* out) { *out = stream->ReadString(); }
__forceinline void TransferBinaryInElement(CMemoryDataInputStream* stream, LONG32* out)
{
    LONG32 value;
    stream->Read(&value, sizeof(value));
    *out = value;
}
template <class T> struct CPersistTraits;
template <class T1, class T2> struct CPersistTraits<std::pair<T1, T2> > {
    void TransferBinaryIn(CMemoryDataInputStream* stream, std::pair<T1, T2>* out) const
    {
        TransferBinaryInElement(stream, &out->first);
        TransferBinaryInElement(stream, &out->second);
    }
};
#define CHECK(c) do { if (!(c)) { std::printf("006d3b90_TEST FAIL (line %d)\n", __LINE__); return 1; } } while (0)
int main()
{
    unsigned char data[4] = { 9, 0, 0, 0 };
    CMemoryDataInputStream s;
    InitStream(s, data, 4, 0); s.stringReads = 0;
    std::pair<CCharString, LONG32> p; p.second = 0;
    CPersistTraits<std::pair<CCharString, LONG32> > t;
    t.TransferBinaryIn(&s, &p);
    CHECK(s.stringReads == 1 && std::strcmp(p.first.m_text, "Sword") == 0 && p.second == 9 && s.position == 4);
    InitStream(s, data, 0, 0); s.stringReads = 0;
    t.TransferBinaryIn(&s, &p);
    CHECK(s.overflowCalls == 1 && p.second == (LONG32)0xA3A2A1A0);
    std::printf("006d3b90_TEST PASS\n");
    return 0;
}