// Test for 0x00770FF0 CTCSolidToThing::GetInterfaceType: the virtual returns the class's interface type 0x4D.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCSolidToThing {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCSolidToThing::GetInterfaceType() const
{
    return (ETCInterfaceType)0x4D;
}
int main()
{
    CTCSolidToThing object;
    const CTCSolidToThing* p = &object;
    if ((long)p->GetInterfaceType() != 0x4D) { printf("00770ff0_TEST FAIL\n"); return 1; }
    printf("00770ff0_TEST PASS\n");
    return 0;
}
