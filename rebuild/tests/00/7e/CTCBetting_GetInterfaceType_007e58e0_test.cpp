// Test for 0x007E58E0 CTCBetting::GetInterfaceType: the virtual returns the class's interface type 0x102.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCBetting {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCBetting::GetInterfaceType() const
{
    return (ETCInterfaceType)0x102;
}
int main()
{
    CTCBetting object;
    const CTCBetting* p = &object;
    if ((long)p->GetInterfaceType() != 0x102) { printf("007e58e0_TEST FAIL\n"); return 1; }
    printf("007e58e0_TEST PASS\n");
    return 0;
}
