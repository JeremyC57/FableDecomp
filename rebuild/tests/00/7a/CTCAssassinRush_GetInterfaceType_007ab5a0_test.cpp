// Test for 0x007AB5A0 CTCAssassinRush::GetInterfaceType: the virtual returns the class's interface type 0xA9.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCAssassinRush {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCAssassinRush::GetInterfaceType() const
{
    return (ETCInterfaceType)0xA9;
}
int main()
{
    CTCAssassinRush object;
    const CTCAssassinRush* p = &object;
    if ((long)p->GetInterfaceType() != 0xA9) { printf("007ab5a0_TEST FAIL\n"); return 1; }
    printf("007ab5a0_TEST PASS\n");
    return 0;
}
