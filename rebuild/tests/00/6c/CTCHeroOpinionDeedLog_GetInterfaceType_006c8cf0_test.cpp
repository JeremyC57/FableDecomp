// Test for 0x006C8CF0 CTCHeroOpinionDeedLog::GetInterfaceType: the virtual returns the class's interface type 0x51.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHeroOpinionDeedLog {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHeroOpinionDeedLog::GetInterfaceType() const
{
    return (ETCInterfaceType)0x51;
}
int main()
{
    CTCHeroOpinionDeedLog object;
    const CTCHeroOpinionDeedLog* p = &object;
    if ((long)p->GetInterfaceType() != 0x51) { printf("006c8cf0_TEST FAIL\n"); return 1; }
    printf("006c8cf0_TEST PASS\n");
    return 0;
}
