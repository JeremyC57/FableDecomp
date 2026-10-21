// NHeroInformationScreens::CManager::GetScreenAtScrollIndex(long) -- the inventory screen the scroll-bar slot maps to, or null when out of range
// Leaf: ?GetScreenAtScrollIndex@CManager@NHeroInformationScreens@@QAEPBVCBase@2@J@Z
// Retail: Fable.exe @ 0x0066E7E1, 36 bytes; Ego_r.pdb same symbol. Reads ScrollBarIndices[i] (+0x1c start), compares it
// unsigned against InventoryScreens.size() (+0x10/+0x14, `sar edx,2`) and returns InventoryScreens[screen] or 0.
// Layout: Ego_r.pdb NHeroInformationScreens::CManager (World +0x00, Screens, InventoryScreens, ScrollBarIndices) with
// 12-byte STLport vectors, which puts InventoryScreens at +0x10 and ScrollBarIndices at +0x1c (PDB +0x14/+0x24).
// Opt: size TU (unaligned entry; /O2 is one byte longer).
#pragma optimize("s", on)
class CWorld;
template <class T> class vector {                        // STLport vector<T>
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    const T& operator[](unsigned int n) const { return *(_M_start + n); }
};
namespace NHeroInformationScreens {
class CBase;
class CManager {
public:
    const CBase* GetScreenAtScrollIndex(long scroll_index);
    CWorld*              World;                          // +0x00
    vector<const CBase*> Screens;                        // +0x04
    vector<const CBase*> InventoryScreens;               // +0x10
    vector<long>         ScrollBarIndices;               // +0x1c
};
const CBase* CManager::GetScreenAtScrollIndex(long scroll_index)
{
    long screen = ScrollBarIndices[scroll_index];
    if (screen < InventoryScreens.size())
        return InventoryScreens[screen];
    return 0;
}
}