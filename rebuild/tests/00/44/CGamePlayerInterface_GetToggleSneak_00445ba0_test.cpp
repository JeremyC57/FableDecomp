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
    bool GetToggleSneak() const;
};

bool CGamePlayerInterface_Methods::GetToggleSneak() const
{
    return GetControlsDef()->ToggleSneak;
}

int main()
{
    static CGamePlayerInterface_Methods gpi; static CControlsDef def;
    gpi.PControlsDef_Object = (CDefPointeeBase*)&def;
    def.ToggleSneak = true;
    bool ok = gpi.GetToggleSneak() && offsetof(CControlsDef, ToggleSneak) == 0x4a;
    def.ToggleSneak = false;
    ok = ok && !gpi.GetToggleSneak();
    printf(ok ? "00445ba0_TEST PASS\n" : "00445ba0_TEST FAIL\n");
    return ok ? 0 : 1;
}