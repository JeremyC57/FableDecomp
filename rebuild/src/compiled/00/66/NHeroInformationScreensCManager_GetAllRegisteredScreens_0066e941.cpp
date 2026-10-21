// NHeroInformationScreens::CManager::GetAllRegisteredScreens(vector<const CBase*>& screens) const -- copy every non-null registered screen into screens
// Leaf: ?GetAllRegisteredScreens@CManager@NHeroInformationScreens@@QBEXAAV?$CVector@PBVCBase@NHeroInformationScreens@@@2@@Z
// Retail: Fable.exe @ 0x0066E941, 68 bytes, `ret 4`; callers 0x0049F42C, 0x0066E9B1; Ego_r.pdb
// ?GetAllRegisteredScreens@CManager@NHeroInformationScreens@@QBEXAAV?$vector@PBVCBase@NHeroInformationScreens@@...@std@@@Z.
// Body: screens.clear() (erase(begin, end), 0x0066ECE6); for i < Screens.size() (read once): if (Screens[i])
// screens.push_back(Screens[i]) (0x0066EDA6, by reference to the element).
// Layout: retail Screens +0x04 (12-byte STLport vector), as modelled at 0x0066EA85.
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
class CWorld;
namespace NHeroInformationScreens {
class CBase;
template <class T> class CVector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    T* erase(T* first, T* last);                         // 0x0066ECE6 for const CBase*
    void push_back(const T& x);                          // 0x0066EDA6 for const CBase*
    void clear() { erase(_M_start, _M_finish); }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    const T& operator[](unsigned int n) const { return _M_start[n]; }
};
class CManager {
public:
    CWorld*               World;                         // +0x00
    CVector<const CBase*> Screens;                       // +0x04
    CVector<const CBase*> InventoryScreens;              // +0x10
    CVector<long>         ScrollBarIndices;              // +0x1c
    void GetAllRegisteredScreens(CVector<const CBase*>& screens) const;
};
void CManager::GetAllRegisteredScreens(CVector<const CBase*>& screens) const
{
    screens.clear();
    const long count = Screens.size();
    for (long i = 0; i < count; ++i)
        if (Screens[i])
            screens.push_back(Screens[i]);
}
}