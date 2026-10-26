// Test for 0x004D43DF CTCActivationTrigger::GetInterfaceType: the virtual returns the class's interface type 0x66.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCActivationTrigger {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCActivationTrigger::GetInterfaceType() const
{
    return (ETCInterfaceType)0x66;
}
int main()
{
    CTCActivationTrigger object;
    const CTCActivationTrigger* p = &object;
    if ((long)p->GetInterfaceType() != 0x66) { printf("004d43df_TEST FAIL\n"); return 1; }
    printf("004d43df_TEST PASS\n");
    return 0;
}
