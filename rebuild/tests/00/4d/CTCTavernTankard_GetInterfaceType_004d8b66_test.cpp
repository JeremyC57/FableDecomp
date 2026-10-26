// Test for 0x004D8B66 CTCTavernTankard::GetInterfaceType: the virtual returns the class's interface type 0xBA.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCTavernTankard {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTavernTankard::GetInterfaceType() const
{
    return (ETCInterfaceType)0xBA;
}
int main()
{
    CTCTavernTankard object;
    const CTCTavernTankard* p = &object;
    if ((long)p->GetInterfaceType() != 0xBA) { printf("004d8b66_TEST FAIL\n"); return 1; }
    printf("004d8b66_TEST PASS\n");
    return 0;
}
