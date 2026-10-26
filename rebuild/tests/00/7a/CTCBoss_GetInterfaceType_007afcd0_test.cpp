// Test for 0x007AFCD0 CTCBoss::GetInterfaceType: the virtual returns the class's interface type 0xAD.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCBoss {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCBoss::GetInterfaceType() const
{
    return (ETCInterfaceType)0xAD;
}
int main()
{
    CTCBoss object;
    const CTCBoss* p = &object;
    if ((long)p->GetInterfaceType() != 0xAD) { printf("007afcd0_TEST FAIL\n"); return 1; }
    printf("007afcd0_TEST PASS\n");
    return 0;
}
