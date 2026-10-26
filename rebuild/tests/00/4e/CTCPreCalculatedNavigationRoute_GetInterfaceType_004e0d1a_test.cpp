// Test for 0x004E0D1A CTCPreCalculatedNavigationRoute::GetInterfaceType: the virtual returns the class's interface type 0xA6.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCPreCalculatedNavigationRoute {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCPreCalculatedNavigationRoute::GetInterfaceType() const
{
    return (ETCInterfaceType)0xA6;
}
int main()
{
    CTCPreCalculatedNavigationRoute object;
    const CTCPreCalculatedNavigationRoute* p = &object;
    if ((long)p->GetInterfaceType() != 0xA6) { printf("004e0d1a_TEST FAIL\n"); return 1; }
    printf("004e0d1a_TEST PASS\n");
    return 0;
}
