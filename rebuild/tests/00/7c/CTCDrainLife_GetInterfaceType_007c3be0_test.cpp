// Test for 0x007C3BE0 CTCDrainLife::GetInterfaceType: the virtual returns the class's interface type 0x97.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDrainLife {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDrainLife::GetInterfaceType() const
{
    return (ETCInterfaceType)0x97;
}
int main()
{
    CTCDrainLife object;
    const CTCDrainLife* p = &object;
    if ((long)p->GetInterfaceType() != 0x97) { printf("007c3be0_TEST FAIL\n"); return 1; }
    printf("007c3be0_TEST PASS\n");
    return 0;
}
