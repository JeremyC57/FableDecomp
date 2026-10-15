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
    bool HasBeenCustomer(CThing* thing)
    {
        for (long i = 0; i < 3; ++i) {
            CIntelligentPointer& customer = m_recentCustomers[i];
            if (customer.GetPItem() == (CBaseObject*)thing)
                return true;
        }
        return false;
    }
};
int main()
{
    CIntelligentPointer slots[3]; CThing t[4];
    for (int i = 0; i < 3; ++i) slots[i].SetPItem(&t[i]);
    CTCShopKeeper k; k.m_recentCustomers.first = slots;
    if (!k.HasBeenCustomer(&t[0]) || !k.HasBeenCustomer(&t[2]) || k.HasBeenCustomer(&t[3])) { std::printf("00798dd0_TEST FAIL\n"); return 1; }
    std::printf("00798dd0_TEST PASS\n");
    return 0;
}