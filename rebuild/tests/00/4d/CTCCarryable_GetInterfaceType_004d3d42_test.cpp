// Test for 0x004D3D42 CTCCarryable::GetInterfaceType: the virtual returns the class's interface type 0x47.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCarryable {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCarryable::GetInterfaceType() const
{
    return (ETCInterfaceType)0x47;
}
int main()
{
    CTCCarryable object;
    const CTCCarryable* p = &object;
    if ((long)p->GetInterfaceType() != 0x47) { printf("004d3d42_TEST FAIL\n"); return 1; }
    printf("004d3d42_TEST PASS\n");
    return 0;
}
