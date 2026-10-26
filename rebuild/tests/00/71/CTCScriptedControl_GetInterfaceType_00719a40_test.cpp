// Test for 0x00719A40 CTCScriptedControl::GetInterfaceType: the virtual returns the class's interface type 0x1F.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCScriptedControl {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCScriptedControl::GetInterfaceType() const
{
    return (ETCInterfaceType)0x1F;
}
int main()
{
    CTCScriptedControl object;
    const CTCScriptedControl* p = &object;
    if ((long)p->GetInterfaceType() != 0x1F) { printf("00719a40_TEST FAIL\n"); return 1; }
    printf("00719a40_TEST PASS\n");
    return 0;
}
