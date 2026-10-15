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
struct CThingCreatureBase : CBaseObject { };
struct CShopperList {
    CBaseObject* m_items[8]; int m_count;
    void PushBack(const CIntelligentPointer& p) { m_items[m_count++] = p.GetPItem(); }
};
struct CTCShop {
    CShopperList m_shoppers;
    void RemoveShopper(CThingCreatureBase* s)
    {
        int j = 0;
        for (int i = 0; i < m_shoppers.m_count; ++i) if (m_shoppers.m_items[i] != s) m_shoppers.m_items[j++] = m_shoppers.m_items[i];
        m_shoppers.m_count = j;
    }
    void AddShopper(CThingCreatureBase* shopper)
    {
        RemoveShopper(shopper);
        CIntelligentPointer pointer;
        pointer.SetPItem((CBaseObject*)shopper);
        m_shoppers.PushBack(pointer);
    }
};
int main()
{
    CThingCreatureBase a, b;
    CTCShop shop; shop.m_shoppers.m_count = 0;
    shop.AddShopper(&a); shop.AddShopper(&b); shop.AddShopper(&a);
    if (shop.m_shoppers.m_count != 2 || shop.m_shoppers.m_items[0] != &b || shop.m_shoppers.m_items[1] != &a) { std::printf("00773dc0_TEST FAIL\n"); return 1; }
    std::printf("00773dc0_TEST PASS\n");
    return 0;
}
