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
class CTCShopKeeper { public:
    CRetailVector<CIntelligentPointer> m_recentCustomers;
    void EmptyCustomerList()
    {
        for (long i = 0; i < 3; ++i) {
            CIntelligentPointer empty;
            empty.SetPItem(0);
            m_recentCustomers[i].SetPItem(empty.GetPItem());
        }
    }
};
int main()
{
    CIntelligentPointer slots[3]; CThing t[3];
    for (int i = 0; i < 3; ++i) slots[i].SetPItem(&t[i]);
    CTCShopKeeper k; k.m_recentCustomers.first = slots;
    k.EmptyCustomerList();
    for (int i = 0; i < 3; ++i) if (slots[i].GetPItem() != 0) { std::printf("00798e10_TEST FAIL\n"); return 1; }
    std::printf("00798e10_TEST PASS\n");
    return 0;
}