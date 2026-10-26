// Test for 0x007E26E0 CTCElectrocuteOnTouch::GetInterfaceType: the virtual returns the class's interface type 0xEE.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCElectrocuteOnTouch {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCElectrocuteOnTouch::GetInterfaceType() const
{
    return (ETCInterfaceType)0xEE;
}
int main()
{
    CTCElectrocuteOnTouch object;
    const CTCElectrocuteOnTouch* p = &object;
    if ((long)p->GetInterfaceType() != 0xEE) { printf("007e26e0_TEST FAIL\n"); return 1; }
    printf("007e26e0_TEST PASS\n");
    return 0;
}
