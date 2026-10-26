// Test for 0x004D900B CTCGamePlayer::GetInterfaceType: the virtual returns the class's interface type 0xD0.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCGamePlayer {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCGamePlayer::GetInterfaceType() const
{
    return (ETCInterfaceType)0xD0;
}
int main()
{
    CTCGamePlayer object;
    const CTCGamePlayer* p = &object;
    if ((long)p->GetInterfaceType() != 0xD0) { printf("004d900b_TEST FAIL\n"); return 1; }
    printf("004d900b_TEST PASS\n");
    return 0;
}
