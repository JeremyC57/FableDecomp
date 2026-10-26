// Test for 0x005E9720 CTCQuestCompletionUI::GetInterfaceType: the virtual returns the class's interface type 0xFC.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCQuestCompletionUI {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCQuestCompletionUI::GetInterfaceType() const
{
    return (ETCInterfaceType)0xFC;
}
int main()
{
    CTCQuestCompletionUI object;
    const CTCQuestCompletionUI* p = &object;
    if ((long)p->GetInterfaceType() != 0xFC) { printf("005e9720_TEST FAIL\n"); return 1; }
    printf("005e9720_TEST PASS\n");
    return 0;
}
