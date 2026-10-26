// Test for 0x0077CFC0 CTCHighlightItem::GetInterfaceType: the virtual returns the class's interface type 0x5F.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHighlightItem {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHighlightItem::GetInterfaceType() const
{
    return (ETCInterfaceType)0x5F;
}
int main()
{
    CTCHighlightItem object;
    const CTCHighlightItem* p = &object;
    if ((long)p->GetInterfaceType() != 0x5F) { printf("0077cfc0_TEST FAIL\n"); return 1; }
    printf("0077cfc0_TEST PASS\n");
    return 0;
}
