// Test for 0x007029F0 CTCQuestCard::GetInterfaceType: the virtual returns the class's interface type 0x6C.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCQuestCard {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCQuestCard::GetInterfaceType() const
{
    return (ETCInterfaceType)0x6C;
}
int main()
{
    CTCQuestCard object;
    const CTCQuestCard* p = &object;
    if ((long)p->GetInterfaceType() != 0x6C) { printf("007029f0_TEST FAIL\n"); return 1; }
    printf("007029f0_TEST PASS\n");
    return 0;
}
