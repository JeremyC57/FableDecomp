// Test for 0x007A6130 CTCForcePushPower::GetInterfaceType: the virtual returns the class's interface type 0x96.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCForcePushPower {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCForcePushPower::GetInterfaceType() const
{
    return (ETCInterfaceType)0x96;
}
int main()
{
    CTCForcePushPower object;
    const CTCForcePushPower* p = &object;
    if ((long)p->GetInterfaceType() != 0x96) { printf("007a6130_TEST FAIL\n"); return 1; }
    printf("007a6130_TEST PASS\n");
    return 0;
}
