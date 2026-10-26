// Test for 0x006AF050 CTCRegionFollower::GetInterfaceType: the virtual returns the class's interface type 0x7A.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCRegionFollower {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCRegionFollower::GetInterfaceType() const
{
    return (ETCInterfaceType)0x7A;
}
int main()
{
    CTCRegionFollower object;
    const CTCRegionFollower* p = &object;
    if ((long)p->GetInterfaceType() != 0x7A) { printf("006af050_TEST FAIL\n"); return 1; }
    printf("006af050_TEST PASS\n");
    return 0;
}
