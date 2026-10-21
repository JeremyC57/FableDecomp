// NHeroInformationScreens::CManager::DeregisterScreen(const CBase& screen) -- clear the screen's slot if it still holds it; shrink the scroll slots to the registered-screen count when that falls below them
// Leaf: ?DeregisterScreen@CManager@NHeroInformationScreens@@QAEXABVCBase@2@@Z
// Retail: Fable.exe @ 0x0066EB78, 104 bytes, `ret 4`.
// Body: nothing while the byte flag at 0x013B8A88 is set. Else if Screens[screen slot 2] == &screen it is cleared; count
// the non-null Screens and, when count < ScrollBarIndices.size(), InitialiseScrollBarIndices(count) (0x0066EA85).
// Layout: retail NHeroInformationScreens::CManager as the matched 0x0066EB11 RegisterScreen row.
// Opt: size TU (unaligned entry); the counting loop is strength-reduced to a pointer.
#pragma optimize("s", on)
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