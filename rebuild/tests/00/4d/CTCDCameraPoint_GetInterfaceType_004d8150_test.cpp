// Test for 0x004D8150 CTCDCameraPoint::GetInterfaceType: the virtual returns the class's interface type 0x3E.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDCameraPoint {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDCameraPoint::GetInterfaceType() const
{
    return (ETCInterfaceType)0x3E;
}
int main()
{
    CTCDCameraPoint object;
    const CTCDCameraPoint* p = &object;
    if ((long)p->GetInterfaceType() != 0x3E) { printf("004d8150_TEST FAIL\n"); return 1; }
    printf("004d8150_TEST PASS\n");
    return 0;
}
