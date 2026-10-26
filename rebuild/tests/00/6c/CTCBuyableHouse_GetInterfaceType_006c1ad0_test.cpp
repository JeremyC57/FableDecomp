// Test for 0x006C1AD0 CTCBuyableHouse::GetInterfaceType: the virtual returns the class's interface type 0xD5.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCBuyableHouse {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCBuyableHouse::GetInterfaceType() const
{
    return (ETCInterfaceType)0xD5;
}
int main()
{
    CTCBuyableHouse object;
    const CTCBuyableHouse* p = &object;
    if ((long)p->GetInterfaceType() != 0xD5) { printf("006c1ad0_TEST FAIL\n"); return 1; }
    printf("006c1ad0_TEST PASS\n");
    return 0;
}
