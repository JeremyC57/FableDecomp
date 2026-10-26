// Test for 0x0056A630 CTCHeroOnlineScoreboard::GetInterfaceType: the virtual returns the class's interface type 0x10D.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHeroOnlineScoreboard {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHeroOnlineScoreboard::GetInterfaceType() const
{
    return (ETCInterfaceType)0x10D;
}
int main()
{
    CTCHeroOnlineScoreboard object;
    const CTCHeroOnlineScoreboard* p = &object;
    if ((long)p->GetInterfaceType() != 0x10D) { printf("0056a630_TEST FAIL\n"); return 1; }
    printf("0056a630_TEST PASS\n");
    return 0;
}
