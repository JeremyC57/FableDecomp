// Test for 0x004D96A1 CTCFlourishTarget::GetInterfaceType: the virtual returns the class's interface type 0x100.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCFlourishTarget {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCFlourishTarget::GetInterfaceType() const
{
    return (ETCInterfaceType)0x100;
}
int main()
{
    CTCFlourishTarget object;
    const CTCFlourishTarget* p = &object;
    if ((long)p->GetInterfaceType() != 0x100) { printf("004d96a1_TEST FAIL\n"); return 1; }
    printf("004d96a1_TEST PASS\n");
    return 0;
}
