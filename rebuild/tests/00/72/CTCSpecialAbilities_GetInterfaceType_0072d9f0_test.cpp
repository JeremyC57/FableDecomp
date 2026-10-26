// Test for 0x0072D9F0 CTCSpecialAbilities::GetInterfaceType: the virtual returns the class's interface type 0xA7.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCSpecialAbilities {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCSpecialAbilities::GetInterfaceType() const
{
    return (ETCInterfaceType)0xA7;
}
int main()
{
    CTCSpecialAbilities object;
    const CTCSpecialAbilities* p = &object;
    if ((long)p->GetInterfaceType() != 0xA7) { printf("0072d9f0_TEST FAIL\n"); return 1; }
    printf("0072d9f0_TEST PASS\n");
    return 0;
}
