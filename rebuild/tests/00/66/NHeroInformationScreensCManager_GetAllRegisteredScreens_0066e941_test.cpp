#include <cstdio>
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

namespace NHeroInformationScreens {
template <class T> T* CVector<T>::erase(T* first, T* last) { _M_finish = first; return first; }
template <class T> void CVector<T>::push_back(const T& x) { *_M_finish++ = x; }
}
int main()
{
    using namespace NHeroInformationScreens;
    static const CBase* src[4] = { (const CBase*)1, 0, (const CBase*)3, (const CBase*)4 };
    static const CBase* dst[8] = { (const CBase*)9 };
    static CManager m; m.Screens._M_start = src; m.Screens._M_finish = src + 4;
    CVector<const CBase*> out; out._M_start = dst; out._M_finish = dst + 1; out._M_end_of_storage = dst + 8;
    m.GetAllRegisteredScreens(out);
    bool ok = out._M_finish == dst + 3 && dst[0] == (const CBase*)1 && dst[1] == (const CBase*)3 && dst[2] == (const CBase*)4;
    if (ok) std::printf("0066e941_TEST PASS\n");
    return 0;
}