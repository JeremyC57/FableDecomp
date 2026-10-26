// Test for 0x0056DF80 CTCBoastUI::GetInterfaceType: the virtual returns the class's interface type 0xE8.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCBoastUI {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCBoastUI::GetInterfaceType() const
{
    return (ETCInterfaceType)0xE8;
}
int main()
{
    CTCBoastUI object;
    const CTCBoastUI* p = &object;
    if ((long)p->GetInterfaceType() != 0xE8) { printf("0056df80_TEST FAIL\n"); return 1; }
    printf("0056df80_TEST PASS\n");
    return 0;
}
