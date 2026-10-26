// Test for 0x007CED00 CTCElectrocutedEffect::GetInterfaceType: the virtual returns the class's interface type 0x8A.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCElectrocutedEffect {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCElectrocutedEffect::GetInterfaceType() const
{
    return (ETCInterfaceType)0x8A;
}
int main()
{
    CTCElectrocutedEffect object;
    const CTCElectrocutedEffect* p = &object;
    if ((long)p->GetInterfaceType() != 0x8A) { printf("007ced00_TEST FAIL\n"); return 1; }
    printf("007ced00_TEST PASS\n");
    return 0;
}
