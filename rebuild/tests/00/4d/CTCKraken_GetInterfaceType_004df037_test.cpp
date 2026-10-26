// Test for 0x004DF037 CTCKraken::GetInterfaceType: the virtual returns the class's interface type 0xE2.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCKraken {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCKraken::GetInterfaceType() const
{
    return (ETCInterfaceType)0xE2;
}
int main()
{
    CTCKraken object;
    const CTCKraken* p = &object;
    if ((long)p->GetInterfaceType() != 0xE2) { printf("004df037_TEST FAIL\n"); return 1; }
    printf("004df037_TEST PASS\n");
    return 0;
}
