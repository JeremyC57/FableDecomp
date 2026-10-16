// CTCInventoryClothing::SetThingAsWearingSelectedClothes(CThing* thing) -- strips the thing's clothing modifiers and re-applies every selected suit part's item
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B406D, length 121 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound @ 0x0040F020,
// CTCHeroAttachableAppearanceModifiers::RemoveAllAppearanceModifiers(type) @ 0x00706640 and
// AddAppearanceModifierWithReplace(long) @ 0x00706950 (interface 0x5E), the STLport tree successor
// _Rb_global<bool>::_M_increment @ 0x004292C0 (__cdecl; manifest "CMemoryAllocatorVariableSize::GetNoAllocatedAreas", byte twin).
// Shape: the inline GetInterface (map::find form: `found = (it == last || key < it->type) ? last : it`) keeps 0x5E in edi (`push 5Eh ; pop edi`) and the key in the dead `thing` slot; the map walk
// re-reads end() (the header pointer) every pass and only positive item indices are applied.
// Layout: engine/CTCInventoryClothing.h (PDB) has CurrentClothingSelected at +0x15C with 16-byte containers; retail has
// 12-byte containers and a narrower base, so the map (header pointer first) sits at +0x140 and a local view carries the PDB name.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->type) ? map->last : it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};
enum { APPEARANCE_MODIFIER_CLOTHING = 2 };
struct CTCHeroAttachableAppearanceModifiers {        // interface type 0x5E
    void RemoveAllAppearanceModifiers(long type);    // 0x00706640
    void AddAppearanceModifierWithReplace(long defIndex);   // 0x00706950
};
struct SNode {                                       // _Rb_tree_node<pair<const EClothingSuitPart, long>>
    long   _M_color;
    SNode* _M_parent;
    SNode* _M_left;
    SNode* _M_right;
    long   first;                                    // +0x10 EClothingSuitPart
    long   second;                                   // +0x14 item def index
};
SNode* __cdecl _M_increment(SNode* node);            // 0x004292C0 (_Rb_global<bool>::_M_increment)
struct CSuitPartMap {                                // std::map<EClothingSuitPart, long> (12 bytes)
    SNode* _M_header;
    unsigned long _M_node_count;
    long _M_key_compare;
    SNode* begin() { return _M_header->_M_left; }
    SNode* end() { return _M_header; }
};
struct CTCInventoryClothing_Retail {
    void* __vftable;                                 // +0x000
    unsigned char _pad_0x004[0x13C];
    CSuitPartMap CurrentClothingSelected;            // +0x140
    void SetThingAsWearingSelectedClothes(CThing* thing);
};
void CTCInventoryClothing_Retail::SetThingAsWearingSelectedClothes(CThing* thing)
{
    CTCHeroAttachableAppearanceModifiers* modifiers;
    if (thing->GetInterface(&modifiers, 0x5E)) {
        modifiers->RemoveAllAppearanceModifiers(APPEARANCE_MODIFIER_CLOTHING);
        for (SNode* it = CurrentClothingSelected.begin(); it != CurrentClothingSelected.end(); it = _M_increment(it)) {
            if (it->second > 0)
                modifiers->AddAppearanceModifierWithReplace(it->second);
        }
    }
}