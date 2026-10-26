// Test for 0x00619F00 CTCInventoryMap::GetInterfaceType: the virtual returns the class's interface type 0x1B.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCInventoryMap {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCInventoryMap::GetInterfaceType() const
{
    return (ETCInterfaceType)0x1B;
}
int main()
{
    CTCInventoryMap object;
    const CTCInventoryMap* p = &object;
    if ((long)p->GetInterfaceType() != 0x1B) { printf("00619f00_TEST FAIL\n"); return 1; }
    printf("00619f00_TEST PASS\n");
    return 0;
}
