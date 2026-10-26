// Test for 0x004E5C5B CTCCreatureGroup::GetInterfaceType: the virtual returns the class's interface type 0xA3.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCreatureGroup {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCreatureGroup::GetInterfaceType() const
{
    return (ETCInterfaceType)0xA3;
}
int main()
{
    CTCCreatureGroup object;
    const CTCCreatureGroup* p = &object;
    if ((long)p->GetInterfaceType() != 0xA3) { printf("004e5c5b_TEST FAIL\n"); return 1; }
    printf("004e5c5b_TEST PASS\n");
    return 0;
}
