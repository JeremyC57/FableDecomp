// Behaviour test for 0x00619560 GFSerialiseVectorBinaryIn<CTCInventoryMap::CUIRegionInfo>: the count read inline (bounds check,
// fast chunk copy or the ReadWithSrcChunkOverflow slow path), resize with the traits' zeroed DefaultVal, then one
// TransferBinaryIn per record. Stream, vector and traits are small C++03 models; the callees are stubs.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
typedef long long Int64;
#else
typedef __int64 Int64;
#endif
class CMemoryDataInputStream {
public:
    unsigned long StreamPos;
    const unsigned char* CurrentSourcePtr;
    long SourceChunkBytesRemaining;
    const unsigned char* Overflow;          // the stub's next chunk
    int OverflowCalls;
    void Read(void* buffer, long size)
    {
        if (size <= 0) return;
        Int64 newPosition = (Int64)(StreamPos + size);
        if (newPosition < 0 || newPosition > 0x7fffffff) return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    long ReadSLONG() { long v; Read(&v, sizeof(v)); return v; }
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++OverflowCalls; memcpy(buffer, Overflow, size); Overflow += size; StreamPos += size;
    }
};
class CTCInventoryMap { public:
    class CUIRegionInfo { public:
        long Region, Shops, BarberShops, TitleShops, TatooShops, ShopsForSale, HousesForSale, ShopsOwned, HousesOwned, Taverns;
        CUIRegionInfo() : Region(0), Shops(0), BarberShops(0), TitleShops(0), TatooShops(0), ShopsForSale(0), HousesForSale(0),
                          ShopsOwned(0), HousesOwned(0), Taverns(0) { }
    };
};
namespace std {
template <class T> class vector {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { delete[] _M_start; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
    void resize(unsigned int n, const T& x)
    {
        T* p = new T[n];
        for (unsigned int i = 0; i < n; ++i) p[i] = i < size() ? _M_start[i] : x;
        delete[] _M_start; _M_start = p; _M_finish = _M_end_of_storage = p + n;
    }
};
}
static int g_transfers = 0;
template <class T> class CPersistTraits {
public:
    T DefaultVal;
    void TransferBinaryIn(CMemoryDataInputStream& stream, T& value)
    {
        ++g_transfers;
        if (DefaultVal.Region != 0 || DefaultVal.Taverns != 0) return;     // the default record stays zero
        stream.Read(&value, sizeof(value));
    }
};
template <class T>
void __fastcall GFSerialiseVectorBinaryIn(CMemoryDataInputStream& stream, std::vector<T>& out)
{
    long count = stream.ReadSLONG();
    CPersistTraits<T> traits;
    out.resize(count, traits.DefaultVal);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, out[i]);
}
int main()
{
    long data[1 + 2 * 10];
    data[0] = 2;
    for (int i = 0; i < 20; ++i) data[1 + i] = 100 + i;
    CMemoryDataInputStream s; s.StreamPos = 0; s.CurrentSourcePtr = (const unsigned char*)data; s.SourceChunkBytesRemaining = sizeof(data);
    s.Overflow = 0; s.OverflowCalls = 0;
    std::vector<CTCInventoryMap::CUIRegionInfo> v;
    GFSerialiseVectorBinaryIn(s, v);
    int ok = v.size() == 2 && v[0].Region == 100 && v[0].Taverns == 109 && v[1].Region == 110 && v[1].Taverns == 119
             && g_transfers == 2 && s.StreamPos == sizeof(data) && s.OverflowCalls == 0;
    // slow path: an empty chunk, the count (0) comes from the overflow reader; the vector is resized to nothing
    long zero = 0;
    CMemoryDataInputStream t; t.StreamPos = 8; t.CurrentSourcePtr = 0; t.SourceChunkBytesRemaining = 0;
    t.Overflow = (const unsigned char*)&zero; t.OverflowCalls = 0;
    std::vector<CTCInventoryMap::CUIRegionInfo> w;
    GFSerialiseVectorBinaryIn(t, w);
    ok = ok && t.OverflowCalls == 1 && w.size() == 0 && g_transfers == 2 && t.StreamPos == 8 + sizeof(long);
    puts(ok ? "00619560_TEST PASS" : "00619560_TEST FAIL");
    return ok ? 0 : 1;
}