// Test for 0x00739CE0 CTCCombat::GetInterfaceType: the virtual returns the class's interface type 0x30.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCombat {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCombat::GetInterfaceType() const
{
    return (ETCInterfaceType)0x30;
}
int main()
{
    CTCCombat object;
    const CTCCombat* p = &object;
    if ((long)p->GetInterfaceType() != 0x30) { printf("00739ce0_TEST FAIL\n"); return 1; }
    printf("00739ce0_TEST PASS\n");
    return 0;
}
