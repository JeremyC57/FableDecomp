// Test for 0x007E2580 CTCDiggingSpot::GetInterfaceType: the virtual returns the class's interface type 0xEF.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDiggingSpot {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDiggingSpot::GetInterfaceType() const
{
    return (ETCInterfaceType)0xEF;
}
int main()
{
    CTCDiggingSpot object;
    const CTCDiggingSpot* p = &object;
    if ((long)p->GetInterfaceType() != 0xEF) { printf("007e2580_TEST FAIL\n"); return 1; }
    printf("007e2580_TEST PASS\n");
    return 0;
}
