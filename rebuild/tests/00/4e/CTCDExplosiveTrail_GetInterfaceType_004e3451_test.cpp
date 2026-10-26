// Test for 0x004E3451 CTCDExplosiveTrail::GetInterfaceType: the virtual returns the class's interface type 0x10F.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDExplosiveTrail {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDExplosiveTrail::GetInterfaceType() const
{
    return (ETCInterfaceType)0x10F;
}
int main()
{
    CTCDExplosiveTrail object;
    const CTCDExplosiveTrail* p = &object;
    if ((long)p->GetInterfaceType() != 0x10F) { printf("004e3451_TEST FAIL\n"); return 1; }
    printf("004e3451_TEST PASS\n");
    return 0;
}
