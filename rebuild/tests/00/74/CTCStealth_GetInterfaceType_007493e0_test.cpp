// Test for 0x007493E0 CTCStealth::GetInterfaceType: the virtual returns the class's interface type 0x36.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCStealth {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCStealth::GetInterfaceType() const
{
    return (ETCInterfaceType)0x36;
}
int main()
{
    CTCStealth object;
    const CTCStealth* p = &object;
    if ((long)p->GetInterfaceType() != 0x36) { printf("007493e0_TEST FAIL\n"); return 1; }
    printf("007493e0_TEST PASS\n");
    return 0;
}
