// Test for 0x0066C290 CTCSoundPlayer::GetInterfaceType: the virtual returns the class's interface type 0x10.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCSoundPlayer {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCSoundPlayer::GetInterfaceType() const
{
    return (ETCInterfaceType)0x10;
}
int main()
{
    CTCSoundPlayer object;
    const CTCSoundPlayer* p = &object;
    if ((long)p->GetInterfaceType() != 0x10) { printf("0066c290_TEST FAIL\n"); return 1; }
    printf("0066c290_TEST PASS\n");
    return 0;
}
