// Test for 0x004D8020 CTCGeneratedCreature::GetInterfaceType: the virtual returns the class's interface type 0x111.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCGeneratedCreature {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCGeneratedCreature::GetInterfaceType() const
{
    return (ETCInterfaceType)0x111;
}
int main()
{
    CTCGeneratedCreature object;
    const CTCGeneratedCreature* p = &object;
    if ((long)p->GetInterfaceType() != 0x111) { printf("004d8020_TEST FAIL\n"); return 1; }
    printf("004d8020_TEST PASS\n");
    return 0;
}
