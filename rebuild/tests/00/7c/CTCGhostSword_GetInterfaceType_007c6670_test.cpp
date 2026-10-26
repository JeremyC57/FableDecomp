// Test for 0x007C6670 CTCGhostSword::GetInterfaceType: the virtual returns the class's interface type 0x9A.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCGhostSword {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCGhostSword::GetInterfaceType() const
{
    return (ETCInterfaceType)0x9A;
}
int main()
{
    CTCGhostSword object;
    const CTCGhostSword* p = &object;
    if ((long)p->GetInterfaceType() != 0x9A) { printf("007c6670_TEST FAIL\n"); return 1; }
    printf("007c6670_TEST PASS\n");
    return 0;
}
