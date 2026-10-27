// Behaviour test for 0x008E9B60 NCreatureMode::CModeStrafe::OnDeactivate(): ActiveWeapon = -1, then the base
// CModeBase::OnDeactivate (Active = false), then the strafe-animation reset.
#include <cstdio>

static int g_order[4];
static int g_n = 0;

class ModeBase {
public:
    bool Active;
    virtual ~ModeBase() { }
    virtual void OnDeactivate() { Active = false; g_order[g_n++] = 1; }
};
class ModeStrafe : public ModeBase {
public:
    long ActiveWeapon; long weaponSeenByBase;
    void ResetStrafeAnims() { g_order[g_n++] = 2; }
    virtual void OnDeactivate()
    {
        ActiveWeapon = -1;
        weaponSeenByBase = ActiveWeapon;
        ModeBase::OnDeactivate();
        ResetStrafeAnims();
    }
};

int main()
{
    ModeStrafe m; m.Active = true; m.ActiveWeapon = 3; m.weaponSeenByBase = 0;
    ModeBase& b = m;
    b.OnDeactivate();
    if (m.ActiveWeapon != -1 || m.Active || m.weaponSeenByBase != -1) { std::printf("008e9b60_TEST FAIL\n"); return 1; }
    if (g_n != 2 || g_order[0] != 1 || g_order[1] != 2) { std::printf("008e9b60_TEST FAIL\n"); return 1; }
    std::printf("008e9b60_TEST PASS\n");
    return 0;
}