// Test for 0x007DD180 CTCCreatureFlyOnSpline::GetInterfaceType: the virtual returns the class's interface type 0xD9.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCreatureFlyOnSpline {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCreatureFlyOnSpline::GetInterfaceType() const
{
    return (ETCInterfaceType)0xD9;
}
int main()
{
    CTCCreatureFlyOnSpline object;
    const CTCCreatureFlyOnSpline* p = &object;
    if ((long)p->GetInterfaceType() != 0xD9) { printf("007dd180_TEST FAIL\n"); return 1; }
    printf("007dd180_TEST PASS\n");
    return 0;
}
