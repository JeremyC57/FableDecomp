// Behaviour test for 0x0046B9B0 GFSerialiseVectorBinaryIn<CMusicSetEntry>: count, resize(n, T()), one traits read per element
#include <cstdio>
#include <cstring>
class CMusicSetEntry { public: int NormalMusicID; int DangerMusicID; int LoopCount; CMusicSetEntry() : NormalMusicID(0), DangerMusicID(0), LoopCount(123) {} };
struct EntryVector {
    CMusicSetEntry data[8]; unsigned int count;
    void resize(unsigned int n, const CMusicSetEntry& x) { for (unsigned int i = count; i < n; ++i) data[i] = x; count = n; }
    CMusicSetEntry& operator[](unsigned int i) { return data[i]; }
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
    CMusicSetEntry DefaultVal;
    void TransferBinaryIn(Stream& s, CMusicSetEntry* out) const { s.Read(&out->NormalMusicID, 4); s.Read(&out->DangerMusicID, 4); s.Read(&out->LoopCount, 4); }
};
static void SerialiseIn(Stream& stream, EntryVector& out)
{
    Traits traits;
    int count;
    stream.Read(&count, 4);
    out.resize(count, CMusicSetEntry());
    for (int i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, &out[i]);
}
int main()
{
    unsigned char data[4 + 12] = { 1, 0, 0, 0, 9, 0, 0, 0, 8, 0, 0, 0, 7, 0, 0, 0 };
    Stream s = { 0, data, (int)sizeof(data), 0 };
    EntryVector v; v.count = 0;
    SerialiseIn(s, v);
    if (v.count != 1 || v[0].NormalMusicID != 9 || v[0].DangerMusicID != 8 || v[0].LoopCount != 7 || s.position != 16) {
        std::printf("0046b9b0_TEST FAIL\n"); return 1;
    }
    unsigned char two[4] = { 3, 0, 0, 0 };                  // count 3 but no element data: the overflow reader zeroes them
    Stream t = { 0, two, 4, 0 };
    SerialiseIn(t, v);
    if (v.count != 3 || t.overflowCalls != 9 || v[2].LoopCount != 0) { std::printf("0046b9b0_TEST FAIL\n"); return 1; }
    std::printf("0046b9b0_TEST PASS\n");
    return 0;
}