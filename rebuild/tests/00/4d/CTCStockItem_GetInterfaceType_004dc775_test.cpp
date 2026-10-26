// Test for 0x004DC775 CTCStockItem::GetInterfaceType: the virtual returns the class's interface type 0x54.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCStockItem {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCStockItem::GetInterfaceType() const
{
    return (ETCInterfaceType)0x54;
}
int main()
{
    CTCStockItem object;
    const CTCStockItem* p = &object;
    if ((long)p->GetInterfaceType() != 0x54) { printf("004dc775_TEST FAIL\n"); return 1; }
    printf("004dc775_TEST PASS\n");
    return 0;
}
