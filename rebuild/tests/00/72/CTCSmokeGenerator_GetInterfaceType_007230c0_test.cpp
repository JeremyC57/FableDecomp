// Test for 0x007230C0 CTCSmokeGenerator::GetInterfaceType: the virtual returns the class's interface type 0x6.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCSmokeGenerator {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCSmokeGenerator::GetInterfaceType() const
{
    return (ETCInterfaceType)0x6;
}
int main()
{
    CTCSmokeGenerator object;
    const CTCSmokeGenerator* p = &object;
    if ((long)p->GetInterfaceType() != 0x6) { printf("007230c0_TEST FAIL\n"); return 1; }
    printf("007230c0_TEST PASS\n");
    return 0;
}
