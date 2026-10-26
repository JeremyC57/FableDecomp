// Test for 0x007B4FE0 CTCGuard::GetInterfaceType: the virtual returns the class's interface type 0xB4.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCGuard {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCGuard::GetInterfaceType() const
{
    return (ETCInterfaceType)0xB4;
}
int main()
{
    CTCGuard object;
    const CTCGuard* p = &object;
    if ((long)p->GetInterfaceType() != 0xB4) { printf("007b4fe0_TEST FAIL\n"); return 1; }
    printf("007b4fe0_TEST PASS\n");
    return 0;
}
