// Test for 0x007B6C60 CTCCrateStack::GetInterfaceType: the virtual returns the class's interface type 0xB5.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCrateStack {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCrateStack::GetInterfaceType() const
{
    return (ETCInterfaceType)0xB5;
}
int main()
{
    CTCCrateStack object;
    const CTCCrateStack* p = &object;
    if ((long)p->GetInterfaceType() != 0xB5) { printf("007b6c60_TEST FAIL\n"); return 1; }
    printf("007b6c60_TEST PASS\n");
    return 0;
}
