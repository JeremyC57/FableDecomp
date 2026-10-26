// Test for 0x00770920 CTCWeaponTrail::GetInterfaceType: the virtual returns the class's interface type 0x4C.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCWeaponTrail {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCWeaponTrail::GetInterfaceType() const
{
    return (ETCInterfaceType)0x4C;
}
int main()
{
    CTCWeaponTrail object;
    const CTCWeaponTrail* p = &object;
    if ((long)p->GetInterfaceType() != 0x4C) { printf("00770920_TEST FAIL\n"); return 1; }
    printf("00770920_TEST PASS\n");
    return 0;
}
