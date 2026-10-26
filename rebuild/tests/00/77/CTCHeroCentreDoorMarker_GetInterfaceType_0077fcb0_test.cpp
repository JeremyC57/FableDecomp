// Test for 0x0077FCB0 CTCHeroCentreDoorMarker::GetInterfaceType: the virtual returns the class's interface type 0x6E.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHeroCentreDoorMarker {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHeroCentreDoorMarker::GetInterfaceType() const
{
    return (ETCInterfaceType)0x6E;
}
int main()
{
    CTCHeroCentreDoorMarker object;
    const CTCHeroCentreDoorMarker* p = &object;
    if ((long)p->GetInterfaceType() != 0x6E) { printf("0077fcb0_TEST FAIL\n"); return 1; }
    printf("0077fcb0_TEST PASS\n");
    return 0;
}
