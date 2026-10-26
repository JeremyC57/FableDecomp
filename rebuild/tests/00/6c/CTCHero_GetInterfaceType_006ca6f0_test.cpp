// Test for 0x006CA6F0 CTCHero::GetInterfaceType: the virtual returns the class's interface type 0x29.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHero {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHero::GetInterfaceType() const
{
    return (ETCInterfaceType)0x29;
}
int main()
{
    CTCHero object;
    const CTCHero* p = &object;
    if ((long)p->GetInterfaceType() != 0x29) { printf("006ca6f0_TEST FAIL\n"); return 1; }
    printf("006ca6f0_TEST PASS\n");
    return 0;
}
