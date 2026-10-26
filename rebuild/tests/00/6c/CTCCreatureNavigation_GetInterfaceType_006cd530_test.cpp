// Test for 0x006CD530 CTCCreatureNavigation::GetInterfaceType: the virtual returns the class's interface type 0x7.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCreatureNavigation {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCreatureNavigation::GetInterfaceType() const
{
    return (ETCInterfaceType)0x7;
}
int main()
{
    CTCCreatureNavigation object;
    const CTCCreatureNavigation* p = &object;
    if ((long)p->GetInterfaceType() != 0x7) { printf("006cd530_TEST FAIL\n"); return 1; }
    printf("006cd530_TEST PASS\n");
    return 0;
}
