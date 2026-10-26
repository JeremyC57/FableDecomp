// Test for 0x004DCE6F CTCSoundAtmosphereVillage::GetInterfaceType: the virtual returns the class's interface type 0xF4.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCSoundAtmosphereVillage {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCSoundAtmosphereVillage::GetInterfaceType() const
{
    return (ETCInterfaceType)0xF4;
}
int main()
{
    CTCSoundAtmosphereVillage object;
    const CTCSoundAtmosphereVillage* p = &object;
    if ((long)p->GetInterfaceType() != 0xF4) { printf("004dce6f_TEST FAIL\n"); return 1; }
    printf("004dce6f_TEST PASS\n");
    return 0;
}
