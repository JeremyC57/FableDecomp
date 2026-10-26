// Test for 0x004DB55F CTCHasName::GetInterfaceType: the virtual returns the class's interface type 0x10C.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCHasName {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCHasName::GetInterfaceType() const
{
    return (ETCInterfaceType)0x10C;
}
int main()
{
    CTCHasName object;
    const CTCHasName* p = &object;
    if ((long)p->GetInterfaceType() != 0x10C) { printf("004db55f_TEST FAIL\n"); return 1; }
    printf("004db55f_TEST PASS\n");
    return 0;
}
