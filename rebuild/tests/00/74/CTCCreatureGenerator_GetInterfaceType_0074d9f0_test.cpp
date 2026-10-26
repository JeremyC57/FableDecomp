// Test for 0x0074D9F0 CTCCreatureGenerator::GetInterfaceType: the virtual returns the class's interface type 0x3D.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCreatureGenerator {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCreatureGenerator::GetInterfaceType() const
{
    return (ETCInterfaceType)0x3D;
}
int main()
{
    CTCCreatureGenerator object;
    const CTCCreatureGenerator* p = &object;
    if ((long)p->GetInterfaceType() != 0x3D) { printf("0074d9f0_TEST FAIL\n"); return 1; }
    printf("0074d9f0_TEST PASS\n");
    return 0;
}
