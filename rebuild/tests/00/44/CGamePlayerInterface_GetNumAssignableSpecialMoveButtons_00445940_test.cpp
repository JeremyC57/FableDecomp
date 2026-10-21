#include <stdio.h>
struct CInputTypeBase; struct CCPPointerInfo;
template <class T> struct CCountedPointer { T* Data; CCPPointerInfo* Info; };
template <class T> struct CRetailVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };
struct CGamePlayerInterface_Retail {
    unsigned char _pad00[0x7b8];
    CRetailVector<CCountedPointer<const CInputTypeBase> > AssignableSpecialMoveButtons;
    CRetailVector<CCountedPointer<const CInputTypeBase> > QuickAccessItemButtons;
    CRetailVector<CCountedPointer<const CInputTypeBase> > ContextSensitiveItemButtons;
    long GetNumAssignableSpecialMoveButtons() const;
};

long CGamePlayerInterface_Retail::GetNumAssignableSpecialMoveButtons() const
{
    return AssignableSpecialMoveButtons._M_finish - AssignableSpecialMoveButtons._M_start;
}

int main()
{
    static CGamePlayerInterface_Retail g; static CCountedPointer<const CInputTypeBase> buf[6];
    g.AssignableSpecialMoveButtons._M_start = buf; g.AssignableSpecialMoveButtons._M_finish = buf + 5;
    bool ok = g.GetNumAssignableSpecialMoveButtons() == 5 && (char*)&g.AssignableSpecialMoveButtons - (char*)&g == 0x7b8;
    g.AssignableSpecialMoveButtons._M_finish = buf; ok = ok && g.GetNumAssignableSpecialMoveButtons() == 0;
    printf(ok ? "00445940_TEST PASS\n" : "00445940_TEST FAIL\n");
    return ok ? 0 : 1;
}