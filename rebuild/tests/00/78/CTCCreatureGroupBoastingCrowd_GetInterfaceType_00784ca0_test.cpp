// Test for 0x00784CA0 CTCCreatureGroupBoastingCrowd::GetInterfaceType: the virtual returns the class's interface type 0x74.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCreatureGroupBoastingCrowd {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCreatureGroupBoastingCrowd::GetInterfaceType() const
{
    return (ETCInterfaceType)0x74;
}
int main()
{
    CTCCreatureGroupBoastingCrowd object;
    const CTCCreatureGroupBoastingCrowd* p = &object;
    if ((long)p->GetInterfaceType() != 0x74) { printf("00784ca0_TEST FAIL\n"); return 1; }
    printf("00784ca0_TEST PASS\n");
    return 0;
}
