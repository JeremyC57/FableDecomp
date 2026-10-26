// Test for 0x004D4F0B CTCCombatLeader::GetInterfaceType: the virtual returns the class's interface type 0xAF.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCombatLeader {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCombatLeader::GetInterfaceType() const
{
    return (ETCInterfaceType)0xAF;
}
int main()
{
    CTCCombatLeader object;
    const CTCCombatLeader* p = &object;
    if ((long)p->GetInterfaceType() != 0xAF) { printf("004d4f0b_TEST FAIL\n"); return 1; }
    printf("004d4f0b_TEST PASS\n");
    return 0;
}
