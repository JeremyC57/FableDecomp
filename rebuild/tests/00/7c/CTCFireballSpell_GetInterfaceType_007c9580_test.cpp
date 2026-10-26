// Test for 0x007C9580 CTCFireballSpell::GetInterfaceType: the virtual returns the class's interface type 0x9D.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCFireballSpell {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCFireballSpell::GetInterfaceType() const
{
    return (ETCInterfaceType)0x9D;
}
int main()
{
    CTCFireballSpell object;
    const CTCFireballSpell* p = &object;
    if ((long)p->GetInterfaceType() != 0x9D) { printf("007c9580_TEST FAIL\n"); return 1; }
    printf("007c9580_TEST PASS\n");
    return 0;
}
