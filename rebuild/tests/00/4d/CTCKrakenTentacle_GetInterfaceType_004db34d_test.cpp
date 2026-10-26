// Test for 0x004DB34D CTCKrakenTentacle::GetInterfaceType: the virtual returns the class's interface type 0xE3.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCKrakenTentacle {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCKrakenTentacle::GetInterfaceType() const
{
    return (ETCInterfaceType)0xE3;
}
int main()
{
    CTCKrakenTentacle object;
    const CTCKrakenTentacle* p = &object;
    if ((long)p->GetInterfaceType() != 0xE3) { printf("004db34d_TEST FAIL\n"); return 1; }
    printf("004db34d_TEST PASS\n");
    return 0;
}
