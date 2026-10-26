// Test for 0x004D7E7C CTCOwnedEntity::GetInterfaceType: the virtual returns the class's interface type 0x33.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCOwnedEntity {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCOwnedEntity::GetInterfaceType() const
{
    return (ETCInterfaceType)0x33;
}
int main()
{
    CTCOwnedEntity object;
    const CTCOwnedEntity* p = &object;
    if ((long)p->GetInterfaceType() != 0x33) { printf("004d7e7c_TEST FAIL\n"); return 1; }
    printf("004d7e7c_TEST PASS\n");
    return 0;
}
