// Test for 0x007A9280 CTCBattleCharge::GetInterfaceType: the virtual returns the class's interface type 0xA8.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCBattleCharge {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCBattleCharge::GetInterfaceType() const
{
    return (ETCInterfaceType)0xA8;
}
int main()
{
    CTCBattleCharge object;
    const CTCBattleCharge* p = &object;
    if ((long)p->GetInterfaceType() != 0xA8) { printf("007a9280_TEST FAIL\n"); return 1; }
    printf("007a9280_TEST PASS\n");
    return 0;
}
