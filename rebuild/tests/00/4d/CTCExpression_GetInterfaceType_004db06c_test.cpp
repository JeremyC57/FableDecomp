// Test for 0x004DB06C CTCExpression::GetInterfaceType: the virtual returns the class's interface type 0x8F.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCExpression {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCExpression::GetInterfaceType() const
{
    return (ETCInterfaceType)0x8F;
}
int main()
{
    CTCExpression object;
    const CTCExpression* p = &object;
    if ((long)p->GetInterfaceType() != 0x8F) { printf("004db06c_TEST FAIL\n"); return 1; }
    printf("004db06c_TEST PASS\n");
    return 0;
}
