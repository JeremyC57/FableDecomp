// Test for 0x00780290 CTCHeroGuide::GetInterfaceType: the virtual returns the class's interface type 0x6B.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHeroGuide {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHeroGuide::GetInterfaceType() const
{
    return (ETCInterfaceType)0x6B;
}
int main()
{
    CTCHeroGuide object;
    const CTCHeroGuide* p = &object;
    if ((long)p->GetInterfaceType() != 0x6B) { printf("00780290_TEST FAIL\n"); return 1; }
    printf("00780290_TEST PASS\n");
    return 0;
}
