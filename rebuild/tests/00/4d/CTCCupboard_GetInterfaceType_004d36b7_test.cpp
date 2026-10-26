// Test for 0x004D36B7 CTCCupboard::GetInterfaceType: the virtual returns the class's interface type 0x38.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCupboard {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCupboard::GetInterfaceType() const
{
    return (ETCInterfaceType)0x38;
}
int main()
{
    CTCCupboard object;
    const CTCCupboard* p = &object;
    if ((long)p->GetInterfaceType() != 0x38) { printf("004d36b7_TEST FAIL\n"); return 1; }
    printf("004d36b7_TEST PASS\n");
    return 0;
}
