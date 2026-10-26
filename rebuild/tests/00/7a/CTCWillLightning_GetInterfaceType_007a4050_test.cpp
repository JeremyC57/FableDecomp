// Test for 0x007A4050 CTCWillLightning::GetInterfaceType: the virtual returns the class's interface type 0x94.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCWillLightning {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCWillLightning::GetInterfaceType() const
{
    return (ETCInterfaceType)0x94;
}
int main()
{
    CTCWillLightning object;
    const CTCWillLightning* p = &object;
    if ((long)p->GetInterfaceType() != 0x94) { printf("007a4050_TEST FAIL\n"); return 1; }
    printf("007a4050_TEST PASS\n");
    return 0;
}
