// Behaviour test for 0x0046C31C GFSerialiseVectorBinaryIn<CObjectFamilyEntry>: count, resize(n, {0,0}), one traits read per element
#include <cstdio>
#include <cstring>
struct CObjectFamilyEntry { int Object; int Probability; };
struct EntryVector {
    CObjectFamilyEntry data[8]; unsigned int count; int fillZero;
    void resize(unsigned int n, const CObjectFamilyEntry& x) { if (x.Object == 0 && x.Probability == 0) ++fillZero; for (unsigned int i = count; i < n; ++i) data[i] = x; count = n; }
    CObjectFamilyEntry& operator[](unsigned int i) { return data[i]; }
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
    CObjectFamilyEntry DefaultVal;
    Traits() { DefaultVal.Object = 0; DefaultVal.Probability = 0; }
    void TransferBinaryIn(Stream& s, CObjectFamilyEntry* out) const { s.Read(&out->Object, 4); s.Read(&out->Probability, 4); }
};
static void SerialiseIn(Stream& stream, EntryVector& out)
{
    Traits traits;
    int count;
    stream.Read(&count, 4);
    CObjectFamilyEntry zero = CObjectFamilyEntry();
    out.resize(count, zero);
    for (int i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, &out[i]);
}
int main()
{
    unsigned char data[4 + 3 * 8] = { 3, 0, 0, 0, 7, 0, 0, 0, 10, 0, 0, 0, 8, 0, 0, 0, 20, 0, 0, 0, 9, 0, 0, 0, 30, 0, 0, 0 };
    Stream s = { 0, data, (int)sizeof(data), 0 };
    EntryVector v; v.count = 0; v.fillZero = 0;
    SerialiseIn(s, v);
    if (v.count != 3 || v.fillZero != 1 || v[0].Object != 7 || v[0].Probability != 10 || v[2].Object != 9 || v[2].Probability != 30 || s.position != 28 || s.overflowCalls != 0) {
        std::printf("0046c31c_TEST FAIL\n"); return 1;
    }
    std::printf("0046c31c_TEST PASS\n");
    return 0;
}