// CTCInventoryWeapons::SetWeaponMappingToMenuEntry(NUISystem::CComponent*, long) -- records which inventory item a menu entry stands for
// Leaf: SetWeaponMappingToMenuEntry@CTCInventoryWeapons
// Retail: Fable.exe @ 0x0042D405, 25 bytes, `ret 8`; Ego_r.pdb ?SetWeaponMappingToMenuEntry@CTCInventoryWeapons@@QAEXPAVCComponent@NUISystem@@J@Z.
// `map[entry] = item` through the out-of-line std::map<NUISystem::CComponent*, long>::operator[] 0x0042D1B7 on this+0x170.
// Catalog label CTCInventoryClothing::SetClothingMappingToMenuEntry is wrong: the caller at 0x005CB0C7 is
// CTCInventoryWeapons::RefreshWeaponDescriptions, and the sibling at 0x0042D41E (map at +0x160) is the clothing one.
// Layout: retail WeaponMenuItemMap +0x170 (Ego_r.pdb +0x18c; the STLport containers before it are smaller).
namespace NUISystem { class CComponent; }
namespace std {
template <class K, class V> class map {
public:
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare_pad;
    V& operator[](const K& key);                         // 0x0042D1B7
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