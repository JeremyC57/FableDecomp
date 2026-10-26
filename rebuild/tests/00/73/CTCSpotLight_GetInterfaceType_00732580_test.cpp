// Test for 0x00732580 CTCSpotLight::GetInterfaceType: the virtual returns the class's interface type 0xE6.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCSpotLight {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCSpotLight::GetInterfaceType() const
{
    return (ETCInterfaceType)0xE6;
}
int main()
{
    CTCSpotLight object;
    const CTCSpotLight* p = &object;
    if ((long)p->GetInterfaceType() != 0xE6) { printf("00732580_TEST FAIL\n"); return 1; }
    printf("00732580_TEST PASS\n");
    return 0;
}
