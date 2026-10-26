// Test for 0x007E3220 CTCCameraCollision::GetInterfaceType: the virtual returns the class's interface type 0xF8.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCameraCollision {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCameraCollision::GetInterfaceType() const
{
    return (ETCInterfaceType)0xF8;
}
int main()
{
    CTCCameraCollision object;
    const CTCCameraCollision* p = &object;
    if ((long)p->GetInterfaceType() != 0xF8) { printf("007e3220_TEST FAIL\n"); return 1; }
    printf("007e3220_TEST PASS\n");
    return 0;
}
