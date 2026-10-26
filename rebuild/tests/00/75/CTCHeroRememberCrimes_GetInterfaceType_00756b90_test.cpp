// Test for 0x00756B90 CTCHeroRememberCrimes::GetInterfaceType: the virtual returns the class's interface type 0xE0.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHeroRememberCrimes {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHeroRememberCrimes::GetInterfaceType() const
{
    return (ETCInterfaceType)0xE0;
}
int main()
{
    CTCHeroRememberCrimes object;
    const CTCHeroRememberCrimes* p = &object;
    if ((long)p->GetInterfaceType() != 0xE0) { printf("00756b90_TEST FAIL\n"); return 1; }
    printf("00756b90_TEST PASS\n");
    return 0;
}
