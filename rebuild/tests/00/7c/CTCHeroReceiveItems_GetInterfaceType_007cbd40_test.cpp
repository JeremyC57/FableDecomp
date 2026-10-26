// Test for 0x007CBD40 CTCHeroReceiveItems::GetInterfaceType: the virtual returns the class's interface type 0xCB.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHeroReceiveItems {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHeroReceiveItems::GetInterfaceType() const
{
    return (ETCInterfaceType)0xCB;
}
int main()
{
    CTCHeroReceiveItems object;
    const CTCHeroReceiveItems* p = &object;
    if ((long)p->GetInterfaceType() != 0xCB) { printf("007cbd40_TEST FAIL\n"); return 1; }
    printf("007cbd40_TEST PASS\n");
    return 0;
}
