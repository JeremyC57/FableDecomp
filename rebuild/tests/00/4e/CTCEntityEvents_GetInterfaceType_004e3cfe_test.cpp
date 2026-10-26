// Test for 0x004E3CFE CTCEntityEvents::GetInterfaceType: the virtual returns the class's interface type 0x42.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCEntityEvents {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCEntityEvents::GetInterfaceType() const
{
    return (ETCInterfaceType)0x42;
}
int main()
{
    CTCEntityEvents object;
    const CTCEntityEvents* p = &object;
    if ((long)p->GetInterfaceType() != 0x42) { printf("004e3cfe_TEST FAIL\n"); return 1; }
    printf("004e3cfe_TEST PASS\n");
    return 0;
}
