// Test for 0x004D420E CTCDNavigationSeed::GetInterfaceType: the virtual returns the class's interface type 0x58.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCDNavigationSeed {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCDNavigationSeed::GetInterfaceType() const
{
    return (ETCInterfaceType)0x58;
}
int main()
{
    CTCDNavigationSeed object;
    const CTCDNavigationSeed* p = &object;
    if ((long)p->GetInterfaceType() != 0x58) { printf("004d420e_TEST FAIL\n"); return 1; }
    printf("004d420e_TEST PASS\n");
    return 0;
}
