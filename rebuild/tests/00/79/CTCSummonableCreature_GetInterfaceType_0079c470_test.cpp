// Test for 0x0079C470 CTCSummonableCreature::GetInterfaceType: the virtual returns the class's interface type 0xA0.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCSummonableCreature {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCSummonableCreature::GetInterfaceType() const
{
    return (ETCInterfaceType)0xA0;
}
int main()
{
    CTCSummonableCreature object;
    const CTCSummonableCreature* p = &object;
    if ((long)p->GetInterfaceType() != 0xA0) { printf("0079c470_TEST FAIL\n"); return 1; }
    printf("0079c470_TEST PASS\n");
    return 0;
}
