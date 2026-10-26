// Test for 0x007BAD50 CTCTrespassable::GetInterfaceType: the virtual returns the class's interface type 0xBD.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTrespassable {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTrespassable::GetInterfaceType() const
{
    return (ETCInterfaceType)0xBD;
}
int main()
{
    CTCTrespassable object;
    const CTCTrespassable* p = &object;
    if ((long)p->GetInterfaceType() != 0xBD) { printf("007bad50_TEST FAIL\n"); return 1; }
    printf("007bad50_TEST PASS\n");
    return 0;
}
