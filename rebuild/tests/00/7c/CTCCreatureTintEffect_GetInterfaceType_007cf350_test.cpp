// Test for 0x007CF350 CTCCreatureTintEffect::GetInterfaceType: the virtual returns the class's interface type 0x8B.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCreatureTintEffect {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCreatureTintEffect::GetInterfaceType() const
{
    return (ETCInterfaceType)0x8B;
}
int main()
{
    CTCCreatureTintEffect object;
    const CTCCreatureTintEffect* p = &object;
    if ((long)p->GetInterfaceType() != 0x8B) { printf("007cf350_TEST FAIL\n"); return 1; }
    printf("007cf350_TEST PASS\n");
    return 0;
}
