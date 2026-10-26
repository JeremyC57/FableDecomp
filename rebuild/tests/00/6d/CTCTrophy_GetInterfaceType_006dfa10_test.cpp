// Test for 0x006DFA10 CTCTrophy::GetInterfaceType: the virtual returns the class's interface type 0x39.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTrophy {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTrophy::GetInterfaceType() const
{
    return (ETCInterfaceType)0x39;
}
int main()
{
    CTCTrophy object;
    const CTCTrophy* p = &object;
    if ((long)p->GetInterfaceType() != 0x39) { printf("006dfa10_TEST FAIL\n"); return 1; }
    printf("006dfa10_TEST PASS\n");
    return 0;
}
