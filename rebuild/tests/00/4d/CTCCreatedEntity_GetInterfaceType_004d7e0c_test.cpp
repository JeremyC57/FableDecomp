// Test for 0x004D7E0C CTCCreatedEntity::GetInterfaceType: the virtual returns the class's interface type 0x32.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCreatedEntity {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCreatedEntity::GetInterfaceType() const
{
    return (ETCInterfaceType)0x32;
}
int main()
{
    CTCCreatedEntity object;
    const CTCCreatedEntity* p = &object;
    if ((long)p->GetInterfaceType() != 0x32) { printf("004d7e0c_TEST FAIL\n"); return 1; }
    printf("004d7e0c_TEST PASS\n");
    return 0;
}
