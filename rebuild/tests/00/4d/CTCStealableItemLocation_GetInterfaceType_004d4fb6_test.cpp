// Test for 0x004D4FB6 CTCStealableItemLocation::GetInterfaceType: the virtual returns the class's interface type 0xB2.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCStealableItemLocation {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCStealableItemLocation::GetInterfaceType() const
{
    return (ETCInterfaceType)0xB2;
}
int main()
{
    CTCStealableItemLocation object;
    const CTCStealableItemLocation* p = &object;
    if ((long)p->GetInterfaceType() != 0xB2) { printf("004d4fb6_TEST FAIL\n"); return 1; }
    printf("004d4fb6_TEST PASS\n");
    return 0;
}
