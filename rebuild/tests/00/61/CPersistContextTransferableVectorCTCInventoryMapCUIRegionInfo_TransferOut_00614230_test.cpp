// Behaviour test for 0x00614230 CPersistContext_TransferableVector<CTCInventoryMap::CUIRegionInfo>::TransferOut: a zeroed default region record, &first[index].
#include <cstdio>
#ifndef _MSC_VER
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

struct CPersistContext { int id; };
struct CStringParser { int id; };
struct CCharString { int id; };
struct CUIRegionInfo {
    long Region, Shops, BarberShops, TitleShops, TatooShops, ShopsForSale, HousesForSale, ShopsOwned, HousesOwned, Taverns;
    CUIRegionInfo()
        : Region(0), Shops(0), BarberShops(0), TitleShops(0), TatooShops(0), ShopsForSale(0), HousesForSale(0),
          ShopsOwned(0), HousesOwned(0), Taverns(0)
    {
    }
};
template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](long index) { return *(m_pFirst + index); }
};
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, T* value);
    void TransferIn(CPersistContext* context, CStringParser* parser, T* value);
};

static int g_calls = 0, g_defaultsOk = 0;
static CPersistContext* g_ctx = 0; static void* g_arg = 0; static CUIRegionInfo* g_value = 0;

template <>
void CPersistTraits<CUIRegionInfo>::TransferOut(CPersistContext* context, CCharString* arg, CUIRegionInfo* value)
{
    ++g_calls; g_ctx = context; g_arg = arg; g_value = value;
    g_defaultsOk = DefaultVal.Region == 0 && DefaultVal.Shops == 0 && DefaultVal.TatooShops == 0 && DefaultVal.HousesOwned == 0 && DefaultVal.Taverns == 0;
}

struct CPersistContext_TransferableVector {
    virtual long GetCount();
    virtual void Resize(long count);
    virtual void TransferOut(CPersistContext* context, CCharString* out, long index);
    virtual void TransferIn(CPersistContext* context, CStringParser* parser, long index);
    CRetailVector<CUIRegionInfo>* TransVector;
};

void CPersistContext_TransferableVector::TransferOut(CPersistContext* context, CCharString* out, long index)
{
    CPersistTraits<CUIRegionInfo> traits;
    traits.TransferOut(context, out, &(*TransVector)[index]);
}

long CPersistContext_TransferableVector::GetCount() { return (long)(TransVector->m_pLast - TransVector->m_pFirst); }
void CPersistContext_TransferableVector::Resize(long) { }
void CPersistContext_TransferableVector::TransferIn(CPersistContext*, CStringParser*, long) { }


int main()
{
    CUIRegionInfo elems[3];
    CRetailVector<CUIRegionInfo> vec;
    vec.m_pFirst = elems; vec.m_pLast = elems + 3; vec.m_pEnd = elems + 3;
    CPersistContext_TransferableVector wrapper;
    wrapper.TransVector = &vec;
    CPersistContext context;
    CCharString arg;
    wrapper.TransferOut(&context, &arg, 2);
    CPersistContext_TransferableVector* hook = &wrapper;
    hook->TransferOut(&context, &arg, 0);
    if (g_calls != 2 || g_ctx != &context || g_arg != (void*)&arg || g_value != &elems[0] || !g_defaultsOk || !(1) || hook->GetCount() != 3) {
        std::printf("00614230_TEST FAIL\n"); return 1;
    }
    std::printf("00614230_TEST PASS\n");
    return 0;
}