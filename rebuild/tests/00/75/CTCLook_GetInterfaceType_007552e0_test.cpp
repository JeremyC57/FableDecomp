// Test for 0x007552E0 CTCLook::GetInterfaceType: the virtual returns the class's interface type 0x43.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCLook {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCLook::GetInterfaceType() const
{
    return (ETCInterfaceType)0x43;
}
int main()
{
    CTCLook object;
    const CTCLook* p = &object;
    if ((long)p->GetInterfaceType() != 0x43) { printf("007552e0_TEST FAIL\n"); return 1; }
    printf("007552e0_TEST PASS\n");
    return 0;
}
