// Test for 0x006D4540 CTCWife::GetInterfaceType: the virtual returns the class's interface type 0xD7.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCWife {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCWife::GetInterfaceType() const
{
    return (ETCInterfaceType)0xD7;
}
int main()
{
    CTCWife object;
    const CTCWife* p = &object;
    if ((long)p->GetInterfaceType() != 0xD7) { printf("006d4540_TEST FAIL\n"); return 1; }
    printf("006d4540_TEST PASS\n");
    return 0;
}
