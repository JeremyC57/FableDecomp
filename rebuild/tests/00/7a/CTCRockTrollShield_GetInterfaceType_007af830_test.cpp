// Test for 0x007AF830 CTCRockTrollShield::GetInterfaceType: the virtual returns the class's interface type 0xAE.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCRockTrollShield {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCRockTrollShield::GetInterfaceType() const
{
    return (ETCInterfaceType)0xAE;
}
int main()
{
    CTCRockTrollShield object;
    const CTCRockTrollShield* p = &object;
    if ((long)p->GetInterfaceType() != 0xAE) { printf("007af830_TEST FAIL\n"); return 1; }
    printf("007af830_TEST PASS\n");
    return 0;
}
