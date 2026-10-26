// Test for 0x007ACDA0 CTCAIScratchpad::GetInterfaceType: the virtual returns the class's interface type 0xAA.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCAIScratchpad {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCAIScratchpad::GetInterfaceType() const
{
    return (ETCInterfaceType)0xAA;
}
int main()
{
    CTCAIScratchpad object;
    const CTCAIScratchpad* p = &object;
    if ((long)p->GetInterfaceType() != 0xAA) { printf("007acda0_TEST FAIL\n"); return 1; }
    printf("007acda0_TEST PASS\n");
    return 0;
}
