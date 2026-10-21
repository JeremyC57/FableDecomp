// NHeroInformationScreens::CManager::GetAllRegisteredInventoryScreens(vector<const CBase*>& screens) const -- copy every registered screen whose slot-3 test holds into screens
// Leaf: ?GetAllRegisteredInventoryScreens@CManager@NHeroInformationScreens@@ABEXAAV?$vector@PBVCBase@NHeroInformationScreens@@V?$allocator@PBVCBase@NHeroInformationScreens@@@std@@@std@@@Z
// Retail: Fable.exe @ 0x0066E9E9, 93 bytes, `ret 4`; caller Open 0x0066EA46; Ego_r.pdb same symbol (private).
// Body: screens.clear() (erase(begin, end), 0x0066ECE6); for i < Screens.size() (read once): if (Screens[i] &&
// Screens[i]->IsDisplayable()) (virtual slot 3, +0xc) screens.push_back(Screens[i]) (0x0066EDA6, by reference to the element).
// The twin of GetAllRegisteredScreens 0x0066E941 plus the slot-3 test.
// Layout: retail Screens +0x04 (12-byte STLport vector), as modelled at 0x0066E941 / 0x0066EA85. The CBase slot-3 name is
// not confirmed.
// Opt: size TU with frame pointer (`push ebp / mov ebp,esp`, `leave`, unaligned entry; i lives in [ebp-4]).
#pragma optimize("s", on)
#pragma optimize("y", off)
class CWorld;
namespace NHeroInformationScreens {
class CBase {
public:
    virtual void _v00() const;
    virtual void _v01() const;
    virtual void _v02() const;
    virtual bool IsDisplayable() const;                  // slot 3 (+0x0c, name not confirmed)
};
}
namespace std {
template <class T> class allocator {};
template <class T, class A = allocator<T> > class vector {
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
}
namespace NHeroInformationScreens {
class CManager {
public:
    CWorld*                         World;               // +0x00
    std::vector<const CBase*>       Screens;             // +0x04
    std::vector<const CBase*>       InventoryScreens;    // +0x10
    std::vector<long>               ScrollBarIndices;    // +0x1c
private:
    void GetAllRegisteredInventoryScreens(std::vector<const CBase*>& screens) const;
    friend struct CManager_Test;                         // behaviour-test hook
};
void CManager::GetAllRegisteredInventoryScreens(std::vector<const CBase*>& screens) const
{
    screens.clear();
    const long count = Screens.size();
    for (long i = 0; i < count; ++i)
        if (Screens[i] && Screens[i]->IsDisplayable())
            screens.push_back(Screens[i]);
}
}