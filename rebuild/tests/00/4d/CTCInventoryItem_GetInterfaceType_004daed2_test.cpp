// Test for 0x004DAED2 CTCInventoryItem::GetInterfaceType: the virtual returns the class's interface type 0x1A.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCInventoryItem {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCInventoryItem::GetInterfaceType() const
{
    return (ETCInterfaceType)0x1A;
}
int main()
{
    CTCInventoryItem object;
    const CTCInventoryItem* p = &object;
    if ((long)p->GetInterfaceType() != 0x1A) { printf("004daed2_TEST FAIL\n"); return 1; }
    printf("004daed2_TEST PASS\n");
    return 0;
}
