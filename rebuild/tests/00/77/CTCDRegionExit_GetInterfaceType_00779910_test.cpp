// Test for 0x00779910 CTCDRegionExit::GetInterfaceType: the virtual returns the class's interface type 0x64.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDRegionExit {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDRegionExit::GetInterfaceType() const
{
    return (ETCInterfaceType)0x64;
}
int main()
{
    CTCDRegionExit object;
    const CTCDRegionExit* p = &object;
    if ((long)p->GetInterfaceType() != 0x64) { printf("00779910_TEST FAIL\n"); return 1; }
    printf("00779910_TEST PASS\n");
    return 0;
}
