// Test for 0x004D49FD CTCMoneyReceiver::GetInterfaceType: the virtual returns the class's interface type 0x80.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCMoneyReceiver {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCMoneyReceiver::GetInterfaceType() const
{
    return (ETCInterfaceType)0x80;
}
int main()
{
    CTCMoneyReceiver object;
    const CTCMoneyReceiver* p = &object;
    if ((long)p->GetInterfaceType() != 0x80) { printf("004d49fd_TEST FAIL\n"); return 1; }
    printf("004d49fd_TEST PASS\n");
    return 0;
}
