// Behaviour test for 0x00469CB2 GFSerialiseVectorBinaryIn<CTextureReplacementDef::CEntry>: count, resize, one entry per element
#include <cstdio>
#include <cstring>
struct CEntry { int OriginalTexture; int NewTexture; };
struct EntryVector {
    CEntry data[8]; unsigned int count; int resizeCalls;
    void resize(unsigned int n, const CEntry& x) { ++resizeCalls; for (unsigned int i = count; i < n; ++i) data[i] = x; count = n; }
    CEntry& operator[](unsigned int i) { return data[i]; }
};
struct Stream {
    unsigned int position; const unsigned char* chunkBase; int chunkLength; int overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size) { ++overflowCalls; std::memset(buffer, 0, size); }
    void Read(void* buffer, int size)
    {
        long long newPosition = (long long)(unsigned int)(position + size);
        if (!(newPosition >= 0 && newPosition <= 0x7fffffff)) return;
        if (chunkLength >= size) { std::memcpy(buffer, chunkBase, size); chunkBase += size; chunkLength -= size; position += size; }
        else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
struct Traits {                                    // DefaultVal(0, 0); TransferBinaryIn @ 0x00469D47 reads two longs
    CEntry DefaultVal; int calls;
    Traits() : calls(0) { DefaultVal.OriginalTexture = 0; DefaultVal.NewTexture = 0; }
    void TransferBinaryIn(Stream& s, CEntry* out) { ++calls; s.Read(&out->OriginalTexture, 4); s.Read(&out->NewTexture, 4); }
};
static int SerialiseIn(Stream& stream, EntryVector& out)
{
    Traits traits;
    int count;
    stream.Read(&count, 4);
    CEntry unset;
    out.resize(count, unset);
    for (int i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, &out[i]);
    return traits.calls;
}
int main()
{
    unsigned char data[4 + 2 * 8] = { 2, 0, 0, 0, 7, 0, 0, 0, 8, 0, 0, 0, 9, 0, 0, 0, 10, 0, 0, 0 };
    Stream s = { 0, data, (int)sizeof(data), 0 };
    EntryVector v; v.count = 0; v.resizeCalls = 0;
    int calls = SerialiseIn(s, v);
    if (calls != 2 || v.count != 2 || v.resizeCalls != 1 || v[0].OriginalTexture != 7 || v[0].NewTexture != 8
        || v[1].OriginalTexture != 9 || v[1].NewTexture != 10 || s.position != 20) {
        std::printf("00469cb2_TEST FAIL\n"); return 1;
    }
    std::printf("00469cb2_TEST PASS\n");
    return 0;
}