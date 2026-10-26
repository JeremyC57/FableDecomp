// Test for 0x007E0D70 CTCEffectOnDie::GetInterfaceType: the virtual returns the class's interface type 0x9B.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCEffectOnDie {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCEffectOnDie::GetInterfaceType() const
{
    return (ETCInterfaceType)0x9B;
}
int main()
{
    CTCEffectOnDie object;
    const CTCEffectOnDie* p = &object;
    if ((long)p->GetInterfaceType() != 0x9B) { printf("007e0d70_TEST FAIL\n"); return 1; }
    printf("007e0d70_TEST PASS\n");
    return 0;
}
