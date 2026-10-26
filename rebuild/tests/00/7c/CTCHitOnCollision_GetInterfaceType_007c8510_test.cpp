// Test for 0x007C8510 CTCHitOnCollision::GetInterfaceType: the virtual returns the class's interface type 0xC8.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHitOnCollision {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHitOnCollision::GetInterfaceType() const
{
    return (ETCInterfaceType)0xC8;
}
int main()
{
    CTCHitOnCollision object;
    const CTCHitOnCollision* p = &object;
    if ((long)p->GetInterfaceType() != 0xC8) { printf("007c8510_TEST FAIL\n"); return 1; }
    printf("007c8510_TEST PASS\n");
    return 0;
}
