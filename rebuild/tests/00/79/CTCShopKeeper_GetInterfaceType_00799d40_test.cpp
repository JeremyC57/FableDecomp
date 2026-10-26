// Test for 0x00799D40 CTCShopKeeper::GetInterfaceType: the virtual returns the class's interface type 0x90.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCShopKeeper {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCShopKeeper::GetInterfaceType() const
{
    return (ETCInterfaceType)0x90;
}
int main()
{
    CTCShopKeeper object;
    const CTCShopKeeper* p = &object;
    if ((long)p->GetInterfaceType() != 0x90) { printf("00799d40_TEST FAIL\n"); return 1; }
    printf("00799d40_TEST PASS\n");
    return 0;
}
