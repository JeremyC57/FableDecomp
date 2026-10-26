// Test for 0x007C50D0 CTCHealLife::GetInterfaceType: the virtual returns the class's interface type 0x99.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHealLife {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHealLife::GetInterfaceType() const
{
    return (ETCInterfaceType)0x99;
}
int main()
{
    CTCHealLife object;
    const CTCHealLife* p = &object;
    if ((long)p->GetInterfaceType() != 0x99) { printf("007c50d0_TEST FAIL\n"); return 1; }
    printf("007c50d0_TEST PASS\n");
    return 0;
}
