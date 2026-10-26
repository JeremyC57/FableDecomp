// Test for 0x004D5472 NThingShot::CTCShotDefinitionThrow::GetInterfaceType: the virtual returns the class's interface type 0x77.
#include <stdio.h>
enum ETCInterfaceType { };
namespace NThingShot {
class CTCShotDefinitionThrow {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
}
ETCInterfaceType NThingShot::CTCShotDefinitionThrow::GetInterfaceType() const
{
    return (ETCInterfaceType)0x77;
}
int main()
{
    NThingShot::CTCShotDefinitionThrow object;
    const NThingShot::CTCShotDefinitionThrow* p = &object;
    if ((long)p->GetInterfaceType() != 0x77) { printf("004d5472_TEST FAIL\n"); return 1; }
    printf("004d5472_TEST PASS\n");
    return 0;
}
