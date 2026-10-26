// Test for 0x007319C0 CTCJackPillar::GetInterfaceType: the virtual returns the class's interface type 0x25.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCJackPillar {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCJackPillar::GetInterfaceType() const
{
    return (ETCInterfaceType)0x25;
}
int main()
{
    CTCJackPillar object;
    const CTCJackPillar* p = &object;
    if ((long)p->GetInterfaceType() != 0x25) { printf("007319c0_TEST FAIL\n"); return 1; }
    printf("007319c0_TEST PASS\n");
    return 0;
}
