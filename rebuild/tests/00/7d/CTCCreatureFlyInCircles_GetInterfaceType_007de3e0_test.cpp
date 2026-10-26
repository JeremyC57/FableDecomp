// Test for 0x007DE3E0 CTCCreatureFlyInCircles::GetInterfaceType: the virtual returns the class's interface type 0xE4.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCreatureFlyInCircles {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCreatureFlyInCircles::GetInterfaceType() const
{
    return (ETCInterfaceType)0xE4;
}
int main()
{
    CTCCreatureFlyInCircles object;
    const CTCCreatureFlyInCircles* p = &object;
    if ((long)p->GetInterfaceType() != 0xE4) { printf("007de3e0_TEST FAIL\n"); return 1; }
    printf("007de3e0_TEST PASS\n");
    return 0;
}
