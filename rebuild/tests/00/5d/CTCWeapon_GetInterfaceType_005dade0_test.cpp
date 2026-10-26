// Test for 0x005DADE0 CTCWeapon::GetInterfaceType: the virtual returns the class's interface type 0x48.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCWeapon {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCWeapon::GetInterfaceType() const
{
    return (ETCInterfaceType)0x48;
}
int main()
{
    CTCWeapon object;
    const CTCWeapon* p = &object;
    if ((long)p->GetInterfaceType() != 0x48) { printf("005dade0_TEST FAIL\n"); return 1; }
    printf("005dade0_TEST PASS\n");
    return 0;
}
