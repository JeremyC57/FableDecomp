// Test for 0x0078DAC0 CTCBerserk::GetInterfaceType: the virtual returns the class's interface type 0x84.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCBerserk {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCBerserk::GetInterfaceType() const
{
    return (ETCInterfaceType)0x84;
}
int main()
{
    CTCBerserk object;
    const CTCBerserk* p = &object;
    if ((long)p->GetInterfaceType() != 0x84) { printf("0078dac0_TEST FAIL\n"); return 1; }
    printf("0078dac0_TEST PASS\n");
    return 0;
}
