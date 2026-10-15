#include <cstdio>
#include <cstring>

// CPersistTraits<SRecord54>::TransferBinaryIn: fast copy out of the chunk when 0x54 bytes remain, the
// overflow reader otherwise, nothing when the advanced position leaves [0, 0x7FFFFFFF].
#ifdef _MSC_VER
typedef __int64 int64;
#else
typedef long long int64;
#endif

static int g_overflowCalls = 0;

struct CDataInputStream {
    long           Pad00;
    long           ReadPosition;
    long           Pad08;
    unsigned char* Cursor;
    long           Pad10;
    long           RemainingInSourceChunk;
    void ReadWithSrcChunkOverflow(unsigned char* destination, long size)
    {
        ++g_overflowCalls;
        std::memset(destination, 0xAB, size);
        ReadPosition += size;
    }
};
struct SRecord54 { unsigned char Bytes[0x54]; };
struct CPersistTraitsRecord54 {
    void TransferBinaryIn(CDataInputStream* stream, SRecord54* value);
};
void CPersistTraitsRecord54::TransferBinaryIn(CDataInputStream* stream, SRecord54* value)
{
    int64 nextPosition = (unsigned long)(stream->ReadPosition + 0x54);
    if (nextPosition >= 0 && nextPosition <= 0x7FFFFFFF) {
        if (stream->RemainingInSourceChunk >= 0x54) {
            std::memcpy(value, stream->Cursor, 0x54);
            stream->Cursor += 0x54;
            stream->RemainingInSourceChunk -= 0x54;
            stream->ReadPosition += 0x54;
        } else {
            stream->ReadWithSrcChunkOverflow(value->Bytes, 0x54);
        }
    }
}

int main()
{
    static unsigned char chunk[0x100];
    for (int i = 0; i < 0x100; ++i) chunk[i] = (unsigned char)i;
    CDataInputStream s;
    s.Pad00 = s.Pad08 = s.Pad10 = 0;
    s.ReadPosition = 0x10; s.Cursor = chunk; s.RemainingInSourceChunk = 0x60;
    CPersistTraitsRecord54 traits;
    SRecord54 rec;
    traits.TransferBinaryIn(&s, &rec);
    bool ok = rec.Bytes[0] == 0 && rec.Bytes[0x53] == 0x53 && s.Cursor == chunk + 0x54
        && s.RemainingInSourceChunk == 0x0C && s.ReadPosition == 0x64 && g_overflowCalls == 0;
    traits.TransferBinaryIn(&s, &rec);                     // only 0x0C left: overflow reader
    ok = ok && g_overflowCalls == 1 && rec.Bytes[0] == 0xAB && s.Cursor == chunk + 0x54;
    s.ReadPosition = 0x7FFFFFF0;                           // position + 0x54 out of range: untouched
    rec.Bytes[0] = 0x11;
    traits.TransferBinaryIn(&s, &rec);
    ok = ok && g_overflowCalls == 1 && rec.Bytes[0] == 0x11 && s.ReadPosition == 0x7FFFFFF0;
    if (!ok) {
        std::printf("0046bf1c_TEST FAIL\n");
        return 1;
    }
    std::printf("0046bf1c_TEST PASS\n");
    return 0;
}