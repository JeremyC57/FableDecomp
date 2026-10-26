// Test for 0x006423C0 CTCLight::GetInterfaceType: the virtual returns the class's interface type 0x27.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCLight {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCLight::GetInterfaceType() const
{
    return (ETCInterfaceType)0x27;
}
int main()
{
    CTCLight object;
    const CTCLight* p = &object;
    if ((long)p->GetInterfaceType() != 0x27) { printf("006423c0_TEST FAIL\n"); return 1; }
    printf("006423c0_TEST PASS\n");
    return 0;
}
