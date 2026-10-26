// Test for 0x007303F0 CTCDummyVillager::GetInterfaceType: the virtual returns the class's interface type 0xD6.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDummyVillager {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDummyVillager::GetInterfaceType() const
{
    return (ETCInterfaceType)0xD6;
}
int main()
{
    CTCDummyVillager object;
    const CTCDummyVillager* p = &object;
    if ((long)p->GetInterfaceType() != 0xD6) { printf("007303f0_TEST FAIL\n"); return 1; }
    printf("007303f0_TEST PASS\n");
    return 0;
}
