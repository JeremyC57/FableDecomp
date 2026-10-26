// Test for 0x004D8C55 CTCGossip::GetInterfaceType: the virtual returns the class's interface type 0xC1.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCGossip {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCGossip::GetInterfaceType() const
{
    return (ETCInterfaceType)0xC1;
}
int main()
{
    CTCGossip object;
    const CTCGossip* p = &object;
    if ((long)p->GetInterfaceType() != 0xC1) { printf("004d8c55_TEST FAIL\n"); return 1; }
    printf("004d8c55_TEST PASS\n");
    return 0;
}
