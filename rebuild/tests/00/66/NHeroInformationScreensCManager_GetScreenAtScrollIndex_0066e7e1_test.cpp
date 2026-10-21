// test for 0066e7e1 GetScreenAtScrollIndex
#include <stdio.h>
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

int main()
{
    using namespace NHeroInformationScreens;
    static const CBase* screens[3] = { (const CBase*)0x10, (const CBase*)0x20, (const CBase*)0x30 };
    static long indices[4] = { 2, 0, 3, -1 };
    CManager m;
    m.InventoryScreens._M_start = screens; m.InventoryScreens._M_finish = screens + 3; m.InventoryScreens._M_end_of_storage = screens + 3;
    m.ScrollBarIndices._M_start = indices; m.ScrollBarIndices._M_finish = indices + 4; m.ScrollBarIndices._M_end_of_storage = indices + 4;
    int ok = m.GetScreenAtScrollIndex(0) == screens[2] && m.GetScreenAtScrollIndex(1) == screens[0]
          && m.GetScreenAtScrollIndex(2) == 0 && m.GetScreenAtScrollIndex(3) == 0;
    printf(ok ? "0066e7e1_TEST PASS\n" : "0066e7e1_TEST FAIL\n");
    return ok ? 0 : 1;
}