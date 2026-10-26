// Test for 0x004E3144 CTCDExplosion::GetInterfaceType: the virtual returns the class's interface type 0x41.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDExplosion {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDExplosion::GetInterfaceType() const
{
    return (ETCInterfaceType)0x41;
}
int main()
{
    CTCDExplosion object;
    const CTCDExplosion* p = &object;
    if ((long)p->GetInterfaceType() != 0x41) { printf("004e3144_TEST FAIL\n"); return 1; }
    printf("004e3144_TEST PASS\n");
    return 0;
}
