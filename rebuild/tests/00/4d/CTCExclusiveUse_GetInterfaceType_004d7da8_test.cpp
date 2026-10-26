// Test for 0x004D7DA8 CTCExclusiveUse::GetInterfaceType: the virtual returns the class's interface type 0x2F.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCExclusiveUse {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCExclusiveUse::GetInterfaceType() const
{
    return (ETCInterfaceType)0x2F;
}
int main()
{
    CTCExclusiveUse object;
    const CTCExclusiveUse* p = &object;
    if ((long)p->GetInterfaceType() != 0x2F) { printf("004d7da8_TEST FAIL\n"); return 1; }
    printf("004d7da8_TEST PASS\n");
    return 0;
}
