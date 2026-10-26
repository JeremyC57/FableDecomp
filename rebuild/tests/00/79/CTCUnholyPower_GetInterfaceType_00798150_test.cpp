// Test for 0x00798150 CTCUnholyPower::GetInterfaceType: the virtual returns the class's interface type 0x86.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCUnholyPower {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCUnholyPower::GetInterfaceType() const
{
    return (ETCInterfaceType)0x86;
}
int main()
{
    CTCUnholyPower object;
    const CTCUnholyPower* p = &object;
    if ((long)p->GetInterfaceType() != 0x86) { printf("00798150_TEST FAIL\n"); return 1; }
    printf("00798150_TEST PASS\n");
    return 0;
}
