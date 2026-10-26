// Test for 0x007787F0 CTCTimeAppearanceFade::GetInterfaceType: the virtual returns the class's interface type 0x56.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTimeAppearanceFade {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTimeAppearanceFade::GetInterfaceType() const
{
    return (ETCInterfaceType)0x56;
}
int main()
{
    CTCTimeAppearanceFade object;
    const CTCTimeAppearanceFade* p = &object;
    if ((long)p->GetInterfaceType() != 0x56) { printf("007787f0_TEST FAIL\n"); return 1; }
    printf("007787f0_TEST PASS\n");
    return 0;
}
