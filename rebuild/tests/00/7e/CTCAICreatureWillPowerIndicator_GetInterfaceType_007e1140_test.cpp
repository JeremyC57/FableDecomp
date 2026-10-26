// Test for 0x007E1140 CTCAICreatureWillPowerIndicator::GetInterfaceType: the virtual returns the class's interface type 0xCE.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCAICreatureWillPowerIndicator {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCAICreatureWillPowerIndicator::GetInterfaceType() const
{
    return (ETCInterfaceType)0xCE;
}
int main()
{
    CTCAICreatureWillPowerIndicator object;
    const CTCAICreatureWillPowerIndicator* p = &object;
    if ((long)p->GetInterfaceType() != 0xCE) { printf("007e1140_TEST FAIL\n"); return 1; }
    printf("007e1140_TEST PASS\n");
    return 0;
}
