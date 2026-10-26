// Test for 0x004D3F2C CTCDNone::GetInterfaceType: the virtual returns the class's interface type 0x60.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDNone {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDNone::GetInterfaceType() const
{
    return (ETCInterfaceType)0x60;
}
int main()
{
    CTCDNone object;
    const CTCDNone* p = &object;
    if ((long)p->GetInterfaceType() != 0x60) { printf("004d3f2c_TEST FAIL\n"); return 1; }
    printf("004d3f2c_TEST PASS\n");
    return 0;
}
