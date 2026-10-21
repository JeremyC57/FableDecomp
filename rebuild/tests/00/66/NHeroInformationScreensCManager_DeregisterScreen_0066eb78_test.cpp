#include <cstdio>
class CWorld;
extern bool g_bCopyingThings;                            // 0x013B8A88 (name not recovered)
namespace NHeroInformationScreens {
class CBase {
public:
    virtual void _v00() const;
    virtual void _v01() const;
    virtual long GetScreenType() const;                  // slot 2 (+0x08, name not confirmed)
};
template <class T> class CVector {                       // STLport vector, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
class CManager {
public:
    CWorld*               World;                         // +0x00
    CVector<const CBase*> Screens;                       // +0x04
    CVector<const CBase*> InventoryScreens;              // +0x10
    CVector<long>         ScrollBarIndices;              // +0x1c
    void InitialiseScrollBarIndices(long count);         // 0x0066EA85
    void DeregisterScreen(const CBase& screen);
};
void CManager::DeregisterScreen(const CBase& screen)
{
    if (g_bCopyingThings)
        return;
    const CBase*& slot = Screens[screen.GetScreenType()];
    if (slot == &screen)
        slot = 0;
    unsigned long count = 0;
    for (unsigned long i = 0; i < Screens.size(); ++i)
        if (Screens[i])
            ++count;
    if (count < ScrollBarIndices.size())
        InitialiseScrollBarIndices(count);
}
}

bool g_bCopyingThings;
namespace NHeroInformationScreens {
void CBase::_v00() const {} void CBase::_v01() const {}
long CBase::GetScreenType() const { return 0; }
class CTestScreen : public CBase { public: long T; long GetScreenType() const { return T; } };
static long g_initCount = -1; static int g_inits;
void CManager::InitialiseScrollBarIndices(long count) { g_initCount = count; ++g_inits; }
}
using namespace NHeroInformationScreens;
int main()
{
    const CBase* slots[11] = { 0 }; long idx[8];
    CManager m; m.Screens._M_start = slots; m.Screens._M_finish = slots + 11;
    m.ScrollBarIndices._M_start = idx; m.ScrollBarIndices._M_finish = idx + 3;
    CTestScreen a, b, c, x; a.T = 3; b.T = 7; c.T = 0; x.T = 3;
    slots[3] = &a; slots[7] = &b; slots[0] = &c;
    g_bCopyingThings = true; m.DeregisterScreen(a);
    bool ok = slots[3] == &a && g_inits == 0;
    g_bCopyingThings = false; m.DeregisterScreen(x);            // slot holds another screen: kept, count 3 == size: no reset
    ok = ok && slots[3] == &a && g_inits == 0;
    m.DeregisterScreen(a); ok = ok && slots[3] == 0 && g_inits == 1 && g_initCount == 2;
    m.ScrollBarIndices._M_finish = idx + 2;
    m.DeregisterScreen(b); ok = ok && slots[7] == 0 && g_inits == 2 && g_initCount == 1;
    m.ScrollBarIndices._M_finish = idx + 1;
    m.DeregisterScreen(c); ok = ok && slots[0] == 0 && g_inits == 3 && g_initCount == 0;
    m.ScrollBarIndices._M_finish = idx;
    m.DeregisterScreen(c); ok = ok && g_inits == 3;
    std::printf(ok ? "0066eb78_TEST PASS\n" : "0066eb78_TEST FAIL\n");
    return ok ? 0 : 1;
}