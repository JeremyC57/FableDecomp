// Test for 0x007E3760 CTCAtmosPlayer::GetInterfaceType: the virtual returns the class's interface type 0xFF.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCAtmosPlayer {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCAtmosPlayer::GetInterfaceType() const
{
    return (ETCInterfaceType)0xFF;
}
int main()
{
    CTCAtmosPlayer object;
    const CTCAtmosPlayer* p = &object;
    if ((long)p->GetInterfaceType() != 0xFF) { printf("007e3760_TEST FAIL\n"); return 1; }
    printf("007e3760_TEST PASS\n");
    return 0;
}
