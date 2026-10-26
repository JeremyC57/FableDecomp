// Test for 0x0077AB60 CTCDExperienceOrb::GetInterfaceType: the virtual returns the class's interface type 0x69.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDExperienceOrb {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDExperienceOrb::GetInterfaceType() const
{
    return (ETCInterfaceType)0x69;
}
int main()
{
    CTCDExperienceOrb object;
    const CTCDExperienceOrb* p = &object;
    if ((long)p->GetInterfaceType() != 0x69) { printf("0077ab60_TEST FAIL\n"); return 1; }
    printf("0077ab60_TEST PASS\n");
    return 0;
}
