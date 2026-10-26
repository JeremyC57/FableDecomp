// Test for 0x00686680 CTCCreatureOpinionOfHero::GetInterfaceType: the virtual returns the class's interface type 0x4F.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCreatureOpinionOfHero {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCreatureOpinionOfHero::GetInterfaceType() const
{
    return (ETCInterfaceType)0x4F;
}
int main()
{
    CTCCreatureOpinionOfHero object;
    const CTCCreatureOpinionOfHero* p = &object;
    if ((long)p->GetInterfaceType() != 0x4F) { printf("00686680_TEST FAIL\n"); return 1; }
    printf("00686680_TEST PASS\n");
    return 0;
}
