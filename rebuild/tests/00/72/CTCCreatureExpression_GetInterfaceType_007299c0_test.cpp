// Test for 0x007299C0 CTCCreatureExpression::GetInterfaceType: the virtual returns the class's interface type 0xC.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCreatureExpression {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCreatureExpression::GetInterfaceType() const
{
    return (ETCInterfaceType)0xC;
}
int main()
{
    CTCCreatureExpression object;
    const CTCCreatureExpression* p = &object;
    if ((long)p->GetInterfaceType() != 0xC) { printf("007299c0_TEST FAIL\n"); return 1; }
    printf("007299c0_TEST PASS\n");
    return 0;
}
