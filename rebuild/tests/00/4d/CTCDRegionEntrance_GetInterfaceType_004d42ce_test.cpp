// Test for 0x004D42CE CTCDRegionEntrance::GetInterfaceType: the virtual returns the class's interface type 0x63.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDRegionEntrance {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDRegionEntrance::GetInterfaceType() const
{
    return (ETCInterfaceType)0x63;
}
int main()
{
    CTCDRegionEntrance object;
    const CTCDRegionEntrance* p = &object;
    if ((long)p->GetInterfaceType() != 0x63) { printf("004d42ce_TEST FAIL\n"); return 1; }
    printf("004d42ce_TEST PASS\n");
    return 0;
}
