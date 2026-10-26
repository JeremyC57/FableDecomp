// Test for 0x007E4C10 CTCCreditsUI::GetInterfaceType: the virtual returns the class's interface type 0x101.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCreditsUI {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCreditsUI::GetInterfaceType() const
{
    return (ETCInterfaceType)0x101;
}
int main()
{
    CTCCreditsUI object;
    const CTCCreditsUI* p = &object;
    if ((long)p->GetInterfaceType() != 0x101) { printf("007e4c10_TEST FAIL\n"); return 1; }
    printf("007e4c10_TEST PASS\n");
    return 0;
}
