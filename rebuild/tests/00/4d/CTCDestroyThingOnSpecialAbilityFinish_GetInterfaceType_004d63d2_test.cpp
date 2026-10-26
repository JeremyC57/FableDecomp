// Test for 0x004D63D2 CTCDestroyThingOnSpecialAbilityFinish::GetInterfaceType: the virtual returns the class's interface type 0x109.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDestroyThingOnSpecialAbilityFinish {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDestroyThingOnSpecialAbilityFinish::GetInterfaceType() const
{
    return (ETCInterfaceType)0x109;
}
int main()
{
    CTCDestroyThingOnSpecialAbilityFinish object;
    const CTCDestroyThingOnSpecialAbilityFinish* p = &object;
    if ((long)p->GetInterfaceType() != 0x109) { printf("004d63d2_TEST FAIL\n"); return 1; }
    printf("004d63d2_TEST PASS\n");
    return 0;
}
