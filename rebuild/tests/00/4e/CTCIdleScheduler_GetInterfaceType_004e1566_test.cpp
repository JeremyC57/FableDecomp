// Test for 0x004E1566 CTCIdleScheduler::GetInterfaceType: the virtual returns the class's interface type 0xD3.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCIdleScheduler {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCIdleScheduler::GetInterfaceType() const
{
    return (ETCInterfaceType)0xD3;
}
int main()
{
    CTCIdleScheduler object;
    const CTCIdleScheduler* p = &object;
    if ((long)p->GetInterfaceType() != 0xD3) { printf("004e1566_TEST FAIL\n"); return 1; }
    printf("004e1566_TEST PASS\n");
    return 0;
}
