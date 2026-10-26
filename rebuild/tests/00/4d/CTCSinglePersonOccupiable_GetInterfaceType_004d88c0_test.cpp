// Test for 0x004D88C0 CTCSinglePersonOccupiable::GetInterfaceType: the virtual returns the class's interface type 0xAC.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCSinglePersonOccupiable {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCSinglePersonOccupiable::GetInterfaceType() const
{
    return (ETCInterfaceType)0xAC;
}
int main()
{
    CTCSinglePersonOccupiable object;
    const CTCSinglePersonOccupiable* p = &object;
    if ((long)p->GetInterfaceType() != 0xAC) { printf("004d88c0_TEST FAIL\n"); return 1; }
    printf("004d88c0_TEST PASS\n");
    return 0;
}
