// Test for 0x007CE170 CTCBurntEffect::GetInterfaceType: the virtual returns the class's interface type 0x89.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCBurntEffect {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCBurntEffect::GetInterfaceType() const
{
    return (ETCInterfaceType)0x89;
}
int main()
{
    CTCBurntEffect object;
    const CTCBurntEffect* p = &object;
    if ((long)p->GetInterfaceType() != 0x89) { printf("007ce170_TEST FAIL\n"); return 1; }
    printf("007ce170_TEST PASS\n");
    return 0;
}
