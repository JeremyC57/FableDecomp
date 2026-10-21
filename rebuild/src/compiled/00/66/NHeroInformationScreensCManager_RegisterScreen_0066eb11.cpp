// NHeroInformationScreens::CManager::RegisterScreen(const CBase& screen) -- slot the screen in by type; while fewer than five scroll slots exist, reset them to the registered-screen count (at most five)
// Leaf: ?RegisterScreen@CManager@NHeroInformationScreens@@QAEXABVCBase@2@@Z
// Retail: Fable.exe @ 0x0066EB11, 103 bytes, `ret 4`; Ego_r.pdb ?RegisterScreen@CManager@NHeroInformationScreens@@QAEXABVCBase@2@@Z.
// Body: nothing while the byte flag at 0x013B8A88 is set (name not recovered; the landed CTCBase::Copy family names it
// after its copy role). Else Screens[screen slot 2 (+0x08)] = &screen; if ScrollBarIndices.size() < 5: count the
// non-null Screens (size re-read each pass) and, when count <= 5, InitialiseScrollBarIndices(count) (0x0066EA85).
// Layout: retail NHeroInformationScreens::CManager -- Screens +0x04, ScrollBarIndices +0x1c (12-byte STLport vectors), as
// 0x0066EA85. The CBase slot-2 name is not confirmed.
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
    void RegisterScreen(const CBase& screen);
};
void CManager::RegisterScreen(const CBase& screen)
{
    if (g_bCopyingThings)
        return;
    Screens[screen.GetScreenType()] = &screen;
    if (ScrollBarIndices.size() < 5)
    {
        long count = 0;
        for (unsigned long i = 0; i < Screens.size(); ++i)
            if (Screens[i])
                ++count;
        if (count <= 5)
            InitialiseScrollBarIndices(count);
    }
}
}