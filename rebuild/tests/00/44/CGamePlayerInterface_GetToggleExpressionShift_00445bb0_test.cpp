#include <stdio.h>
#include <stddef.h>
struct CDefPointeeBase;
struct CGamePlayerInterface { unsigned char _pad[0x7a0]; CDefPointeeBase* PControlsDef_Object; };
struct CControlsDef {
    unsigned char _base[0x3c]; void* Controls[3];
    bool ToggleZTarget, ToggleSpells, ToggleSneak, ToggleExpressionMenu, ToggleExpressionShift, FlourishNeedsAttackButtonHeld;
};
struct CGamePlayerInterface_Methods : CGamePlayerInterface {
    const CControlsDef* GetControlsDef() const { return (const CControlsDef*)PControlsDef_Object; }
    bool GetToggleExpressionShift() const;
};

bool CGamePlayerInterface_Methods::GetToggleExpressionShift() const
{
    return GetControlsDef()->ToggleExpressionShift;
}

int main()
{
    static CGamePlayerInterface_Methods gpi; static CControlsDef def;
    gpi.PControlsDef_Object = (CDefPointeeBase*)&def;
    def.ToggleExpressionShift = true;
    bool ok = gpi.GetToggleExpressionShift() && offsetof(CControlsDef, ToggleExpressionShift) == 0x4c;
    def.ToggleExpressionShift = false;
    ok = ok && !gpi.GetToggleExpressionShift();
    printf(ok ? "00445bb0_TEST PASS\n" : "00445bb0_TEST FAIL\n");
    return ok ? 0 : 1;
}