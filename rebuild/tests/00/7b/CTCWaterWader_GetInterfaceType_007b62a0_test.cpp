// Test for 0x007B62A0 CTCWaterWader::GetInterfaceType: the virtual returns the class's interface type 0xB6.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCWaterWader {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCWaterWader::GetInterfaceType() const
{
    return (ETCInterfaceType)0xB6;
}
int main()
{
    CTCWaterWader object;
    const CTCWaterWader* p = &object;
    if ((long)p->GetInterfaceType() != 0xB6) { printf("007b62a0_TEST FAIL\n"); return 1; }
    printf("007b62a0_TEST PASS\n");
    return 0;
}
