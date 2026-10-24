// Behaviour test for 0x00614700 GFSerialiseVectorBinaryOut<CTCInventoryMap::CUIRegionInfo>(stream, vector): write the count
// (WriteSLONG 0x00993EB0), then every region record through CPersistTraits<CUIRegionInfo>::TransferBinaryOut (0x005FA610:
// ten longs, TatooShops before TitleShops, as the landed body writes them).
#include <cstdio>

struct CDataOutputStreamWriter {
    long m_longs[64];
    int  m_n;
    CDataOutputStreamWriter() : m_n(0) { }
    void WriteSLONG(long value) { m_longs[m_n++] = value; }
};

struct CTCInventoryMap {
    struct CUIRegionInfo {
        long Region, Shops, BarberShops, TitleShops, TatooShops, ShopsForSale, HousesForSale, ShopsOwned, HousesOwned, Taverns;
    };
};

namespace stl {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferBinaryOut(CDataOutputStreamWriter& stream, const T* value) const
    {
        stream.WriteSLONG(value->Region);
        stream.WriteSLONG(value->Shops);
        stream.WriteSLONG(value->BarberShops);
        stream.WriteSLONG(value->TatooShops);
        stream.WriteSLONG(value->TitleShops);
        stream.WriteSLONG(value->ShopsForSale);
        stream.WriteSLONG(value->HousesForSale);
        stream.WriteSLONG(value->ShopsOwned);
        stream.WriteSLONG(value->HousesOwned);
        stream.WriteSLONG(value->Taverns);
    }
};

template <class T>
void GFSerialiseVectorBinaryOut(CDataOutputStreamWriter& stream, stl::vector<T>& value)
{
    CPersistTraits<T> traits;
    long count = (long)value.size();
    stream.WriteSLONG(count);
    for (long i = 0; i < count; ++i)
        traits.TransferBinaryOut(stream, &value[i]);
}

typedef CTCInventoryMap::CUIRegionInfo R;

int main()
{
    int ok = 1;
    R regions[2];
    long* fields[2][10];
    for (int r = 0; r < 2; ++r) {
        R& x = regions[r];
        long* f[10] = { &x.Region, &x.Shops, &x.BarberShops, &x.TitleShops, &x.TatooShops, &x.ShopsForSale,
                        &x.HousesForSale, &x.ShopsOwned, &x.HousesOwned, &x.Taverns };
        for (int i = 0; i < 10; ++i) { fields[r][i] = f[i]; *f[i] = 100 * r + i; }
    }
    stl::vector<R> v;
    v._M_start = regions; v._M_finish = regions + 2; v._M_end_of_storage = regions + 2;
    CDataOutputStreamWriter w;
    GFSerialiseVectorBinaryOut(w, v);
    if (w.m_n != 21 || w.m_longs[0] != 2) ok = 0;
    int order[10] = { 0, 1, 2, 4, 3, 5, 6, 7, 8, 9 };
    for (int r = 0; r < 2; ++r)
        for (int i = 0; i < 10; ++i)
            if (w.m_longs[1 + 10 * r + i] != 100 * r + order[i]) ok = 0;
    if (!ok) { std::printf("00614700_TEST FAIL\n"); return 1; }
    std::printf("00614700_TEST PASS\n");
    return 0;
}