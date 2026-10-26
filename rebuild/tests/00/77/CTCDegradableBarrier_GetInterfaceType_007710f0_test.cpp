// Test for 0x007710F0 CTCDegradableBarrier::GetInterfaceType: the virtual returns the class's interface type 0x67.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDegradableBarrier {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDegradableBarrier::GetInterfaceType() const
{
    return (ETCInterfaceType)0x67;
}
int main()
{
    CTCDegradableBarrier object;
    const CTCDegradableBarrier* p = &object;
    if ((long)p->GetInterfaceType() != 0x67) { printf("007710f0_TEST FAIL\n"); return 1; }
    printf("007710f0_TEST PASS\n");
    return 0;
}
