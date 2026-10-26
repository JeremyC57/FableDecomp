// Test for 0x007DFAA0 CTCWallMountEffects::GetInterfaceType: the virtual returns the class's interface type 0xDB.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCWallMountEffects {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCWallMountEffects::GetInterfaceType() const
{
    return (ETCInterfaceType)0xDB;
}
int main()
{
    CTCWallMountEffects object;
    const CTCWallMountEffects* p = &object;
    if ((long)p->GetInterfaceType() != 0xDB) { printf("007dfaa0_TEST FAIL\n"); return 1; }
    printf("007dfaa0_TEST PASS\n");
    return 0;
}
