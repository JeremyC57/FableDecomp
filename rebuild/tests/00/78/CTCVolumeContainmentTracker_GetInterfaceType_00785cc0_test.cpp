// Test for 0x00785CC0 CTCVolumeContainmentTracker::GetInterfaceType: the virtual returns the class's interface type 0x75.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCVolumeContainmentTracker {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCVolumeContainmentTracker::GetInterfaceType() const
{
    return (ETCInterfaceType)0x75;
}
int main()
{
    CTCVolumeContainmentTracker object;
    const CTCVolumeContainmentTracker* p = &object;
    if ((long)p->GetInterfaceType() != 0x75) { printf("00785cc0_TEST FAIL\n"); return 1; }
    printf("00785cc0_TEST PASS\n");
    return 0;
}
