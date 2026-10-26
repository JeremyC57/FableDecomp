// Test for 0x004DB52A CTCLightningOrb::GetInterfaceType: the virtual returns the class's interface type 0x10A.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCLightningOrb {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCLightningOrb::GetInterfaceType() const
{
    return (ETCInterfaceType)0x10A;
}
int main()
{
    CTCLightningOrb object;
    const CTCLightningOrb* p = &object;
    if ((long)p->GetInterfaceType() != 0x10A) { printf("004db52a_TEST FAIL\n"); return 1; }
    printf("004db52a_TEST PASS\n");
    return 0;
}
