// Test for 0x0072DCE0 CTCInventoryMagic::GetInterfaceType: the virtual returns the class's interface type 0x15.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCInventoryMagic {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCInventoryMagic::GetInterfaceType() const
{
    return (ETCInterfaceType)0x15;
}
int main()
{
    CTCInventoryMagic object;
    const CTCInventoryMagic* p = &object;
    if ((long)p->GetInterfaceType() != 0x15) { printf("0072dce0_TEST FAIL\n"); return 1; }
    printf("0072dce0_TEST PASS\n");
    return 0;
}
