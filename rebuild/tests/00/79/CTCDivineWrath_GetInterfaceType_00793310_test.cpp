// Test for 0x00793310 CTCDivineWrath::GetInterfaceType: the virtual returns the class's interface type 0x85.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDivineWrath {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDivineWrath::GetInterfaceType() const
{
    return (ETCInterfaceType)0x85;
}
int main()
{
    CTCDivineWrath object;
    const CTCDivineWrath* p = &object;
    if ((long)p->GetInterfaceType() != 0x85) { printf("00793310_TEST FAIL\n"); return 1; }
    printf("00793310_TEST PASS\n");
    return 0;
}
