// Test for 0x004DDB18 CTCThingOwner::GetInterfaceType: the virtual returns the class's interface type 0x21.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCThingOwner {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCThingOwner::GetInterfaceType() const
{
    return (ETCInterfaceType)0x21;
}
int main()
{
    CTCThingOwner object;
    const CTCThingOwner* p = &object;
    if ((long)p->GetInterfaceType() != 0x21) { printf("004ddb18_TEST FAIL\n"); return 1; }
    printf("004ddb18_TEST PASS\n");
    return 0;
}
