// Test for 0x007CD7D0 CTCMultiStrike::GetInterfaceType: the virtual returns the class's interface type 0x87.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCMultiStrike {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCMultiStrike::GetInterfaceType() const
{
    return (ETCInterfaceType)0x87;
}
int main()
{
    CTCMultiStrike object;
    const CTCMultiStrike* p = &object;
    if ((long)p->GetInterfaceType() != 0x87) { printf("007cd7d0_TEST FAIL\n"); return 1; }
    printf("007cd7d0_TEST PASS\n");
    return 0;
}
