// Test for 0x004E07C2 CTCDHeroSuit::GetInterfaceType: the virtual returns the class's interface type 0x5D.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDHeroSuit {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDHeroSuit::GetInterfaceType() const
{
    return (ETCInterfaceType)0x5D;
}
int main()
{
    CTCDHeroSuit object;
    const CTCDHeroSuit* p = &object;
    if ((long)p->GetInterfaceType() != 0x5D) { printf("004e07c2_TEST FAIL\n"); return 1; }
    printf("004e07c2_TEST PASS\n");
    return 0;
}
