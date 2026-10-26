// Test for 0x004E0368 CTCSingleUserActionPoints::GetInterfaceType: the virtual returns the class's interface type 0x2D.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCSingleUserActionPoints {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCSingleUserActionPoints::GetInterfaceType() const
{
    return (ETCInterfaceType)0x2D;
}
int main()
{
    CTCSingleUserActionPoints object;
    const CTCSingleUserActionPoints* p = &object;
    if ((long)p->GetInterfaceType() != 0x2D) { printf("004e0368_TEST FAIL\n"); return 1; }
    printf("004e0368_TEST PASS\n");
    return 0;
}
