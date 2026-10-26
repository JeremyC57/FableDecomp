// Test for 0x00624D60 CTCInventoryQuests::GetInterfaceType: the virtual returns the class's interface type 0x19.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCInventoryQuests {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCInventoryQuests::GetInterfaceType() const
{
    return (ETCInterfaceType)0x19;
}
int main()
{
    CTCInventoryQuests object;
    const CTCInventoryQuests* p = &object;
    if ((long)p->GetInterfaceType() != 0x19) { printf("00624d60_TEST FAIL\n"); return 1; }
    printf("00624d60_TEST PASS\n");
    return 0;
}
