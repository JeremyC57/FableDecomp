// Test for 0x00784110 CTCBoastingCrowdControl::GetInterfaceType: the virtual returns the class's interface type 0x73.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCBoastingCrowdControl {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCBoastingCrowdControl::GetInterfaceType() const
{
    return (ETCInterfaceType)0x73;
}
int main()
{
    CTCBoastingCrowdControl object;
    const CTCBoastingCrowdControl* p = &object;
    if ((long)p->GetInterfaceType() != 0x73) { printf("00784110_TEST FAIL\n"); return 1; }
    printf("00784110_TEST PASS\n");
    return 0;
}
