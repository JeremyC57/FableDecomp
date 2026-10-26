// Test for 0x0072A3E0 CTCActionUseScriptedHook::GetInterfaceType: the virtual returns the class's interface type 0xC2.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCActionUseScriptedHook {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCActionUseScriptedHook::GetInterfaceType() const
{
    return (ETCInterfaceType)0xC2;
}
int main()
{
    CTCActionUseScriptedHook object;
    const CTCActionUseScriptedHook* p = &object;
    if ((long)p->GetInterfaceType() != 0xC2) { printf("0072a3e0_TEST FAIL\n"); return 1; }
    printf("0072a3e0_TEST PASS\n");
    return 0;
}
