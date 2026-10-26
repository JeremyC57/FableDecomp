// Test for 0x007BAAB0 CTCTavernBar::GetInterfaceType: the virtual returns the class's interface type 0xB9.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTavernBar {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTavernBar::GetInterfaceType() const
{
    return (ETCInterfaceType)0xB9;
}
int main()
{
    CTCTavernBar object;
    const CTCTavernBar* p = &object;
    if ((long)p->GetInterfaceType() != 0xB9) { printf("007baab0_TEST FAIL\n"); return 1; }
    printf("007baab0_TEST PASS\n");
    return 0;
}
