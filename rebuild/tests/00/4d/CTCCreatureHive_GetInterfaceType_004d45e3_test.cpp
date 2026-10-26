// Test for 0x004D45E3 CTCCreatureHive::GetInterfaceType: the virtual returns the class's interface type 0x62.
#include <stdio.h>
enum ETCInterfaceType { };
class CTCCreatureHive {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCCreatureHive::GetInterfaceType() const
{
    return (ETCInterfaceType)0x62;
}
int main()
{
    CTCCreatureHive object;
    const CTCCreatureHive* p = &object;
    if ((long)p->GetInterfaceType() != 0x62) { printf("004d45e3_TEST FAIL\n"); return 1; }
    printf("004d45e3_TEST PASS\n");
    return 0;
}
