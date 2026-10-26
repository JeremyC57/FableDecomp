// Test for 0x0079F4D0 CTCPhysicalShieldSpell::GetInterfaceType: the virtual returns the class's interface type 0x93.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCPhysicalShieldSpell {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCPhysicalShieldSpell::GetInterfaceType() const
{
    return (ETCInterfaceType)0x93;
}
int main()
{
    CTCPhysicalShieldSpell object;
    const CTCPhysicalShieldSpell* p = &object;
    if ((long)p->GetInterfaceType() != 0x93) { printf("0079f4d0_TEST FAIL\n"); return 1; }
    printf("0079f4d0_TEST PASS\n");
    return 0;
}
