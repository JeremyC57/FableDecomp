#include <stdio.h>
#include <stddef.h>
class CDefPointeeBase;
class CGamePlayerInterface { public: unsigned char _pad[0x7a0]; CDefPointeeBase* PControlsDef_Object; };
class CControlsDef { public:
    unsigned char _base[0x3c]; void* Controls[3];
    bool ToggleZTarget, ToggleSpells, ToggleSneak, ToggleExpressionMenu, ToggleExpressionShift, FlourishNeedsAttackButtonHeld;
};
struct CGamePlayerInterface_Methods : CGamePlayerInterface {
    const CControlsDef* GetControlsDef() const { return (const CControlsDef*)PControlsDef_Object; }
    bool GetFlourishNeedsAttackButtonHeld() const;
};

bool CGamePlayerInterface_Methods::GetFlourishNeedsAttackButtonHeld() const
{
    return GetControlsDef()->FlourishNeedsAttackButtonHeld;
}

int main()
{
    static CGamePlayerInterface_Methods gpi; static CControlsDef def;
    gpi.PControlsDef_Object = (CDefPointeeBase*)&def;
    def.FlourishNeedsAttackButtonHeld = true;
    bool ok = gpi.GetFlourishNeedsAttackButtonHeld() && offsetof(CControlsDef, FlourishNeedsAttackButtonHeld) == 0x4d;
    def.FlourishNeedsAttackButtonHeld = false;
    ok = ok && !gpi.GetFlourishNeedsAttackButtonHeld();
    printf(ok ? "00445bc0_TEST PASS\n" : "00445bc0_TEST FAIL\n");
    return ok ? 0 : 1;
}