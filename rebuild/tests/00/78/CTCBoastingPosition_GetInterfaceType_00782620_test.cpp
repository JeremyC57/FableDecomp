// Test for 0x00782620 CTCBoastingPosition::GetInterfaceType: the virtual returns the class's interface type 0x71.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCBoastingPosition {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCBoastingPosition::GetInterfaceType() const
{
    return (ETCInterfaceType)0x71;
}
int main()
{
    CTCBoastingPosition object;
    const CTCBoastingPosition* p = &object;
    if ((long)p->GetInterfaceType() != 0x71) { printf("00782620_TEST FAIL\n"); return 1; }
    printf("00782620_TEST PASS\n");
    return 0;
}
