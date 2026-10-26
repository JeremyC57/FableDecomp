// Test for 0x0076D950 CTCBandit::GetInterfaceType: the virtual returns the class's interface type 0x4A.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCBandit {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCBandit::GetInterfaceType() const
{
    return (ETCInterfaceType)0x4A;
}
int main()
{
    CTCBandit object;
    const CTCBandit* p = &object;
    if ((long)p->GetInterfaceType() != 0x4A) { printf("0076d950_TEST FAIL\n"); return 1; }
    printf("0076d950_TEST PASS\n");
    return 0;
}
