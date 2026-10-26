// Test for 0x0078AF40 CTCHeroRangedAttacks::GetInterfaceType: the virtual returns the class's interface type 0x81.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHeroRangedAttacks {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHeroRangedAttacks::GetInterfaceType() const
{
    return (ETCInterfaceType)0x81;
}
int main()
{
    CTCHeroRangedAttacks object;
    const CTCHeroRangedAttacks* p = &object;
    if ((long)p->GetInterfaceType() != 0x81) { printf("0078af40_TEST FAIL\n"); return 1; }
    printf("0078af40_TEST PASS\n");
    return 0;
}
