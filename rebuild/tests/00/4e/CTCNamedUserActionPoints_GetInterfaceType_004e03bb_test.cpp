// Test for 0x004E03BB CTCNamedUserActionPoints::GetInterfaceType: the virtual returns the class's interface type 0x2E.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCNamedUserActionPoints {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCNamedUserActionPoints::GetInterfaceType() const
{
    return (ETCInterfaceType)0x2E;
}
int main()
{
    CTCNamedUserActionPoints object;
    const CTCNamedUserActionPoints* p = &object;
    if ((long)p->GetInterfaceType() != 0x2E) { printf("004e03bb_TEST FAIL\n"); return 1; }
    printf("004e03bb_TEST PASS\n");
    return 0;
}
