// Test for 0x0076D870 CTCEnemy::GetInterfaceType: the virtual returns the class's interface type 0x49.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCEnemy {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCEnemy::GetInterfaceType() const
{
    return (ETCInterfaceType)0x49;
}
int main()
{
    CTCEnemy object;
    const CTCEnemy* p = &object;
    if ((long)p->GetInterfaceType() != 0x49) { printf("0076d870_TEST FAIL\n"); return 1; }
    printf("0076d870_TEST PASS\n");
    return 0;
}
