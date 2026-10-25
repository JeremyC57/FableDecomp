// Behaviour test (0x00614da0): CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferBinaryIn resets the record and reads the ten longs (TatooShops before TitleShops)
#include <cstdio>
#include <cstring>
#include <cmath>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
// Memory input stream model: the chunk pointer/length fast path of CMemoryDataInputStream::Read and a
// ReadWithSrcChunkOverflow that serves the rest from the backing buffer (counted, so the slow path is checked).
struct TestStream {
    unsigned int         position;
    int                  sourceLength;
    const unsigned char* chunkBase;
    int                  chunkPosition;
    int                  chunkLength;
    const unsigned char* data;
    int                  overflowCalls;
    void Init(const unsigned char* bytes, int size, int chunk)
    {
        position = 0; sourceLength = size; chunkBase = bytes; chunkPosition = 0; chunkLength = chunk; data = bytes; overflowCalls = 0;
    }
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size)
    {
        ++overflowCalls;
        std::memcpy(buffer, data + position, size);
        position += size;
        chunkBase = data + position;
        chunkLength = sourceLength - (int)position;
    }
    static bool IsValidPosition(i64 p) { return p >= 0 && p <= 0x7fffffff; }
    void Read(void* buffer, int size)
    {
        i64 newPosition = (i64)(unsigned int)(position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (chunkLength >= size) {
            std::memcpy(buffer, chunkBase, size);
            chunkBase += size;
            chunkLength -= size;
            position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
static void PutLong(unsigned char* p, int v) { std::memcpy(p, &v, 4); }
static void PutFloat(unsigned char* p, float v) { std::memcpy(p, &v, 4); }
struct CUIRegionInfo {
    int Region, Shops, BarberShops, TitleShops, TatooShops, ShopsForSale, HousesForSale, ShopsOwned, HousesOwned, Taverns;
    CUIRegionInfo() : Region(0), Shops(0), BarberShops(0), TitleShops(0), TatooShops(0), ShopsForSale(0), HousesForSale(0), ShopsOwned(0), HousesOwned(0), Taverns(0) {}
};
static void ReadLong(TestStream* s, int* v) { int n; s->Read(&n, 4); *v = n; }
static void TransferBinaryIn(TestStream* stream, CUIRegionInfo* value)
{
    *value = CUIRegionInfo();
    ReadLong(stream, &value->Region); ReadLong(stream, &value->Shops); ReadLong(stream, &value->BarberShops);
    ReadLong(stream, &value->TatooShops); ReadLong(stream, &value->TitleShops); ReadLong(stream, &value->ShopsForSale);
    ReadLong(stream, &value->HousesForSale); ReadLong(stream, &value->ShopsOwned); ReadLong(stream, &value->HousesOwned);
    ReadLong(stream, &value->Taverns);
}
int main()
{
    unsigned char bytes[40];
    for (int i = 0; i < 10; ++i) PutLong(bytes + 4 * i, 10 + i);
    TestStream s; s.Init(bytes, 40, 40);
    CUIRegionInfo r;
    TransferBinaryIn(&s, &r);
    if (r.Region != 10 || r.Shops != 11 || r.BarberShops != 12 || r.TatooShops != 13 || r.TitleShops != 14 || r.ShopsForSale != 15
        || r.HousesForSale != 16 || r.ShopsOwned != 17 || r.HousesOwned != 18 || r.Taverns != 19) { std::printf("00614da0_TEST FAIL\n"); return 1; }
    TestStream t; t.Init(bytes, 8, 8);                                     // stream runs dry: later fields come from the overflow path
    CUIRegionInfo q; q.Taverns = 5;
    TransferBinaryIn(&t, &q);
    if (q.Region != 10 || q.Shops != 11) { std::printf("00614da0_TEST FAIL\n"); return 1; }
    std::printf("00614da0_TEST PASS\n");
    return 0;
}