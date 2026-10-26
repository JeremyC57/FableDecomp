// Test for 0x00731A20 CTCJackShield::GetInterfaceType: the virtual returns the class's interface type 0x24.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCJackShield {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCJackShield::GetInterfaceType() const
{
    return (ETCInterfaceType)0x24;
}
int main()
{
    CTCJackShield object;
    const CTCJackShield* p = &object;
    if ((long)p->GetInterfaceType() != 0x24) { printf("00731a20_TEST FAIL\n"); return 1; }
    printf("00731a20_TEST PASS\n");
    return 0;
}
