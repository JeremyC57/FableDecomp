// behaviour test for 0x0042D41E
#include <stdio.h>
namespace NUISystem { class CComponent; }
namespace std {
template <class K, class V> class map {
public:
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare_pad;
    K lastKey; V value;
    V& operator[](const K& key) { lastKey = key; return value; }
};
}
class CTCInventoryClothing {
public:
    unsigned char                            _pad00[0x160];
    std::map<NUISystem::CComponent*, long>   ClothingItemMap;   // +0x160
    void SetClothingMappingToMenuEntry(NUISystem::CComponent* entry, long item);
};
void CTCInventoryClothing::SetClothingMappingToMenuEntry(NUISystem::CComponent* entry, long item)
{
    ClothingItemMap[entry] = item;
}

int main()
{
    static CTCInventoryClothing c; NUISystem::CComponent* e = (NUISystem::CComponent*)0x50; int ok = 1;
    c.SetClothingMappingToMenuEntry(e, 42);
    ok &= c.ClothingItemMap.lastKey == e && c.ClothingItemMap.value == 42 && (char*)&c.ClothingItemMap - (char*)&c == 0x160;
    printf(ok ? "0042d41e_TEST PASS\n" : "0042d41e_TEST FAIL\n");
    return ok ? 0 : 1;
}