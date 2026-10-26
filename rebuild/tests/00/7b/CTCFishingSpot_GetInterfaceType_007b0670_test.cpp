// Test for 0x007B0670 CTCFishingSpot::GetInterfaceType: the virtual returns the class's interface type 0xB0.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCFishingSpot {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCFishingSpot::GetInterfaceType() const
{
    return (ETCInterfaceType)0xB0;
}
int main()
{
    CTCFishingSpot object;
    const CTCFishingSpot* p = &object;
    if ((long)p->GetInterfaceType() != 0xB0) { printf("007b0670_TEST FAIL\n"); return 1; }
    printf("007b0670_TEST PASS\n");
    return 0;
}
