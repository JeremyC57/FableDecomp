// Test for 0x004E5F3B CTCCreatureGroupTag::GetInterfaceType: the virtual returns the class's interface type 0xA4.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCreatureGroupTag {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCreatureGroupTag::GetInterfaceType() const
{
    return (ETCInterfaceType)0xA4;
}
int main()
{
    CTCCreatureGroupTag object;
    const CTCCreatureGroupTag* p = &object;
    if ((long)p->GetInterfaceType() != 0xA4) { printf("004e5f3b_TEST FAIL\n"); return 1; }
    printf("004e5f3b_TEST PASS\n");
    return 0;
}
