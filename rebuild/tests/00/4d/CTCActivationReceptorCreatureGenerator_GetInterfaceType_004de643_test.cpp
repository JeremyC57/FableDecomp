// Test for 0x004DE643 CTCActivationReceptorCreatureGenerator::GetInterfaceType: the virtual returns the class's interface type 0x65.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCActivationReceptorCreatureGenerator {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCActivationReceptorCreatureGenerator::GetInterfaceType() const
{
    return (ETCInterfaceType)0x65;
}
int main()
{
    CTCActivationReceptorCreatureGenerator object;
    const CTCActivationReceptorCreatureGenerator* p = &object;
    if ((long)p->GetInterfaceType() != 0x65) { printf("004de643_TEST FAIL\n"); return 1; }
    printf("004de643_TEST PASS\n");
    return 0;
}
