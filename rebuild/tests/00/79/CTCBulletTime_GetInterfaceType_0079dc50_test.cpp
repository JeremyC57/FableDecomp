// Test for 0x0079DC50 CTCBulletTime::GetInterfaceType: the virtual returns the class's interface type 0x92.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCBulletTime {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCBulletTime::GetInterfaceType() const
{
    return (ETCInterfaceType)0x92;
}
int main()
{
    CTCBulletTime object;
    const CTCBulletTime* p = &object;
    if ((long)p->GetInterfaceType() != 0x92) { printf("0079dc50_TEST FAIL\n"); return 1; }
    printf("0079dc50_TEST PASS\n");
    return 0;
}
