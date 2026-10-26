// Test for 0x004E7619 CTCHitLocations::GetInterfaceType: the virtual returns the class's interface type 0x7F.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHitLocations {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHitLocations::GetInterfaceType() const
{
    return (ETCInterfaceType)0x7F;
}
int main()
{
    CTCHitLocations object;
    const CTCHitLocations* p = &object;
    if ((long)p->GetInterfaceType() != 0x7F) { printf("004e7619_TEST FAIL\n"); return 1; }
    printf("004e7619_TEST PASS\n");
    return 0;
}
