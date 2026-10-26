// Test for 0x007CC950 CTCMultiArrow::GetInterfaceType: the virtual returns the class's interface type 0x88.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCMultiArrow {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCMultiArrow::GetInterfaceType() const
{
    return (ETCInterfaceType)0x88;
}
int main()
{
    CTCMultiArrow object;
    const CTCMultiArrow* p = &object;
    if ((long)p->GetInterfaceType() != 0x88) { printf("007cc950_TEST FAIL\n"); return 1; }
    printf("007cc950_TEST PASS\n");
    return 0;
}
