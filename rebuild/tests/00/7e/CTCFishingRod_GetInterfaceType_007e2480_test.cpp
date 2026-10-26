// Test for 0x007E2480 CTCFishingRod::GetInterfaceType: the virtual returns the class's interface type 0xEA.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCFishingRod {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCFishingRod::GetInterfaceType() const
{
    return (ETCInterfaceType)0xEA;
}
int main()
{
    CTCFishingRod object;
    const CTCFishingRod* p = &object;
    if ((long)p->GetInterfaceType() != 0xEA) { printf("007e2480_TEST FAIL\n"); return 1; }
    printf("007e2480_TEST PASS\n");
    return 0;
}
