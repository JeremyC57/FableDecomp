// Test for 0x007C9E60 CTCCreatureGeneratorCreator::GetInterfaceType: the virtual returns the class's interface type 0xCA.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCreatureGeneratorCreator {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCreatureGeneratorCreator::GetInterfaceType() const
{
    return (ETCInterfaceType)0xCA;
}
int main()
{
    CTCCreatureGeneratorCreator object;
    const CTCCreatureGeneratorCreator* p = &object;
    if ((long)p->GetInterfaceType() != 0xCA) { printf("007c9e60_TEST FAIL\n"); return 1; }
    printf("007c9e60_TEST PASS\n");
    return 0;
}
