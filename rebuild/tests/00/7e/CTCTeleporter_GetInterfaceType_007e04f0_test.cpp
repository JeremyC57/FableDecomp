// Test for 0x007E04F0 CTCTeleporter::GetInterfaceType: the virtual returns the class's interface type 0xDD.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTeleporter {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTeleporter::GetInterfaceType() const
{
    return (ETCInterfaceType)0xDD;
}
int main()
{
    CTCTeleporter object;
    const CTCTeleporter* p = &object;
    if ((long)p->GetInterfaceType() != 0xDD) { printf("007e04f0_TEST FAIL\n"); return 1; }
    printf("007e04f0_TEST PASS\n");
    return 0;
}
