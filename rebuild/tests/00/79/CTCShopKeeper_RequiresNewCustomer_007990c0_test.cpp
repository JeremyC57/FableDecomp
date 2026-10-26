#include <cstdio>
class CBaseObject { public: int id; };
class CThing : public CBaseObject { public: };
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    ~CBaseIntelligentPointer() { }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    T& operator[](unsigned long n) { return first[n]; }
};
class CTCShopKeeper;
static CThing* g_rejected = 0;
class CThingFilter_IsPotentialCustomer { public:
    CTCShopKeeper* m_keeper; bool m_checkShop;
    CThingFilter_IsPotentialCustomer(CTCShopKeeper* k, bool c) : m_keeper(k), m_checkShop(c) { }
    bool operator()(CThing* thing) { return thing != g_rejected; }
};
class CTCShopKeeper { public:
    CThing* m_pOwner;
    CIntelligentPointer m_currentCustomer;
    bool RequiresNewCustomer()
    {
        if (m_currentCustomer.GetPItem() != 0) {
            CThing* owner = m_pOwner;
            if ((CThing*)m_currentCustomer.GetPItem() != owner) {
                CThingFilter_IsPotentialCustomer filter(this, true);
                return !filter((CThing*)m_currentCustomer.GetPItem());
            }
        }
        return true;
    }
};
int main()
{
    CThing owner, a, b; g_rejected = &b;
    CTCShopKeeper k; k.m_pOwner = &owner;
    if (!k.RequiresNewCustomer()) { std::printf("007990c0_TEST FAIL\n"); return 1; }
    k.m_currentCustomer.SetPItem(&owner);
    if (!k.RequiresNewCustomer()) { std::printf("007990c0_TEST FAIL\n"); return 1; }
    k.m_currentCustomer.SetPItem(&a);
    if (k.RequiresNewCustomer()) { std::printf("007990c0_TEST FAIL\n"); return 1; }
    k.m_currentCustomer.SetPItem(&b);
    if (!k.RequiresNewCustomer()) { std::printf("007990c0_TEST FAIL\n"); return 1; }
    std::printf("007990c0_TEST PASS\n");
    return 0;
}