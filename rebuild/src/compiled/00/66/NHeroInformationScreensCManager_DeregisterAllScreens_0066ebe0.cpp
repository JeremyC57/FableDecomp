// NHeroInformationScreens::CManager::DeregisterAllScreens() -- null every registered screen slot, then rebuild the scroll-bar indices from 0
// Leaf: ?DeregisterAllScreens@CManager@NHeroInformationScreens@@QAEXXZ
// Retail: Fable.exe @ 0x0066EBE0, 42 bytes; Ego_r.pdb same symbol. Zeroes Screens[i] (`and dword ptr [edx+eax*4],0`)
// for i < Screens.size() (re-read each pass), then calls InitialiseScrollBarIndices(0) (0x0066EA85). Caller 0x0089F636.
// Layout: Ego_r.pdb NHeroInformationScreens::CManager: World +0x00, Screens +0x04 (12-byte STLport vector), as 0x0066E7E1.
// Opt: size TU (`and dword ptr [m],0`).
#pragma optimize("s", on)
class CWorld;
template <class T> class vector {                        // STLport vector<T>
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(_M_start + n); }
};
namespace NHeroInformationScreens {
class CBase;
class CManager {
public:
    void DeregisterAllScreens();
    void InitialiseScrollBarIndices(long first);         // 0x0066EA85
    CWorld*              World;                          // +0x00
    vector<const CBase*> Screens;                        // +0x04
};
void CManager::DeregisterAllScreens()
{
    for (unsigned int i = 0; i < Screens.size(); ++i)
        Screens[i] = 0;
    InitialiseScrollBarIndices(0);
}
}