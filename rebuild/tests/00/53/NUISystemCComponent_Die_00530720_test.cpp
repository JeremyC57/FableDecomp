// behaviour test for 0x00530720 NUISystem::CComponent::Die
#include <stdio.h>
static int g_order[4]; static int g_n; static unsigned long g_state = 99;
namespace NUISystem {
class CComponent {
public:
    virtual ~CComponent() {}
    virtual void _v01() {}
    virtual void _v02() {}
    virtual void _v03() {}
    virtual void _v04() {}
    virtual void _v05() {}
    virtual void _v06() {}
    virtual void _v07() {}
    virtual void _v08() {}
    virtual void _v09() {}
    virtual void _v10() {}
    virtual void _v11() {}
    virtual void _v12() {}
    virtual void _v13() {}
    virtual void _v14() {}
    virtual void _v15() {}
    virtual void _v16() {}
    virtual void _v17() {}
    virtual void _v18() {}
    virtual void _v19() {}
    virtual void _v20() {}
    virtual void _v21() {}
    virtual void _v22() {}
    virtual void _v23() {}
    virtual void _v24() {}
    virtual void _v25() {}
    virtual void _v26() {}
    virtual void _v27() {}
    virtual void _v28() {}
    virtual void _v29() {}
    virtual void _v30() {}
    virtual void _v31() {}
    virtual void _v32() {}
    virtual void _v33() {}
    virtual void _v34() {}
    virtual void _v35() {}
    virtual void _v36() {}
    virtual void _v37() {}
    virtual void _v38() {}
    virtual void _v39() {}
    virtual void _v40() {}
    virtual void _v41() {}
    virtual void _v42() {}
    virtual void _v43() {}
    virtual void _v44() {}
    virtual void _v45() {}
    virtual void _v46() {}
    virtual void _v47() {}
    virtual void ChangeState(unsigned long state) { g_state = state; g_order[g_n++] = 1; }
    virtual void Die();
protected:
    void RemoveObserverRecursive() { g_order[g_n++] = 2; }
};
void CComponent::Die()
{
    ChangeState(2);
    RemoveObserverRecursive();
}
}
int main()
{
    NUISystem::CComponent c; c.Die();
    int ok = g_state == 2 && g_n == 2 && g_order[0] == 1 && g_order[1] == 2;
    printf(ok ? "00530720_TEST PASS\n" : "00530720_TEST FAIL\n");
    return ok ? 0 : 1;
}