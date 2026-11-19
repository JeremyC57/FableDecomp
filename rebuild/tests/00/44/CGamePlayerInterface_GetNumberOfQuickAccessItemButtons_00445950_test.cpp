#include <stdio.h>
class CInputTypeBase; class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };
template <class T> struct CRetailVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };
struct CGamePlayerInterface_Retail {
    unsigned char _pad00[0x7b8];
    CRetailVector<CCountedPointer<const CInputTypeBase> > AssignableSpecialMoveButtons;
    CRetailVector<CCountedPointer<const CInputTypeBase> > QuickAccessItemButtons;
    CRetailVector<CCountedPointer<const CInputTypeBase> > ContextSensitiveItemButtons;
    long GetNumQuickAccessItemButtons() const;
};

long CGamePlayerInterface_Retail::GetNumQuickAccessItemButtons() const
{
    return QuickAccessItemButtons._M_finish - QuickAccessItemButtons._M_start;
}

int main()
{
    static CGamePlayerInterface_Retail g; static CCountedPointer<const CInputTypeBase> buf[6];
    g.QuickAccessItemButtons._M_start = buf; g.QuickAccessItemButtons._M_finish = buf + 5;
    bool ok = g.GetNumQuickAccessItemButtons() == 5 && (char*)&g.QuickAccessItemButtons - (char*)&g == 0x7c4;
    g.QuickAccessItemButtons._M_finish = buf; ok = ok && g.GetNumQuickAccessItemButtons() == 0;
    printf(ok ? "00445950_TEST PASS\n" : "00445950_TEST FAIL\n");
    return ok ? 0 : 1;
}