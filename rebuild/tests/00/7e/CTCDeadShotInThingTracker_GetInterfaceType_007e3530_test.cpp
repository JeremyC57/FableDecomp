// Test for 0x007E3530 CTCDeadShotInThingTracker::GetInterfaceType: the virtual returns the class's interface type 0x76.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDeadShotInThingTracker {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDeadShotInThingTracker::GetInterfaceType() const
{
    return (ETCInterfaceType)0x76;
}
int main()
{
    CTCDeadShotInThingTracker object;
    const CTCDeadShotInThingTracker* p = &object;
    if ((long)p->GetInterfaceType() != 0x76) { printf("007e3530_TEST FAIL\n"); return 1; }
    printf("007e3530_TEST PASS\n");
    return 0;
}
