#include <cstdio>
// Behaviour test for CTCTavernTable::GetFood (0x007B85C0).
class CThingObject { public: int id; };
struct CIntelligentPointer {
    CThingObject* m_pItem;
    CThingObject* GetPItem() const { return m_pItem; }
};
template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) {}
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() { return iterator(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
struct CTCTavernTable {
    CRetailVector<void*> Occupants;
    CRetailVector<CIntelligentPointer> Tankards;
    CRetailVector<CIntelligentPointer> Food;
    CThingObject* GetFood(unsigned long index);
};
CThingObject* CTCTavernTable::GetFood(unsigned long index)
{
    if (Food.size() > index)
        return Food[index].GetPItem();
    return 0;
}
int main()
{
    CThingObject a, b; CIntelligentPointer items[2]; items[0].m_pItem = &a; items[1].m_pItem = &b;
    CIntelligentPointer other[1]; other[0].m_pItem = 0;
    CTCTavernTable t;
    t.Tankards.first = t.Tankards.last = t.Tankards.end_of_storage = other;
    t.Food.first = t.Food.last = t.Food.end_of_storage = other;
    t.Food.first = items; t.Food.last = items + 2; t.Food.end_of_storage = items + 2;
    if (t.GetFood(0) != &a || t.GetFood(1) != &b || t.GetFood(2) != 0 || t.GetFood(0xFFFFFFFFUL) != 0)
    { std::printf("007b85c0_TEST FAIL\n"); return 1; }
    std::printf("007b85c0_TEST PASS\n");
    return 0;
}
