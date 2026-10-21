// CTCInventoryClothing::SetClothingMappingToMenuEntry(NUISystem::CComponent*, long) -- records which inventory item a menu entry stands for
// Leaf: SetClothingMappingToMenuEntry@CTCInventoryClothing
// Retail: Fable.exe @ 0x0042D41E, 25 bytes, `ret 8`; Ego_r.pdb ?SetClothingMappingToMenuEntry@CTCInventoryClothing@@QAEXPAVCComponent@NUISystem@@J@Z.
// `map[entry] = item` through the out-of-line std::map<NUISystem::CComponent*, long>::operator[] 0x0042D1B7 on this+0x160.
// Sibling of CTCInventoryWeapons::SetWeaponMappingToMenuEntry 0x0042D405 (map at +0x170, called from
// CTCInventoryWeapons::RefreshWeaponDescriptions); this one is called from the UI manager 0x004238F8.
// Layout: retail ClothingItemMap +0x160 (Ego_r.pdb +0x184; the STLport containers before it are smaller).
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