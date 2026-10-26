// Test for 0x007BB9E0 CTCDrunkenness::GetInterfaceType: the virtual returns the class's interface type 0xC3.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDrunkenness {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDrunkenness::GetInterfaceType() const
{
    return (ETCInterfaceType)0xC3;
}
int main()
{
    CTCDrunkenness object;
    const CTCDrunkenness* p = &object;
    if ((long)p->GetInterfaceType() != 0xC3) { printf("007bb9e0_TEST FAIL\n"); return 1; }
    printf("007bb9e0_TEST PASS\n");
    return 0;
}
