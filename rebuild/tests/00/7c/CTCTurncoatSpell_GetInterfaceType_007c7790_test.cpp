// Test for 0x007C7790 CTCTurncoatSpell::GetInterfaceType: the virtual returns the class's interface type 0x9E.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTurncoatSpell {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTurncoatSpell::GetInterfaceType() const
{
    return (ETCInterfaceType)0x9E;
}
int main()
{
    CTCTurncoatSpell object;
    const CTCTurncoatSpell* p = &object;
    if ((long)p->GetInterfaceType() != 0x9E) { printf("007c7790_TEST FAIL\n"); return 1; }
    printf("007c7790_TEST PASS\n");
    return 0;
}
