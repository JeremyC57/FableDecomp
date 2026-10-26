// Test for 0x0079BCD0 CTCSummonSpell::GetInterfaceType: the virtual returns the class's interface type 0x91.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCSummonSpell {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCSummonSpell::GetInterfaceType() const
{
    return (ETCInterfaceType)0x91;
}
int main()
{
    CTCSummonSpell object;
    const CTCSummonSpell* p = &object;
    if ((long)p->GetInterfaceType() != 0x91) { printf("0079bcd0_TEST FAIL\n"); return 1; }
    printf("0079bcd0_TEST PASS\n");
    return 0;
}
