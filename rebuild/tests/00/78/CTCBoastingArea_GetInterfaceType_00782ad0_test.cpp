// Test for 0x00782AD0 CTCBoastingArea::GetInterfaceType: the virtual returns the class's interface type 0x72.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCBoastingArea {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCBoastingArea::GetInterfaceType() const
{
    return (ETCInterfaceType)0x72;
}
int main()
{
    CTCBoastingArea object;
    const CTCBoastingArea* p = &object;
    if ((long)p->GetInterfaceType() != 0x72) { printf("00782ad0_TEST FAIL\n"); return 1; }
    printf("00782ad0_TEST PASS\n");
    return 0;
}
