// Test for 0x00778380 CTCShop::GetInterfaceType: the virtual returns the class's interface type 0x53.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCShop {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCShop::GetInterfaceType() const
{
    return (ETCInterfaceType)0x53;
}
int main()
{
    CTCShop object;
    const CTCShop* p = &object;
    if ((long)p->GetInterfaceType() != 0x53) { printf("00778380_TEST FAIL\n"); return 1; }
    printf("00778380_TEST PASS\n");
    return 0;
}
