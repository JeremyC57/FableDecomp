// Behaviour test for 0x0046BE89 GFSerialiseVectorBinaryIn<CReverbEnvironmentEntry>: count, resize(n), one raw read per element
#include <cstdio>
#include <cstring>
struct CReverbEnvironmentEntry { float Params[21]; };
struct EntryVector {
    CReverbEnvironmentEntry data[4]; unsigned int count;
    void resize(unsigned int n) { for (unsigned int i = count; i < n; ++i) std::memset(&data[i], 0, sizeof(data[i])); count = n; }
    CReverbEnvironmentEntry& operator[](unsigned int i) { return data[i]; }
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
struct Traits {
    CReverbEnvironmentEntry DefaultVal;
    Traits() { std::memset(&DefaultVal, 0, sizeof(DefaultVal)); }
    void TransferBinaryIn(Stream& s, CReverbEnvironmentEntry* out) const { s.Read(out, (int)sizeof(*out)); }
};
static void SerialiseIn(Stream& stream, EntryVector& out)
{
    Traits traits;
    int count;
    stream.Read(&count, 4);
    out.resize(count);
    for (int i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, &out[i]);
}
int main()
{
    unsigned char data[4 + 2 * 0x54];
    int count = 2; std::memcpy(data, &count, 4);
    for (int i = 0; i < 42; ++i) { float f = (float)i; std::memcpy(data + 4 + i * 4, &f, 4); }
    Stream s = { 0, data, (int)sizeof(data), 0 };
    EntryVector v; v.count = 0;
    SerialiseIn(s, v);
    if (v.count != 2 || v[0].Params[0] != 0.0f || v[0].Params[20] != 20.0f || v[1].Params[0] != 21.0f || v[1].Params[20] != 41.0f || s.position != 4 + 2 * 0x54) {
        std::printf("0046be89_TEST FAIL\n"); return 1;
    }
    std::printf("0046be89_TEST PASS\n");
    return 0;
}