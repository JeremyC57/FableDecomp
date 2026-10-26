// Test for 0x004D9799 CTCAvoidRegionExit::GetInterfaceType: the virtual returns the class's interface type 0x105.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCAvoidRegionExit {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCAvoidRegionExit::GetInterfaceType() const
{
    return (ETCInterfaceType)0x105;
}
int main()
{
    CTCAvoidRegionExit object;
    const CTCAvoidRegionExit* p = &object;
    if ((long)p->GetInterfaceType() != 0x105) { printf("004d9799_TEST FAIL\n"); return 1; }
    printf("004d9799_TEST PASS\n");
    return 0;
}
