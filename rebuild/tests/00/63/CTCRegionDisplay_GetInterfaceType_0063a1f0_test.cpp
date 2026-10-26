// Test for 0x0063A1F0 CTCRegionDisplay::GetInterfaceType: the virtual returns the class's interface type 0x8E.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCRegionDisplay {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCRegionDisplay::GetInterfaceType() const
{
    return (ETCInterfaceType)0x8E;
}
int main()
{
    CTCRegionDisplay object;
    const CTCRegionDisplay* p = &object;
    if ((long)p->GetInterfaceType() != 0x8E) { printf("0063a1f0_TEST FAIL\n"); return 1; }
    printf("0063a1f0_TEST PASS\n");
    return 0;
}
