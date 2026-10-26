// Test for 0x007312B0 CTCDoor::GetInterfaceType: the virtual returns the class's interface type 0x26.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDoor {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDoor::GetInterfaceType() const
{
    return (ETCInterfaceType)0x26;
}
int main()
{
    CTCDoor object;
    const CTCDoor* p = &object;
    if ((long)p->GetInterfaceType() != 0x26) { printf("007312b0_TEST FAIL\n"); return 1; }
    printf("007312b0_TEST PASS\n");
    return 0;
}
