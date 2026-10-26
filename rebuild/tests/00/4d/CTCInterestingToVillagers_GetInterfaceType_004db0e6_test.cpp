// Test for 0x004DB0E6 CTCInterestingToVillagers::GetInterfaceType: the virtual returns the class's interface type 0xB3.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCInterestingToVillagers {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCInterestingToVillagers::GetInterfaceType() const
{
    return (ETCInterfaceType)0xB3;
}
int main()
{
    CTCInterestingToVillagers object;
    const CTCInterestingToVillagers* p = &object;
    if ((long)p->GetInterfaceType() != 0xB3) { printf("004db0e6_TEST FAIL\n"); return 1; }
    printf("004db0e6_TEST PASS\n");
    return 0;
}
