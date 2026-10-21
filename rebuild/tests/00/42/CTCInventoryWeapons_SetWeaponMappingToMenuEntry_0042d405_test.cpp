// behaviour test for 0x0042D405
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
class CTCInventoryWeapons {
public:
    unsigned char                            _pad00[0x170];
    std::map<NUISystem::CComponent*, long>   WeaponMenuItemMap;   // +0x170
    void SetWeaponMappingToMenuEntry(NUISystem::CComponent* entry, long item);
};
void CTCInventoryWeapons::SetWeaponMappingToMenuEntry(NUISystem::CComponent* entry, long item)
{
    WeaponMenuItemMap[entry] = item;
}

int main()
{
    static CTCInventoryWeapons c; NUISystem::CComponent* e = (NUISystem::CComponent*)0x50; int ok = 1;
    c.SetWeaponMappingToMenuEntry(e, 42);
    ok &= c.WeaponMenuItemMap.lastKey == e && c.WeaponMenuItemMap.value == 42 && (char*)&c.WeaponMenuItemMap - (char*)&c == 0x170;
    printf(ok ? "0042d405_TEST PASS\n" : "0042d405_TEST FAIL\n");
    return ok ? 0 : 1;
}