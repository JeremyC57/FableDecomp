// Test for 0x004E7D66 CTCInventoryClothing::GetInterfaceType: the virtual returns the class's interface type 0x12.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCInventoryClothing {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCInventoryClothing::GetInterfaceType() const
{
    return (ETCInterfaceType)0x12;
}
int main()
{
    CTCInventoryClothing object;
    const CTCInventoryClothing* p = &object;
    if ((long)p->GetInterfaceType() != 0x12) { printf("004e7d66_TEST FAIL\n"); return 1; }
    printf("004e7d66_TEST PASS\n");
    return 0;
}
