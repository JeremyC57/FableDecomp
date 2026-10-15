#include <cstdio>
struct CBaseObject { int id; };
struct CThing : CBaseObject { };
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
struct CTCShopKeeper {
    CRetailVector<CIntelligentPointer> m_recentCustomers;
    long m_nextCustomerSlot;
    void AddNewCustomer(CIntelligentPointer customer)
    {
        m_recentCustomers[m_nextCustomerSlot++].SetPItem(customer.GetPItem());
        if (m_nextCustomerSlot == 3)
            m_nextCustomerSlot = 0;
    }
};
int main()
{
    CIntelligentPointer slots[3]; CThing t[4];
    CTCShopKeeper k; k.m_recentCustomers.first = slots; k.m_nextCustomerSlot = 0;
    for (int i = 0; i < 4; ++i) { CIntelligentPointer p; p.SetPItem(&t[i]); k.AddNewCustomer(p); }
    if (slots[0].GetPItem() != &t[3] || slots[1].GetPItem() != &t[1] || slots[2].GetPItem() != &t[2] || k.m_nextCustomerSlot != 1) { std::printf("00798d90_TEST FAIL\n"); return 1; }
    std::printf("00798d90_TEST PASS\n");
    return 0;
}
