// Test for 0x00702BC0 CTCDayOrNightOnlySupport::GetInterfaceType: the virtual returns the class's interface type 0x7C.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDayOrNightOnlySupport {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDayOrNightOnlySupport::GetInterfaceType() const
{
    return (ETCInterfaceType)0x7C;
}
int main()
{
    CTCDayOrNightOnlySupport object;
    const CTCDayOrNightOnlySupport* p = &object;
    if ((long)p->GetInterfaceType() != 0x7C) { printf("00702bc0_TEST FAIL\n"); return 1; }
    printf("00702bc0_TEST PASS\n");
    return 0;
}
