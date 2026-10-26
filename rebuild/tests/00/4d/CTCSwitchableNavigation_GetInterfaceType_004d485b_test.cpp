// Test for 0x004D485B CTCSwitchableNavigation::GetInterfaceType: the virtual returns the class's interface type 0x79.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCSwitchableNavigation {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCSwitchableNavigation::GetInterfaceType() const
{
    return (ETCInterfaceType)0x79;
}
int main()
{
    CTCSwitchableNavigation object;
    const CTCSwitchableNavigation* p = &object;
    if ((long)p->GetInterfaceType() != 0x79) { printf("004d485b_TEST FAIL\n"); return 1; }
    printf("004d485b_TEST PASS\n");
    return 0;
}
