// Test for 0x004D658B CTCFireRangedWeaponOnLeavingStateGroup::GetInterfaceType: the virtual returns the class's interface type 0x104.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCFireRangedWeaponOnLeavingStateGroup {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCFireRangedWeaponOnLeavingStateGroup::GetInterfaceType() const
{
    return (ETCInterfaceType)0x104;
}
int main()
{
    CTCFireRangedWeaponOnLeavingStateGroup object;
    const CTCFireRangedWeaponOnLeavingStateGroup* p = &object;
    if ((long)p->GetInterfaceType() != 0x104) { printf("004d658b_TEST FAIL\n"); return 1; }
    printf("004d658b_TEST PASS\n");
    return 0;
}
