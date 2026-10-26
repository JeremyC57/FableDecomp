// Test for 0x00732B90 CTCClock::GetInterfaceType: the virtual returns the class's interface type 0x28.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCClock {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCClock::GetInterfaceType() const
{
    return (ETCInterfaceType)0x28;
}
int main()
{
    CTCClock object;
    const CTCClock* p = &object;
    if ((long)p->GetInterfaceType() != 0x28) { printf("00732b90_TEST FAIL\n"); return 1; }
    printf("00732b90_TEST PASS\n");
    return 0;
}
