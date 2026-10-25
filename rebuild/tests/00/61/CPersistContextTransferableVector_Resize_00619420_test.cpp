// Behaviour test for 0x00619420: CPersistContext_TransferableVector<CTCInventoryMap::CUIRegionInfo>::Resize(size) resizes the vector with CUIRegionInfo()
#include <cstdio>
class CTCInventoryMap {
public:
    class CUIRegionInfo {
    public:
        long Region, Shops, BarberShops, TitleShops, TatooShops, ShopsForSale, HousesForSale, ShopsOwned, HousesOwned, Taverns;
        CUIRegionInfo() : Region(0), Shops(0), BarberShops(0), TitleShops(0), TatooShops(0), ShopsForSale(0),
                          HousesForSale(0), ShopsOwned(0), HousesOwned(0), Taverns(0) { }
    };
};

static bool FloatNear(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

template <class T> class TestVector {               // records what the out-of-line resize(n, x) receives
public:
    unsigned int lastN;
    int calls;
    T lastX;
    TestVector() : lastN(0), calls(0) { }
    void resize(unsigned int n, const T& x) { lastN = n; lastX = x; ++calls; }
    void resize(unsigned int n) { resize(n, T()); }  // the reconstructed forwarder
};
template <class T> class TestTransferableVector { // CPersistContext_TransferableVector<T> model
public:
    TestTransferableVector(TestVector<T>& v) : TransVector(&v) { }
    virtual long Size() { return (long)TransVector->lastN; }
    virtual void Resize(long size) { TransVector->resize(size); }   // the reconstructed slot-1 body
    TestVector<T>* TransVector;
};

int main()
{
    TestVector<CTCInventoryMap::CUIRegionInfo > v;
    CTCInventoryMap::CUIRegionInfo& x = v.lastX;
    x.Region = 1; x.Shops = 2; x.BarberShops = 3; x.TitleShops = 4; x.TatooShops = 5; x.ShopsForSale = 6; x.HousesForSale = 7; x.ShopsOwned = 8; x.HousesOwned = 9; x.Taverns = 10;
    TestTransferableVector<CTCInventoryMap::CUIRegionInfo > tv(v);
    TestTransferableVector<CTCInventoryMap::CUIRegionInfo >* p = &tv;
    p->Resize(7);
    if (v.calls != 1 || v.lastN != 7 || !(x.Region == 0 && x.Shops == 0 && x.BarberShops == 0 && x.TitleShops == 0 && x.TatooShops == 0 && x.ShopsForSale == 0 && x.HousesForSale == 0 && x.ShopsOwned == 0 && x.HousesOwned == 0 && x.Taverns == 0)) {
        std::printf("00619420_TEST FAIL\n");
        return 1;
    }
    p->Resize(2);
    if (v.calls != 2 || v.lastN != 2 || !(x.Region == 0 && x.Shops == 0 && x.BarberShops == 0 && x.TitleShops == 0 && x.TatooShops == 0 && x.ShopsForSale == 0 && x.HousesForSale == 0 && x.ShopsOwned == 0 && x.HousesOwned == 0 && x.Taverns == 0)) {
        std::printf("00619420_TEST FAIL\n");
        return 1;
    }
    std::printf("00619420_TEST PASS\n");
    return 0;
}