// Behaviour test for 0x0046B4AB GFSerialiseVectorBinaryIn<CMusicEntry>: count, resize(n), one traits read per element
#include <cstdio>
#include <cstring>
struct CMusicEntry {
    unsigned int ID; int Filename; float Gain; bool CacheToHD;
    CMusicEntry() : ID(0), Filename(-1), Gain(0.0f), CacheToHD(false) {}
};
struct EntryVector {
    CMusicEntry data[8]; unsigned int count;
    void resize(unsigned int n) { for (unsigned int i = count; i < n; ++i) data[i] = CMusicEntry(); count = n; }
    CMusicEntry& operator[](unsigned int i) { return data[i]; }
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
    CMusicEntry DefaultVal;
    void TransferBinaryIn(Stream& s, CMusicEntry* out) const { s.Read(&out->ID, 4); s.Read(&out->Gain, 4); }
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
    float half = 0.5f; unsigned char data[4 + 8];
    int count = 1; unsigned int id = 77;
    std::memcpy(data, &count, 4); std::memcpy(data + 4, &id, 4); std::memcpy(data + 8, &half, 4);
    Stream s = { 0, data, (int)sizeof(data), 0 };
    EntryVector v; v.count = 0;
    SerialiseIn(s, v);
    if (v.count != 1 || v[0].ID != 77 || v[0].Gain != 0.5f || v[0].Filename != -1 || v[0].CacheToHD || s.position != 12) {
        std::printf("0046b4ab_TEST FAIL\n"); return 1;
    }
    unsigned char big[4] = { 0xff, 0xff, 0xff, 0xff };     // position + 4 beyond 0x7FFFFFFF: the read is skipped
    Stream b = { 0x7ffffffeu, big, 4, 0 };
    int before = (int)v.count;
    Traits t; (void)t;
    int c2 = -5; b.Read(&c2, 4);
    if (c2 != -5 || b.position != 0x7ffffffeu || before != 1) { std::printf("0046b4ab_TEST FAIL\n"); return 1; }
    std::printf("0046b4ab_TEST PASS\n");
    return 0;
}