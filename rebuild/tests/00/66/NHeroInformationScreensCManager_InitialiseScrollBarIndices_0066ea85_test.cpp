#include <cstdio>
class CWorld;
namespace NHeroInformationScreens {
class CBase;
template <class T> class CVector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    T* erase(T* first, T* last);                         // 0x00440590 for long
    void resize(unsigned int new_size);                  // 0x004433F0 for long
    void clear() { erase(_M_start, _M_finish); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
class CManager {
public:
    CWorld*               World;                         // +0x00
    CVector<const CBase*> Screens;                       // +0x04
    CVector<const CBase*> InventoryScreens;              // +0x10
    CVector<long>         ScrollBarIndices;              // +0x1c
    void InitialiseScrollBarIndices(long count);
};
void CManager::InitialiseScrollBarIndices(long count)
{
    ScrollBarIndices.clear();
    ScrollBarIndices.resize(count);
    for (long i = 0; i < count; ++i)
        ScrollBarIndices[i] = i;
}
}

static long g_buf[16]; static int g_erase, g_resize;
template <> long* NHeroInformationScreens::CVector<long>::erase(long* first, long* last)
{ ++g_erase; if (first != _M_start || last != _M_finish) g_erase = 100; _M_finish = _M_start; return first; }
template <> void NHeroInformationScreens::CVector<long>::resize(unsigned int n)
{ ++g_resize; if (_M_finish != _M_start) g_resize = 100; for (unsigned int i = 0; i < n; ++i) _M_start[i] = 0; _M_finish = _M_start + n; }
int main()
{
    NHeroInformationScreens::CManager m;
    for (int i = 0; i < 16; ++i) g_buf[i] = 77;
    m.ScrollBarIndices._M_start = g_buf; m.ScrollBarIndices._M_finish = g_buf + 3; m.ScrollBarIndices._M_end_of_storage = g_buf + 16;
    m.InitialiseScrollBarIndices(5);
    bool ok = g_erase == 1 && g_resize == 1 && m.ScrollBarIndices._M_finish == g_buf + 5 && g_buf[5] == 77;
    for (int i = 0; i < 5; ++i) ok = ok && g_buf[i] == i;
    m.InitialiseScrollBarIndices(0);
    ok = ok && g_erase == 2 && g_resize == 2 && m.ScrollBarIndices._M_finish == g_buf;
    std::printf(ok ? "0066ea85_TEST PASS\n" : "FAIL\n");
    return 0;
}