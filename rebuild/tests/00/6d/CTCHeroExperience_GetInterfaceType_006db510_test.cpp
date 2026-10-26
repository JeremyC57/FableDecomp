// Test for 0x006DB510 CTCHeroExperience::GetInterfaceType: the virtual returns the class's interface type 0x68.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHeroExperience {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHeroExperience::GetInterfaceType() const
{
    return (ETCInterfaceType)0x68;
}
int main()
{
    CTCHeroExperience object;
    const CTCHeroExperience* p = &object;
    if ((long)p->GetInterfaceType() != 0x68) { printf("006db510_TEST FAIL\n"); return 1; }
    printf("006db510_TEST PASS\n");
    return 0;
}
