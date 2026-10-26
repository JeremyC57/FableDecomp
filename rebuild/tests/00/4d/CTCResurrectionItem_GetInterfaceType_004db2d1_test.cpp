// Test for 0x004DB2D1 CTCResurrectionItem::GetInterfaceType: the virtual returns the class's interface type 0xE1.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCResurrectionItem {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCResurrectionItem::GetInterfaceType() const
{
    return (ETCInterfaceType)0xE1;
}
int main()
{
    CTCResurrectionItem object;
    const CTCResurrectionItem* p = &object;
    if ((long)p->GetInterfaceType() != 0xE1) { printf("004db2d1_TEST FAIL\n"); return 1; }
    printf("004db2d1_TEST PASS\n");
    return 0;
}
