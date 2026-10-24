// Behaviour test for 0x0046B03A GFSerialiseVectorBinaryIn<CAtmosBankEntry>: count, resize(n, {0,-1}), one traits read per element
#include <cstdio>
#include <cstring>
struct CAtmosBankEntry { unsigned int ID; int Filename; CAtmosBankEntry() : ID(0), Filename(-1) {} };
struct EntryVector {
    CAtmosBankEntry data[8]; unsigned int count; int fillMinusOne;
    void resize(unsigned int n, const CAtmosBankEntry& x) { if (x.ID == 0 && x.Filename == -1) ++fillMinusOne; for (unsigned int i = count; i < n; ++i) data[i] = x; count = n; }
    CAtmosBankEntry& operator[](unsigned int i) { return data[i]; }
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
    CAtmosBankEntry DefaultVal;
    void TransferBinaryIn(Stream& s, CAtmosBankEntry* out) const { s.Read(&out->ID, 4); s.Read(&out->Filename, 4); }
};
static void SerialiseIn(Stream& stream, EntryVector& out)
{
    Traits traits;
    int count;
    stream.Read(&count, 4);
    out.resize(count, CAtmosBankEntry());
    for (int i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, &out[i]);
}
int main()
{
    unsigned char data[4 + 2 * 8] = { 2, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0 };
    Stream s = { 0, data, (int)sizeof(data), 0 };
    EntryVector v; v.count = 0; v.fillMinusOne = 0;
    SerialiseIn(s, v);
    if (v.count != 2 || v.fillMinusOne != 1 || v[0].ID != 1 || v[0].Filename != 2 || v[1].ID != 3 || v[1].Filename != 4 || s.position != 20) {
        std::printf("0046b03a_TEST FAIL\n"); return 1;
    }
    std::printf("0046b03a_TEST PASS\n");
    return 0;
}